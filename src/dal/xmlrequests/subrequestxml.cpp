#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QDomDocument>
#include <QUrl>
#include "subrequestxml.h"


/*
	Protected Constructors
*/

SubRequestXML::SubRequestXML(ConnectionData* _conndata,QObject* parent ) 
	: QObject(parent)
{
    connData = _conndata;
    netAMan = new QNetworkAccessManager(this);
}

/*
	Private Constructors
*/

SubRequestXML::SubRequestXML(){}

/*
	Protected Methods
*/

/*Send request to server and to connect slot which waits for
completed response*/
void SubRequestXML::makeXMLReq()
{
	//construct and set the Basic authentication header value
    QString authDetails_ = "Basic "
		+ (connData->usr + ":" + connData->pss).toLocal8Bit().toBase64();
    netReq.setRawHeader("Authorization", authDetails_.toLocal8Bit());
	
	//set the URL
    netReq.setUrl(getUrl());

	//DEBUGGING start -----
    puts(netReq.url().toString().toLocal8Bit());
	//DEBUGGING end -----

	//initate request and connect to private slot to handle response
    netReply = netAMan->get(netReq);
    connect(netReply,SIGNAL(finished()), this, SLOT(handleRawResponse()));
	int bla = 2;
}

/*Construct and return URL for server dependent on connection data
available*/
QUrl SubRequestXML::getUrl()
{
	QUrl rtnQUrl_ = QUrl(QString("http://" + connData->host));

	if(connData->port != -1) 
		rtnQUrl_.setPort(connData->port);

    rtnQUrl_.setPath(endpointLocation);

	rtnQUrl_.setQueryItems(params);

    return rtnQUrl_;
}
/*Check if response asks for a redirect, if it does 
update connection data and send off a new request*/
bool SubRequestXML::isHttpRedirect()
{
    int httpRC;
	bool redirectedAlready_ = true;
	QString newHost_ = QString();
	int newPort_;

	//get the response code
    httpRC = netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
             .toInt();
	//if it s a redirect
    if(httpRC == 302) {
		//close the current connection
        netReply->close();
		//get the redirect URL
        QUrl redirect_
            = netReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
		//extract the host and port
		newHost_ = redirect_.host();
		newPort_ = redirect_.port();

		//queue netReply object deletion
		netReply->deleteLater();

		//if the redirect is different from that currently
		if(connData->port != newPort_)
		{
			//set the global port
			connData->port = newPort_;
			redirectedAlready_ = false;
		}
		if(connData->host != newHost_)
		{
			//set the global host
			connData->host = newHost_;
			redirectedAlready_ = false;
		}

		emit globalHostStringChanged();
		printf(netReq.url().host().toLocal8Bit()
            + " requests redirect to: " + redirect_.host().toLocal8Bit() + "\n");

		//adjust the request
        netReq.setUrl(getUrl());

		//send off request to server
        netReply = netAMan->get(netReq);
        connect(netReply,SIGNAL(finished()), this, SLOT(handleRawResponse()));

        return true;
    }
    return false;
}
/*Check that the response is not an error response
*/
int SubRequestXML::isHealthySubResp()
{
    respXML = new QDomDocument("ResponseDoc");
    QString* errmsg = new QString(100, '0');
    int errln, errcol;

	//try and parse the response into an XML document
    if(respXML->setContent(netReply->readAll(),false,errmsg,&errln,&errcol)) 
	{
		//get the root element
        QDomElement responseNode = respXML->namedItem("subsonic-response")
                                   .toElement();
		//if the element is not null
        if(!responseNode.isNull()) 
		{
			//check the response status, if failed or null try and parse the 
			//response error code, if its ok return ok.
            if(responseNode.attribute("status","failed").toLocal8Bit() == "failed") 
			{
                bool parsedEC;
                int eC = 0;

                eC = responseNode.childNodes().item(0).toElement()
                     .attribute("code", "0").toInt(&parsedEC,10);

                if (parsedEC) { return eC; }
                else { return 0; }

            }
            return -1;
        }
    }
    return 0;
}

/*
	Private Methods
*/

/*
	Protected Slots
*/
/*checks response error, http redirect, check it is not an error response 
report results*/
void SubRequestXML::handleRawResponse()
{

    printf("Recieved response from: " + netReq.url().host()
           .toLocal8Bit() + "\n");

    if(netReply->error() == QNetworkReply::NoError ) {

        if(!isHttpRedirect()) {

            int eC = isHealthySubResp();

            if(eC != -1) {
                printf("API Request failed with error code:%i\n",eC);
            }
            else {
                printf("Well Formed response\n");
                
				//DEBUGGING start -----
				//puts(respXML->toString().toLocal8Bit());
                //DEBUGGING end -----

				netReply->close();
				netReply->deleteLater();

                emit healthyRespRecieved(respXML);
            }
        }
    }
    else {
        printf("Network Error - GUNNED!!!");
    }
}

/*
	Private Slots
*/







