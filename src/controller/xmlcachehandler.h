#ifndef XMLCACHEHANDLER_H
#define XMLCACHEHANDLER_H
#include <QtXml/QDomDocument>
#include <QObject>
#include "../dal/connectiondata.h"
#include "../dal/xmlrequests/retrievedirectory.h"

/*
	Class to handle the XML cache. A fresh cache will start
	will just the folder index, as requests for data to
	from the server is made responses are cached so less round trips
	for future similar requests. Cache invalidation will be handled
	by either a date stamp or a request made against cached information
	which returns an error
*/

//TODO -- a funking ridonculous amount of error mitigation

class XMLCacheHandler: public QObject
{
    Q_OBJECT

public:
	//----- Constructors
    XMLCacheHandler(ConnectionData* _cd, QObject* parent);
	XMLCacheHandler::XMLCacheHandler(QObject* parent);
	//----- Destructor
    ~XMLCacheHandler();

	//----- Public request functions
	void requestArtistList();
	void requestArtistAlbums(QString _artistName);
	void requestAlbum(QString _artistName, QString _albumName);

	//----- Public clean
	void hardResetCache();

signals:

	/*
		Signals for external slots
	*/
	
	void takeThisIndexOffMeItsCrampingMyStyle(QDomElement _requestedElement);
	void takeThisArtistDirectoryAwayItsJustGettingInTheWay(QDomElement _requestedElement);
	void takeThisAlbumWhileStocksLast(QDomElement _requestedElement);
	void noConnectionData();
	void requireHardReset();

	/*
		Internal comms
	*/
    void cacheReady();
	void readyToSaveNewDir(RetrieveDirectory* _rd, QDomNode* _nodeToAdd);


private slots:
	void saveNewCache(QDomDocument* _responsexml);
	void recievedArtistsDir(QDomDocument*);
	void recievedAlbum(QDomDocument*);
	void returnArtistElement();

private:
	//----- Members
	//cache in memory
    QDomDocument* cacheFile;

	//connection data
	bool gotConnData;
	ConnectionData* conndata;

	//----- Functions
	//--IO
	void loadCache();
	bool loadCacheFromDisk();
	bool saveCacheToDisk();

	//--Constructor
    XMLCacheHandler();

	//--Cache querys
	QDomElement findArtist(QString _name);

	//--DOM Helper
	QDomElement getFirstChildByAttributeValue(QDomElement _toSearch,
		QString _attrib, QString _value);

};

#endif // XMLCACHEHANDLER_H
