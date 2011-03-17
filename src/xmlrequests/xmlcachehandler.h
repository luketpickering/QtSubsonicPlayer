#ifndef XMLCACHEHANDLER_H
#define XMLCACHEHANDLER_H
#include <QtXml/QDomDocument>
#include <QObject>
#include "connectiondata.h"
#include "retrievedirectory.h"

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
