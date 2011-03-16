#include "xmlcachehandler.h"
#include <QFile>
#include "xmlrequests/retrieveindex.h"

XMLCacheHandler::XMLCacheHandler(QObject* parent) : QObject(parent)
{
    QFile file("xmlcache.xml");
    file.open(QIODevice::ReadWrite);

    cacheFile = new QDomDocument("cache");
    if(!cacheFile->setContent(file.readAll()))
    {
        file.remove();
        delete cacheFile;
        getNewIndexes();
    }
    else
        file.close();
}

void XMLCacheHandler::getNewIndexes()
{
    QString host_("hotblack.subsonic.org");
    QString usr_("49MR");
    QString pss_("49");
    int port = 0;
    RetrieveIndex* ri = new RetrieveIndex(&host_,&port,&usr_,&pss_);
    connect(ri,SIGNAL(gedditWhileItsHot()),this, SLOT(saveNewCache()));
    ri->retrieve();
}

void XMLCacheHandler::saveNewCache()
{

    QFile file("xmlcache.xml");
    file.open(QIODevice::WriteOnly);
    file.write(((RetrieveIndex*) sender())->respXML->toString().toLocal8Bit());
    file.flush();
    file.close();
    delete sender();
    file.open(QIODevice::ReadOnly);
    cacheFile = new QDomDocument("cache");
    cacheFile->setContent(file.readAll());
}


void XMLCacheHandler::checkCacheAge()
{
}

XMLCacheHandler::~XMLCacheHandler(){

}
