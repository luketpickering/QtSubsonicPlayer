#include <QFile>
#include <QStringList>
#include <QString>
#include <QBuffer>
#include <iostream>
#include <QtXml/qdom.h>
#include "xmlcachehandler.h"


// BEGIN: Constructors ********************************************************

XMLCacheHandler::XMLCacheHandler(QObject* parent) : QObject(parent)
{
    cacheFile = 0;
}

// END: Constructors **********************************************************

// BEGIN: Public Functions ****************************************************

// -----  BEGIN: Cache Requests

QStringList* XMLCacheHandler::getCachedIndex()
{
	if(cacheFile != 0 && !cacheFile->firstChildElement()
		.firstChildElement().isNull())
	{
		return parseCacheIndex();
	}
	else 
		return 0;
}
QStringList* XMLCacheHandler::getCachedArtist(QString _artistName,
	QString* _id)
{
	QDomElement artist = findArtist(_artistName);

	if(artist.elementsByTagName("album").count() > 0)
	{
		std::cout << "got " << qPrintable(_artistName) 
			<< "'s albums from cache\n" << std::endl;
		return parseCacheArtistDir(artist);
	}
	else 
	{
		*_id = artist.attribute("id","null");
		return 0;
	}
}
QStringList* XMLCacheHandler::getCachedAlbum(QString _artistName, 
	QString _albumName, QString* _id)
{
	QDomElement artist = findArtist(_artistName);
	QDomElement album = getFirstChildByAttributeValue(artist,"title",_albumName);
	
	if(album.elementsByTagName("track").count() > 0)
	{
		std::cout << "got " << qPrintable(_albumName) 
			<< "'s track listing from cache\n" << std::endl;
		return parseCacheAlbumDir(album);
	}
	else
	{
		*_id = album.attribute("id","null");
		return 0;
	}

}

// ----- END: Cache Requests

// ----- BEGIN: Save to Cache

bool XMLCacheHandler::hardResetCache(QDomDocument* _index)
{
	if(cacheFile != 0)
	{
		delete cacheFile;
	}
	cacheFile = _index;
	return saveCacheToDisk();
}
bool XMLCacheHandler::saveArtist(QDomDocument* _artist,
	QString _artistName)
{
	QDomNodeList qnl = _artist->elementsByTagName("child");
	QDomElement artist = findArtist(_artistName);

	if(!artist.isNull())
	{
		while(!qnl.isEmpty())
		{
			qnl.at(0).toElement().setTagName("album");
            qnl.at(0).toElement().removeAttribute("parent");
			qnl.at(0).toElement().removeAttribute("artist");
            qnl.at(0).toElement().removeAttribute("coverArt");
            artist.appendChild(qnl.at(0));
		}
		delete _artist;
		return saveCacheToDisk();
	}
	delete _artist;
	return false;
}
bool XMLCacheHandler::saveAlbum(QDomDocument* _album, QString _artistName,
	QString _albumName)
{
	QDomNodeList qnl = _album->elementsByTagName("child");
	QDomElement artist = findArtist(_artistName);
	QDomElement album = getFirstChildByAttributeValue(artist,
		"title",_albumName);

	if(!artist.isNull())
	{
		while(!qnl.isEmpty())
		{
			qnl.at(0).toElement().setTagName("track");
            qnl.at(0).toElement().removeAttribute("parent");
            qnl.at(0).toElement().removeAttribute("path");
            qnl.at(0).toElement().removeAttribute("coverArt");
            album.appendChild(qnl.at(0));
		}
		delete _album;
		return saveCacheToDisk();
	}
	delete _album;
	return false;
}

// ----- END: Save to Cache

// END: Public Functions ******************************************************


// BEGIN: Private Functions ***************************************************

