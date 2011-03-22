#ifndef RETRIEVETRACKSTREAM_H
#define RETRIEVETRACKSTREAM_H

#include "subrequestbin.h"
#include "qbuffer.h"

class RetrieveTrackStream : public SubRequestBin
{
	Q_OBJECT
public:
	RetrieveTrackStream(ConnectionData* _conndata, QString _trackID,
		QObject* parent);
	QString serialiseRequest();


protected:
	void specificBinHandler(QBuffer* _buf);

};

#endif
