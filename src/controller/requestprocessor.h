#include <QString>
#include <QObject>
#include <QList>
#include <QStringList>
#include "xmlcachehandler.h"
#include "../dal/connectiondata.h"
#include "../dal/subrequest.h"


class RequestProcessor : public QObject
{
	Q_OBJECT
public:

    //TODO_____________________________________________________________

    //have a function to check if serverside directory has been changed
    //since caches began

	//need to rework the subrequest class and have signal piping through
	//the requestprocessor
    //need to be able to check the response for a bad directory response
    //_________________________________________________________________

	RequestProcessor(ConnectionData* _cd, QObject* parent);
	RequestProcessor(QObject* parent);

	void hardResetCache();
	void getIndex();
	void getArtist(QString _artistName);
	void getAlbum(QString _artistName, QString _albumName);

	void getTrack(QString _artistName, QString _albumName,
		QString _track);
	void requestPing();

	void clearQueue();

signals:
	void retrievedIndex(QStringList* _index);
	void retrievedArtistListing(QStringList* _art,
		QString _artistName);

   void retrievedAlbumListing(QList< QPair<QString,QString> >* _alb,
            QString _artistName, QString _albumName);

        /*
	void retrievedAlbumListing(QStringList* _alb, 
		QString _artistName, QString _albumName);

	void retrievedOpenTrackStream(QBuffer* _trackstream,
		QString _artistName, QString _albumName, QString _track);
		*/

	void retrievedTrackData(QString _id, int _seconds);

	void noConnectionData();

	void pingFailed(int ec);
	void pingSucceded();

	void cacheRequiresReset();
	void cacheIsResetting();
	void cacheReset();
    void cacheResetFailed();

private slots:

	void responseIndex(QDomDocument* _respXML);
	void responseArtist(QDomDocument* _respXML, QString _artistName);
	void responseAlbum(QDomDocument* _respXML, QString _artistName, 
		QString _albumName);
	/*
	void responseTrackStream(QBuffer* _buf, QString _artistName, 
		QString _albumName, QString _track);
		*/
	void responsePing();

private:
	QList<SubRequest*> reqList;
	XMLCacheHandler xch;
	ConnectionData* connData;
	bool requestRunning;
	bool cacheResetting;

	ushort consecutiveReqs;

	void runRequest();
	void addToQueue(SubRequest* _toAdd);

	bool checkAlreadyQueued(SubRequest* _toCheck);

};
