#ifndef SUBREQUEST_H
#define SUBREQUEST_H

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QObject>
#include <QUrl>
#include "connectiondata.h"

/*
	Abstract base class which handles all traffic to the server
	contains methods for checkign if response is a redirect and
	sending another response if it is. calls a pure virtual 
	function after final response recieved which allows
	synchronous code flow up the base/derived class tree
*/

class SubRequest : public QObject
{

	Q_OBJECT;

signals:
	void globalHostStringChanged();

private slots:
	void handleRawResponse();

protected:
	//------Members
	QList< QPair<QString, QString> > params;
	QString endpointLocation;

	//Network members
	ConnectionData* connData;
	QNetworkRequest netReq;
	QNetworkAccessManager* netAMan;
	QNetworkReply* netReply;

	//------Functions
	//Constructor
	SubRequest(ConnectionData* _conndata, QObject* parent);
	SubRequest(QObject* parent);

	//Instance Functions
	void makeRequest();
	QUrl getUrl();
	bool isHttpRedirect();

	//pure virtual functions
	void virtual specificHandler() = 0;

};
#endif
