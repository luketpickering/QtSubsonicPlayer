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

	//make it so that save is not every cache response.

	RequestProcessor(ConnectionData* _cd, QObject* parent);
	RequestProcessor(QObject* parent);

	void hardResetCache();
	void getIndex();
	void getArtist(QString _artistName);
	void getAlbum(QString _artistName, QString _albumName);

	void getTrack(QString _artistName, QString _albumName,
		QString _track);
	void requestPing();

signals:
	void retrievedIndex(QStringList* _index);
	void retrievedArtistListing(QStringList* _art,
		QString _artistName);
	void retrievedAlbumListing(QStringList* _alb, 
		QString _artistName, QString _albumName);
	/*
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

private slots:
	void hardReset();

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

	void runRequest();
	void addToQueue(SubRequest* _toAdd);

	bool checkAlreadyQueued(SubRequest* _toCheck);

};
