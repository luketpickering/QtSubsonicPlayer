#ifndef SUBREQUESTXML_H
#define SUBREQUESTXML_H

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QObject>
#include <QUrl>
#include <QtXml/qdom.h>
#include "connectiondata.h"


class SubRequestXML: public QObject
{
	Q_OBJECT


signals:
	void gedditWhileItsHot(QDomDocument* _responsexml);
	void healthyRespRecieved(QDomDocument* _responsexml);
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
	//Constructor
	SubRequestXML(ConnectionData* _conndata, QObject* parent);

	//Instance Functions
	void makeXMLReq();
	QUrl getUrl();
	bool isHttpRedirect();
	int isHealthySubResp();

private:
	//------Members
	QDomDocument* respXML;

	//------Functions
	//Constructor
	SubRequestXML();

};

#endif