/*
  IO
*/
bool XMLCacheHandler::loadCacheFromDisk()
{
    //if the cachefile isnt empty
    if(cacheFile != 0)
        delete cacheFile;

    //open the cache in readonly
    QFile file("xmlcache.xml");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    cacheFile = new QDomDocument("cache");

    //try and parse the file
    bool succ = cacheFile->setContent(file.readAll());

    file.close();

    //if the parsing failed delete the cache
    if(!succ)
    {
        delete cacheFile;
        cacheFile = 0;
        emit requireHardReset();
    }

    return succ;

}
bool XMLCacheHandler::saveCacheToDisk()
{
    if(cacheFile != 0)
    {
        QFile file("xmlcache.xml");
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

        //TODO -- this should use a text stream writer to save
        file.write(cacheFile->toString(3).toUtf8());
        file.flush();
        file.close();
        printf("saved new cache\n");
        return true;
    }
    return false;

}

/*
  Cache Queries
*/
QDomElement XMLCacheHandler::findArtist(QString _name)
{

    //probably needlessly check cache
    if(cacheFile == 0)
    {
        loadCacheFromDisk();
    }
    if(cacheFile != 0)
    {
        //don't search all artist elements in cache just the index
        //corresponding to the requested name
        QString firstLetter = QString(_name.toLocal8Bit().at(0));

        //stupid special case for 'The'
        if(firstLetter.compare("T") == 0
           && QString(_name.toLocal8Bit().at(1)).compare("h") == 0
           && QString(_name.toLocal8Bit().at(2)).compare("e") == 0
           && QString(_name.toLocal8Bit().at(3)).compare(" ") == 0)
        {
            firstLetter = QString(_name.toLocal8Bit().at(4));
        }

		if(firstLetter.compare("X") == 0 || firstLetter.compare("Y") == 0 || firstLetter.compare("Z") == 0)
			firstLetter = QString("X-Z"); 

        //allows for artists starting with a number to be obtained
        bool isNumber = false;
        firstLetter.toInt(&isNumber);
        firstLetter = isNumber ? QString("#") : firstLetter;

        //get the child of the index element
        QDomElement index = getFirstChildByAttributeValue(cacheFile->
                            elementsByTagName("indexes").at(0).toElement(),"name", firstLetter);

        if(!index.isNull())
        {
            //return the artist with matching name
            QDomElement artist = getFirstChildByAttributeValue(index, "name", _name);
            return artist;
        }
        else
            return QDomElement();
    }
    else
		return QDomElement();
}
QStringList* XMLCacheHandler::parseCacheIndex()
{
	return getValuesList(cacheFile->firstChildElement()
		.firstChildElement(),"artist", "name");
}
QStringList* XMLCacheHandler::parseCacheArtistDir(QDomElement _artistEl)
{
	return getValuesList(_artistEl, "album","title");
}
QStringList* XMLCacheHandler::parseCacheAlbumDir(QDomElement _albumEl)
{
	return getValuesList(_albumEl, "track","title");
}

/*
  DOM helper functions
*/
QDomElement XMLCacheHandler::getFirstChildByAttributeValue(QDomElement _toSearch,
                                                           QString _attrib, QString _value)
{
    for(int ctr = 0; ctr < _toSearch.childNodes().count(); ++ctr)
    {
        //iterate through child nodes
        QDomNode qdm = _toSearch.childNodes().at(ctr);
        //check its an element
        if(qdm.isElement())
        {
            //if element attribute value matches return
            if(qdm.toElement().attribute(_attrib,"-").compare(_value) == 0)
            {
                return qdm.toElement();
            }
        }
    }
    return QDomElement();
}
QStringList *XMLCacheHandler::getValuesList(QDomElement element,
                                            QString tagName,
                                            QString attributeName)
{
    QStringList *valuesList = new QStringList;

    QDomNodeList nodeList = element.elementsByTagName(tagName);
    QDomNode node;
    QDomElement elem;class QDomElement;

    for(unsigned int i = 0; i < nodeList.length(); ++i)
    {
        node = nodeList.at(i);
        elem = node.toElement();
        if (!elem.isNull())
        {
            valuesList->append(elem.attribute(attributeName));
        }
    }

    return valuesList;
}

/*
  Destructor
*/
XMLCacheHandler::~XMLCacheHandler()
{
    if(cacheFile != 0)
    {
        delete cacheFile;
    }

}

// END: Private Functions *****************************************************
