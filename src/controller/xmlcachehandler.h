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

class XMLCacheHandler: public QObject
{
    Q_OBJECT

public:
    XMLCacheHandler(ConnectionData* _cd, QObject* parent);
	XMLCacheHandler::XMLCacheHandler(QObject* parent);
    ~XMLCacheHandler();
    

	/* To Implement

	bool cacheIntegrity();

	*/

	void hardResetCache();
	void getArtist(QString _name);
	void getDirectory(QString _id);

signals:
    void cacheReset();
	void readyToSaveNewDir(RetrieveDirectory* _rd, QDomNode* _nodeToAdd);

public slots:

private slots:
	void saveNewCache(QDomDocument* _responsexml);
	void recievedDirectory(RetrieveDirectory* _rd, QDomNode* _nodeToAdd);

private:
    QDomDocument* cacheFile;
	bool gotConnData;
    void checkCacheAge();
	ConnectionData* conndata;
    XMLCacheHandler();

};

#endif // XMLCACHEHANDLER_H
