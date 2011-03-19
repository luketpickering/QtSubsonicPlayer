#include "retrievetrackstream.h"
#include "qbuffer.h"


RetrieveTrackStream::RetrieveTrackStream(ConnectionData* _conndata, QString _trackID, QObject* parent)
	:SubRequestBin(_conndata,parent)
{
	endpointLocation = "rest/stream.view";
	params.append(QPair<QString,QString>("v","1.5.0"));
	params.append(QPair<QString,QString>("c","lukesapp"));
    params.append(QPair<QString,QString>("id",_trackID));
}

void RetrieveTrackStream::retrieve()
{
	makeRequest();
}

void RetrieveTrackStream::specificBinHandler(QBuffer* _qbuf){}