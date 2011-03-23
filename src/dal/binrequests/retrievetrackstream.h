#ifndef RETRIEVETRACKSTREAM_H
#define RETRIEVETRACKSTREAM_H

#include "subrequestbin.h"
#include "qbuffer.h"

class RetrieveTrackStream : public SubRequestBin
{
	Q_OBJECT
public:
	RetrieveTrackStream(ConnectionData* _conndata, QString _trackID,
		QString _artistName, QString _albumName, QString _track,
		QObject* parent);
	QString serialiseRequest();

protected:
	void specificBinHandler(QBuffer* _buf);

private:
	QString artist;
	QString album;
	QString track;

signals:
	void gotTrackStream(QBuffer* _buf, 
		QString _artistName, QString _albumName, QString _track);
};

#endif
