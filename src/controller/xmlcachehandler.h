#ifndef XMLCACHEHANDLER_H
#define XMLCACHEHANDLER_H

#include <QObject>
#include <QtXml/qdom.h>
#include <QBuffer>


class XMLCacheHandler: public QObject
{
    Q_OBJECT

public:
    //----- Constructors
    XMLCacheHandler(QObject* parent);

    //----- Destructor
    ~XMLCacheHandler();

	QStringList* getCachedIndex();
	QStringList* getCachedArtist(QString _artistName, QString* _id);
	QStringList* getCachedAlbum(QString _artistName,
		QString _albumName, QString* _id);
	QString getCacheTrackID(QString _artistName,
		QString _albumName, QString _trackName);

	bool hardResetCache(QDomDocument* _index);
	bool saveArtist(QDomDocument* _artist, QString _artistName);
	bool saveAlbum(QDomDocument* _album, QString _artistName,
		QString _albumName);

	//----- Functions
    //--IO
    bool loadCacheFromDisk();
    bool saveCacheToDisk();

signals:

    // Signals for external slots
	void requireHardReset();
    void cacheReset();

private:
    //----- Members
    //cache in memory
    QDomDocument* cacheFile;

    //--Constructor
    XMLCacheHandler();

    //--Cache querys
    QDomElement findArtist(QString _name);
	QStringList* parseCacheIndex();
	QStringList* parseCacheArtistDir(QDomElement _artistEl);
	QStringList* parseCacheAlbumDir(QDomElement _albumEl);

    //--DOM Helper
    QDomElement getFirstChildByAttributeValue(QDomElement _toSearch,
                                              QString _attrib, 
											  QString _value);
	QStringList * getValuesList(QDomElement element,
                                            QString tagName,
                                            QString attributeName);
};

#endif // XMLCACHEHANDLER_H
