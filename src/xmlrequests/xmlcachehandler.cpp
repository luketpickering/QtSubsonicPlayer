#include "xmlcachehandler.h"
#include <QFile>
#include "retrieveindex.h"
#include "connectiondata.h"
#include "retrievedirectory.h"

XMLCacheHandler::XMLCacheHandler(ConnectionData* _cd, QObject* parent) : QObject(parent)
{
	conndata = _cd;
	gotConnData = true;
}

XMLCacheHandler::XMLCacheHandler(QObject* parent) : QObject(parent)
{
	gotConnData = false;
}

void XMLCacheHandler::hardResetCache()
{
	if(gotConnData)
	{
		QFile file("xmlcache.xml");
		file.open(QIODevice::ReadWrite | QIODevice::Text);
		file.remove();

		RetrieveIndex* ri = new RetrieveIndex(conndata,this->parent());
		connect(ri,SIGNAL(gedditWhileItsHot(QDomDocument*)),this, SLOT(saveNewCache(QDomDocument*)));
		ri->retrieve();

	}
}


void XMLCacheHandler::saveNewCache(QDomDocument* _responsexml)
{

    QFile file("xmlcache.xml");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

	file.write(_responsexml->toString(3).toUtf8());
    file.flush();
    file.close();

    cacheFile = new QDomDocument("cache");
    cacheFile->setContent(_responsexml->toString(3).toUtf8());


	emit cacheReset();
}

void XMLCacheHandler::getArtist(QString _name)
{
	//QString _name("Arctic Monkeys");
	QDomNodeList indexer = cacheFile->elementsByTagName(QString("index"));

	
}

void XMLCacheHandler::recievedDirectory(RetrieveDirectory* _rd, QDomNode* _nodeToAdd)
{
	//puts(_rd->respXML->toString().toUtf8());
}


void XMLCacheHandler::checkCacheAge()
{
}

XMLCacheHandler::~XMLCacheHandler(){

}
