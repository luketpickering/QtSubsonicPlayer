#include <QString>
#include <QObject>
#include <QList>
#include <QStringList>
#include <iostream>
#include "xmlcachehandler.h"
#include "../dal/connectiondata.h"
#include "requestprocessor.h"
#include "../dal/xmlrequests/retrieveindex.h"
#include "../dal/xmlrequests/retrieveartistdir.h"
#include "../dal/xmlrequests/retrievealbumdir.h"
#include "../dal/xmlrequests/pingtest.h"


RequestProcessor::RequestProcessor(ConnectionData* _cd, QObject* parent) : QObject(parent), xch(parent)
{
	requestRunning = false;
	connData = _cd;
	requestPing();
	connect(&xch, SIGNAL(requireHardReset()), this, SLOT(hardReset()));
	xch.loadCacheFromDisk();
}

void RequestProcessor::hardReset()
{
	emit cacheRequiresReset();
}

void RequestProcessor::getIndex()
{
	QStringList* index = xch.getCachedIndex();

	if(index == 0)
	{
		RetrieveIndex* indReq = new RetrieveIndex(connData,this);
		connect(indReq,SIGNAL(gotIndex(QDomDocument*)),
			this, SLOT(responseIndex(QDomDocument*)));

		if(!checkAlreadyQueued(indReq))
		{
			addToQueue(indReq);
		}
		else
		{
			delete indReq;
			std::cout << "Index Request already Queued\n" << std::endl;
		}
	}
	else
	{	
		emit retrievedIndex(index);
	}
}
void RequestProcessor::responseIndex(QDomDocument* _respXML)
{
	sender()->deleteLater();
	xch.hardResetCache(_respXML);

	if(((SubRequest*)sender())->checkBumps() == 0 )
	{
		QStringList* index = xch.getCachedIndex();
		emit retrievedIndex(index);
	}
	
	reqList.removeAt(0);
	requestRunning = false;
	runRequest();
}

void RequestProcessor::getArtist(QString _artistName)
{
	QString id = QString();
	QStringList* artistDir = xch.getCachedArtist(_artistName, &id);

	//this needs to be better - send in a bool rather than this
	if(artistDir == 0)
	{
		if(id.compare("null") == 0)
		{
			std::cout << "null id found\n" << std::endl;
			//do hard reset
		}
		else
		{
			RetrieveArtistDir* artDirReq = new RetrieveArtistDir(connData,id
				,_artistName, this);
			connect(artDirReq,SIGNAL(gotArtistDir(QDomDocument*, QString)),
				this, SLOT(responseArtist(QDomDocument*,QString)));

			if(!checkAlreadyQueued(artDirReq))
			{
				addToQueue(artDirReq);
			}
			else
			{
				delete artDirReq;
				std::cout <<  "Request for " << 
					qPrintable(_artistName) << " already Queued\n" 
					<< std::endl;
			}
		}
	}
	else
	{	
		emit retrievedArtistListing(artistDir,_artistName);
	}
}
void RequestProcessor::responseArtist(QDomDocument* _respXML, 
	QString _artistName)
{
	sender()->deleteLater();
	xch.saveArtist(_respXML, _artistName);
	int bla = ((SubRequest*)sender())->checkBumps();

	if(((SubRequest*)sender())->checkBumps() == 0 )
	{
		QString id = QString();
		QStringList* artistDir = xch.getCachedArtist(_artistName,&id);
		emit retrievedArtistListing(artistDir,_artistName);

		std::cout << qPrintable(_artistName) << " signal was emitted\n" << std::endl;
	}
	else
	{
		std::cout << qPrintable(_artistName) << " was bumped storing, but not signalling\n" << std::endl;
	}

	reqList.removeAt(0);
	requestRunning = false;
	runRequest();
}

void RequestProcessor::getAlbum(QString _artistName, QString _albumName)
{
	QString id = QString();
	QStringList* albumDir = xch.getCachedAlbum(_artistName,
		_albumName, &id);

	if(albumDir == 0)
	{
		if(id.compare("null") == 0)
		{
			std::cout << "null id found\n" << std::endl;
			//do hard reset
		}
		else
		{
			RetrieveAlbumDir* albDirReq = new RetrieveAlbumDir(connData,id
				,_artistName, _albumName, this);

			connect(albDirReq,SIGNAL(gotAlbumDir(QDomDocument*,QString,
				QString)),
				this, SLOT(responseAlbum(QDomDocument*, QString,
				QString)));

			if(!checkAlreadyQueued(albDirReq))
			{
				addToQueue(albDirReq);
			}
			else
			{
				delete albDirReq;
				std::cout <<  "Request for " << 
					qPrintable(_albumName) << " already Queued\n" 
					<< std::endl;
			}
		}
	}
	else
	{	
		emit retrievedArtistListing(albumDir,_artistName);
	}
}
void RequestProcessor::responseAlbum(QDomDocument* _respXML, QString _artistName, 
	QString _albumName)
{
	sender()->deleteLater();
	xch.saveAlbum(_respXML, _artistName, _albumName);

	if(((SubRequest*)sender())->checkBumps() == 0 )
	{
		QString id = QString();
		QStringList* albumDir = xch.getCachedAlbum(_artistName,
			_albumName,&id);
		emit retrievedAlbumListing(albumDir,_artistName, _albumName);
	}

	reqList.removeAt(0);
	requestRunning = false;
	runRequest();
}

void RequestProcessor::requestPing()
{
	PingTest* pt = new PingTest(connData, this);
	connect(pt,SIGNAL(pingResponded()),this,SLOT(responsePing()));
	connect(pt,SIGNAL(serverPingOk()),this, SIGNAL(pingSucceded()));
	connect(pt,SIGNAL(serverPingServerError(int)),
		this, SIGNAL(pingFailed(int)));

	if(!checkAlreadyQueued(pt))
	{
		addToQueue(pt);
	}
	else
	{
		delete pt;
		std::cout <<  "Request for Ping already Queued\n" << std::endl;
	}
				
}
void RequestProcessor::responsePing()
{
	sender()->deleteLater();
	reqList.removeAt(0);
	requestRunning = false;
	runRequest();
}
void RequestProcessor::getTrack(QString _artistName, QString _albumName,
	QString _track)
{

}

bool RequestProcessor::addToQueue(SubRequest* _toAdd)
{
	reqList.insert((reqList.count() > 0)? 1:0,_toAdd);

	for(int ctr = 2; ctr < reqList.count(); ++ctr)
	{
		reqList.at(ctr)->bump();
		std::cout << qPrintable(reqList.at(ctr)->serialiseRequest()) 
			<< " bumped \n" << std::endl;
	}

	if(!requestRunning)
		runRequest();
	return true;
}

void RequestProcessor::runRequest()
{
	if(reqList.length() > 0 && requestRunning == false)
	{
		requestRunning = true;
		reqList.at(0)->makeRequest();
	}
}

void RequestProcessor::responseTrack(QDomDocument* _respXML, QString _artistName, 
	QString _albumName, QString _track){}

bool RequestProcessor::checkAlreadyQueued(SubRequest* _toCheck)
{
	for(int ctr = 0; ctr < reqList.count(); ++ctr)
	{
		if(reqList.at(ctr)->serialiseRequest()
			.compare(_toCheck->serialiseRequest()) == 0)
		{
			return true;
		}
	}
	return false;
}
