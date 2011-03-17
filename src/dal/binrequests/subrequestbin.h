#ifndef SUBREQUESTBIN_H
#define SUBREQUESTBIN_H

#include <Phonon/mediaobject.h>
#include <qbuffer.h>
#include <qobject.h>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "src\xmlrequests\connectiondata.h"
#include "qlist.h"

class SubRequestBin : public QObject
{
	Q_OBJECT

signals:
	void gedditWhileItsHot(QBuffer* _buf);
	void globalHostStringChanged();

private slots:
	void handleRawResponse();

protected:
	//------Members
	QList<QPair<QString, QString>> params;
	QString endpointLocation;

	//Network members
	ConnectionData* connData;
	QNetworkRequest netReq;
	QNetworkAccessManager* netAMan;
	QNetworkReply* netReply;

	//------Functions
	//Constructor:
	SubRequestBin(ConnectionData* _cd, QObject* parent);
	QBuffer* buf;

private:
	SubRequestBin();

};

#endif
