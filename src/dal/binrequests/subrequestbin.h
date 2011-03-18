#ifndef SUBREQUESTBIN_H
#define SUBREQUESTBIN_H

#include <qbuffer.h>
#include <qobject.h>
#include "../connectiondata.h"
#include "../subrequest.h"

/*
	Base class for all requests that return binary data
*/

class SubRequestBin : public SubRequest
{
	Q_OBJECT

signals:
	void gedditWhileItsHot(QBuffer* _buf);


protected:

	//------Functions
	//Constructor:
	SubRequestBin(ConnectionData* _cd, QObject* parent);

private:
	//------Members
	QBuffer* buf;

};

#endif
