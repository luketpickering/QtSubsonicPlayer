#include "xmlcachehandler.h"
#include <QFile>
#include "../dal/xmlrequests/retrieveindex.h"
#include "../dal/connectiondata.h"
#include "../dal/xmlrequests/retrievedirectory.h"

XMLCacheHandler::XMLCacheHandler(ConnectionData* _cd, QObject* parent) : QObject(parent)
{
	conndata = _cd;
	gotConnData = true;
	cacheFile = 0;
}

XMLCacheHandler::XMLCacheHandler(QObject* parent) : QObject(parent)
{
	gotConnData = false;
	cacheFile = 0;
}

bool XMLCacheHandler::loadCacheFromDisk()
{
	if(cacheFile != 0)
	{
		delete cacheFile;
	}

	QFile file("xmlcache.xml");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	cacheFile = new QDomDocument("cache");

	bool succ = cacheFile->setContent(file.readAll());

	file.close();

	return succ;
	
}

void XMLCacheHandler::hardResetCache()
{
	if(gotConnData)
	{
		QFile file("xmlcache.xml");
		file.open(QIODevice::ReadWrite | QIODevice::Text);
		file.remove();

		if(cacheFile != 0)
		{
			delete cacheFile;
			cacheFile = 0;
		}

		RetrieveIndex* ri = new RetrieveIndex(conndata,this->parent());
		connect(ri,SIGNAL(gedditWhileItsHot(QDomDocument*)),this, SLOT(saveNewCache(QDomDocument*)));
		ri->retrieve();

	}
	else
	{
		printf("got not connection data");
	}
}


void XMLCacheHandler::saveNewCache(QDomDocument* _responsexml)
{

    cacheFile = new QDomDocument("cache");
    cacheFile->setContent(_responsexml->toString(3).toUtf8());

	saveCacheToDisk();

	emit cacheReset();
	printf("saved new cache\n");
}

void XMLCacheHandler::getArtist(QString _name)
{
	currentlySearching = _name;
	found = false;
	QDomElement artist = findArtist(_name);	
	QString id = artist.attribute("id","null");
	if(id.compare("null") != 0)
	{
		RetrieveDirectory* rd = new RetrieveDirectory(conndata,id,this);
		connect(rd,SIGNAL(gedditWhileItsHot(QDomDocument*)),this,SLOT(recievedDirectory(QDomDocument*)));
		rd->retrieve();
	}
		
}

QDomElement XMLCacheHandler::findArtist(QString _name)
{
	if(cacheFile == 0)
	{
		loadCacheFromDisk();
	}
	if(cacheFile != 0)
	{
		QDomElement index = getFirstChildByAttributeValue(cacheFile->
			elementsByTagName("indexes").at(0).toElement(),"name", QString(_name.toLocal8Bit().at(0)));

		QDomElement artist = getFirstChildByAttributeValue(index, "name", _name);
		QString id = artist.attribute("id","null");
		return artist;
	}
	else
	{
		printf("I've lost the cache in memory...\n");
		return QDomElement();
	}
}

QDomElement XMLCacheHandler::getFirstChildByAttributeValue(QDomElement _toSearch,
	QString _attrib, QString _value)
{
	for(int ctr = 0; ctr < _toSearch.childNodes().count(); ++ctr)
	{
		QDomNode qdm = _toSearch.childNodes().at(ctr);
		if(qdm.isElement())
		{
			if(qdm.toElement().attribute(_attrib,"-").compare(_value) == 0)
			{
				return qdm.toElement();
			}
		}
	}
	return QDomElement();
}

void XMLCacheHandler::recievedDirectory(QDomDocument* _respXML)
{		
	QDomNodeList qnl = _respXML->elementsByTagName("child");

	QDomElement parent = findArtist(currentlySearching);

	while(qnl.count() != 0)
	{
		puts(qnl.at(0).toElement().attribute("title").toLocal8Bit());
		qnl.at(0).toElement().setTagName("folder");
		qnl.at(0).toElement().removeAttribute("parent");
		parent.appendChild(qnl.at(0));
	}

	delete _respXML;

	puts(cacheFile->toString().toLocal8Bit());

	found = true;

	saveCacheToDisk();
}

bool XMLCacheHandler::saveCacheToDisk()
{
	if(cacheFile != 0)
	{
		QFile file("xmlcache.xml");
		file.open(QIODevice::WriteOnly | QIODevice::Text);

		//TODO -- this should use a text stream writer to save
		file.write(cacheFile->toString(3).toUtf8());
		file.flush();
		file.close();
	}
	return true;
}

void XMLCacheHandler::checkCacheAge()
{
}

XMLCacheHandler::~XMLCacheHandler(){

}
