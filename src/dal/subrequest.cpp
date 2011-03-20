#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QDomDocument>
#include <QUrl>
#include <iostream>

#include "subrequest.h"

using std::cout;
using std::endl;

// Protected Constructor
SubRequest::SubRequest(ConnectionData* _conndata,QObject* parent )
    : QObject(parent)
{
    connData = _conndata;
    netAMan = new QNetworkAccessManager(this);
}

// BEGIN: Protected Methods ***************************************************

/*
  Send request to server and to connect slot which waits forcompleted response.
*/
void SubRequest::makeRequest()
{
    //construct and set the Basic authentication header value
    QString authDetails_ = "Basic "
            + (connData->usr + ":" + connData->pss).toLocal8Bit().toBase64();
    netReq.setRawHeader("Authorization", authDetails_.toLocal8Bit());
	
    //set the URL
    netReq.setUrl(getUrl());

    //DEBUGGING start -----
    cout << qPrintable(netReq.url().toString()) << endl;
    //DEBUGGING end -----

    //initate request and connect to private slot to handle response
    netReply = netAMan->get(netReq);

	//need to find the server first. Redirect has no body so doesn;t fire ready read

	if(!connData->gotServerIP)
	{
		connect(netReply,SIGNAL(finished()), this, SLOT(handleRawResponse()));
	}
	else
	{
		connect(netReply,SIGNAL(readyRead()), this, SLOT(handleRawResponse()));
	}
}

/*
  Construct and return URL for server dependent on connection data available.
*/
QUrl SubRequest::getUrl()
{
    QUrl rtnQUrl_ = QUrl(QString("http://" + connData->host));

    if(connData->port != -1)
        rtnQUrl_.setPort(connData->port);

    rtnQUrl_.setPath(endpointLocation);

    rtnQUrl_.setQueryItems(params);

    return rtnQUrl_;
}


// END: Protected Methods *****************************************************




// BEGIN: Protected Slots *****************************************************

/*
  checks response for network error, if redirect performs redirect on final
  response calls base class specificHandler
*/
void SubRequest::handleRawResponse()
{
    cout << "Recieved response from: " << qPrintable(netReq.url().host())
            << endl;

    if(netReply->error() == QNetworkReply::NoError ) 
    {

		//get the response code
		int httpRC = netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
				 .toInt();

		//if it s a redirect
        if(!connData->gotServerIP && httpRC == 302)  
        {
            //close the current connection
            netReply->close();

            //get the redirect URL
            QUrl redirect_
                    = netReply->
                      attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

            //extract the host and port
            connData->host = redirect_.host();
            connData->port = redirect_.port();

            //queue netReply object deletion
            netReply->deleteLater();

            emit globalHostStringChanged();

            std::cout << qPrintable(netReq.url().host())
                    << " requests redirect to: "
                    << qPrintable(redirect_.host()) << std::endl;

            //adjust the request
            netReq.setUrl(getUrl());

            //send off request to server
            netReply = netAMan->get(netReq);
            connect(netReply,SIGNAL(finished()), this, SLOT(handleRawResponse()));
			return;
        }
		else if(!connData->gotServerIP && httpRC == 200)
        {
			connData->gotServerIP = true;
        }

		if(netReply->header(QNetworkRequest::ContentTypeHeader).toString().compare("audio/mpeg") == 0)
		{
			disconnect(netReply,SIGNAL(readyRead()), this, SLOT(handleRawResponse()));
			specificHandler();
		}
		else
		{
			disconnect(netReply,SIGNAL(readyRead()), this, SLOT(handleRawResponse()));
			connect(netReply,SIGNAL(finished()), this, SLOT(handleRawResponse()));

			if(netReply->isFinished())
			{
				specificHandler();
			}
		}
		
    }
    else {
        cout << "Network Error - "
                << qPrintable(netReply->errorString()) << endl;
    }
}

// END: Protected Slots *******************************************************
