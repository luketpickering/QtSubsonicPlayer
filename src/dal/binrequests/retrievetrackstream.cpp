#include "retrievetrackstream.h"
#include "qbuffer.h"


RetrieveTrackStream::RetrieveTrackStream(ConnectionData* _conndata, 
	QString _trackID,
	QString _artistName, QString _albumName, QString _track,
	QObject* parent)
	:SubRequestBin(_conndata,parent)
{
	endpointLocation = "rest/stream.view";
	params.append(QPair<QString,QString>("v","1.5.0"));
	params.append(QPair<QString,QString>("c","lukesapp"));
    params.append(QPair<QString,QString>("id",_trackID));

	artist = _artistName;
	album = _albumName;
	track = _track;
}

QString RetrieveTrackStream::serialiseRequest()
{
	return QString("RTS-id:" + params.at(2).second);
}

void RetrieveTrackStream::specificBinHandler(QBuffer* _buf)
{
	emit gotTrackStream(_buf, artist, album, track);
}
