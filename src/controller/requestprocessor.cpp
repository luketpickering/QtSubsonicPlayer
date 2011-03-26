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
#include "../dal/binrequests/retrievetrackstream.h"


RequestProcessor::RequestProcessor(ConnectionData* _cd, QObject* parent) : QObject(parent), xch(parent)
{
    requestRunning = false;
    cacheResetting = false;
	consecutiveReqs = 0;
    connData = _cd;
    connect(&xch, SIGNAL(requireHardReset()), this, SIGNAL(cacheRequiresReset()));
    xch.loadCacheFromDisk();
}

void RequestProcessor::getIndex()
{
    QStringList* index = xch.getCachedIndex();

    if(index == 0)
    {
        hardResetCache();
    }
    else
    {
        emit retrievedIndex(index);
    }
}

void RequestProcessor::hardResetCache()
{
    cacheResetting = true;

	clearQueue();

    RetrieveIndex* indReq = new RetrieveIndex(connData,this);
    connect(indReq,SIGNAL(gotIndex(QDomDocument*)),
            this, SLOT(responseIndex(QDomDocument*)));

    addToQueue(indReq);
}

void RequestProcessor::responseIndex(QDomDocument* _respXML)
{
    sender()->deleteLater();
    bool saved = xch.hardResetCache(_respXML);


    QStringList* index = xch.getCachedIndex();
    emit retrievedIndex(index);

    reqList.removeAt(0);


    cacheResetting = false;

    if(saved)
    {
        emit cacheReset();
        std::cout << "cache reset" << std::endl;
    }
    else
    {
        emit cacheResetFailed();
        std::cout << "new cache couldn't be saved properly" << std::endl;
    }



    requestRunning = false;
    runRequest();
}

void RequestProcessor::getArtist(QString _artistName)
{
    if(!cacheResetting)
    {
        QString id = QString();
        QStringList* artistDir = xch.getCachedArtist(_artistName, &id);

        //this needs to be better - send in a bool rather than this
        if(artistDir == 0)
        {
            if(id.compare("null") == 0)
            {
                std::cout << "cannot find id for Artist directory " <<
                             qPrintable(_artistName) << "\n" << std::endl;
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
    else
        emit cacheIsResetting();
}

void RequestProcessor::responseArtist(QDomDocument* _respXML, 
                                      QString _artistName)
{
    sender()->deleteLater();
    xch.saveArtist(_respXML, _artistName);
    
	if(consecutiveReqs > 10)
	{
		xch.saveCacheToDisk();
		consecutiveReqs = 0;
	}

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
    if(!cacheResetting)
    {
        QString id = QString();

        /* /
        QStringList* albumDir = xch.getCachedAlbum(_artistName,
                                                   _albumName, &id);
        /* */

       QList< QPair<QString,QString> >* albumDir = xch.getCachedWholeAlbum(_artistName,_albumName,&id);

        if(albumDir == 0)
        {
            if(id.compare("null") == 0)
            {
                std::cout << "cannot find id for album " <<
                             qPrintable(_albumName) << "\n" << std::endl;
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
            emit retrievedAlbumListing(albumDir,_artistName,_albumName);
        }
    }
    else
        emit cacheIsResetting();
}

void RequestProcessor::responseAlbum(QDomDocument* _respXML, QString _artistName, 
                                     QString _albumName)
{
    sender()->deleteLater();
    xch.saveAlbum(_respXML, _artistName, _albumName);

	if(consecutiveReqs > 10)
	{
		xch.saveCacheToDisk();
		consecutiveReqs = 0;
	}

    if(((SubRequest*)sender())->checkBumps() == 0 )
    {
        QString id = QString();

        /*   /
        QStringList* albumDir = xch.getCachedAlbum(_artistName,
                                                   _albumName,&id);
        /*   */

        QList< QPair<QString,QString> >* albumDir = xch.getCachedWholeAlbum(_artistName,_albumName,&id);
        emit retrievedAlbumListing(albumDir,_artistName, _albumName);
    }

    reqList.removeAt(0);
    requestRunning = false;
    runRequest();
}

void RequestProcessor::requestPing()
{
    if(!cacheResetting)
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
    else
        emit cacheIsResetting();

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
    if(!cacheResetting)
    {
        QString id = xch.getCacheTrackID( _artistName, _albumName, _track);
        int length = xch.getCacheTrackLength( _artistName, _albumName, _track);
        emit retrievedTrackData(id, length);
    }
    else
        emit cacheIsResetting();
}

/*
void RequestProcessor::getTrack(QString _artistName, QString _albumName,
                                QString _track)
{
    if(!cacheResetting)
    {
        QString id = xch.getCacheTrackID( _artistName, _albumName, _track);

        if(id.compare("null") == 0)
        {
            std::cout << "cannot find id for track " <<
                         qPrintable(_track) << "\n" << std::endl;
            //do hard reset
        }
        else
        {
            RetrieveTrackStream* reqRts =
                    new RetrieveTrackStream(connData, id,
                                            _artistName, _albumName, _track, this);

            connect(reqRts, SIGNAL(gotTrackStream(QBuffer*,
                                                  QString, QString, QString)),
                    this,SLOT(responseTrackStream(QBuffer*,
                                                  QString, QString, QString)));

            addToQueue(reqRts);
        }
    }
    else
        emit cacheIsResetting();
}
void RequestProcessor::responseTrackStream(QBuffer* _buf, 
                                           QString _artistName, QString _albumName, QString _track)
{
    emit retrievedOpenTrackStream(_buf, _artistName, _albumName, _track);

    reqList.removeAt(0);
    requestRunning = false;
    runRequest();
}
*/


void RequestProcessor::clearQueue()
{

	int deleteUntil = (requestRunning) ? 1 : 0;

    while(reqList.count() > deleteUntil)
    {
		reqList.at(deleteUntil)->deleteLater();
        reqList.removeAt(deleteUntil);
    }

	if(deleteUntil == 1)
	{
		reqList.at(0)->bump();
		reqList.at(0)->deleteLater();
		reqList.removeAt(0);
	}
}

void RequestProcessor::addToQueue(SubRequest* _toAdd)
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
}
void RequestProcessor::runRequest()
{
	if(reqList.length() > 0 && requestRunning == false)
	{
		requestRunning = true;
		reqList.at(0)->makeRequest();
		++consecutiveReqs;
	}
}

bool RequestProcessor::checkAlreadyQueued(SubRequest* _toCheck)
{
	for(int ctr = 0; ctr < reqList.count(); ++ctr)
	{
		if(reqList.at(ctr)->serialiseRequest()
			.compare(_toCheck->serialiseRequest()) == 0)
		{
			reqList.move(ctr,1);
                        reqList.at(1)->resetBumps();
			std::cout << qPrintable(reqList.at(1)->serialiseRequest()) 
			<< " re-prioritised. will go out next. \n" << std::endl;
			return true;
		}
	}
	return false;
}
