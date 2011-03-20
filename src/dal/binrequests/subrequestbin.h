#ifndef SUBREQUESTBIN_H
#define SUBREQUESTBIN_H

#include <QObject>

#include "../connectiondata.h"
#include "../subrequest.h"

// Forwards Class Definitions
class QBuffer;

/*
  Base class for all requests that return binary data
*/

class SubRequestBin : public SubRequest
{
	Q_OBJECT

signals:
	void gedditWhileItsHot(QBuffer* _buf, qint64 _loaded, qint64 _tot);
	void finishedBuffering(qint64 _total);

protected slots:
	void writeToBuffer();
	void checkProgress(qint64 _cur, qint64 _tot);
	void finishedDownloading();
protected:
	//------Functions
	//Constructor:
	SubRequestBin(ConnectionData* _cd, QObject* parent);

	void specificHandler();

private:
	//------Members
	QBuffer* buf;

};

#endif
