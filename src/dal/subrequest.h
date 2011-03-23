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

public:
		void makeRequest();
		virtual QString serialiseRequest();
		int bump();
		int checkBumps();

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
	QUrl getUrl();
	bool isHttpRedirect();

	//pure virtual functions
	virtual void specificHandler();
private:
	int bumps;

};
#endif
