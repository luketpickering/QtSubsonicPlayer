#ifndef SUBREQUEST_H
#define SUBREQUEST_H

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QObject>
#include <QUrl>
#include <QtXml/qdom.h>

class SubRequestXML : public QObject
{
    Q_OBJECT

    public:
		//Member functions
		
signals:
		void gedditWhileItsHot();
		void healthyRespRecieved();
		void globalHostStringChanged();

	private slots:
		void recievedData();
	
	protected:
		//Data Members
		QString* host;
		QString* usr; 
		QString* pss;
		int* port;

		QMap<QString, QString> params;
		QString endpointLocation;

		QNetworkRequest* netReq;
		QNetworkAccessManager* netAMan;
		QNetworkReply* netReply;


		QDomDocument* respXML;

		bool isHTTPRedirect();
		int isHealthySubResp();
		void makeXMLReq();
		SubRequestXML(QString* _host, int* _port, QString* _usr, QString* _pss);

		QUrl getUrl();

	private:
		SubRequestXML();

};

#endif
