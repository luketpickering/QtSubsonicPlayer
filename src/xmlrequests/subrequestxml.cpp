#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtXml>
#include <QUrl>
#include "xmlrequests/subrequestxml.h"

// SubRequest Methods *********************************************************

SubRequestXML::SubRequestXML(QString* _host,
                             int* _port,
                             QString* _usr,
                             QString* _pss)
{
    host = _host;
    usr = _usr;
    pss = _pss;
    port = _port;
    netReq = new QNetworkRequest();
    netAMan = new QNetworkAccessManager(this);
}

QUrl SubRequestXML::getUrl()
{
    QUrl rtnQUrl_ = QUrl(QString("http://" + *host));

    if(*port != 0) {
        rtnQUrl_.setPort(*port);
    }

    rtnQUrl_.setPath(endpointLocation);

    QMap<QString, QString>::Iterator i = params.begin();

    while(i != params.end()) {
        rtnQUrl_.addQueryItem(i.key(), i.value());
        ++i;
    }

    return rtnQUrl_;
}

void SubRequestXML::makeXMLReq()
{
    QString authDetails_ = "Basic "
                           + (*usr + ":" + *pss).toLocal8Bit().toBase64();

    netReq->setRawHeader("Authorization", authDetails_.toLocal8Bit());

    netReq->setUrl(getUrl());
    puts(netReq->url().toString().toLocal8Bit());
    netReply = netAMan->get(*netReq);
    connect(netReply,SIGNAL(finished()), this, SLOT(recievedData()));
}

void SubRequestXML::recievedData()
{
    printf("Recieved response from: " + netReq->url().host()
           .toLocal8Bit() + "\n");

    if(netReply->error() == QNetworkReply::NoError ) {
        if(!isHTTPRedirect()) {
            int eC = isHealthySubResp();
            if(eC != -1) {
                printf("API Request failed with error code:%i\n",eC);
            }
            else {
                printf("Well Formed response\n");
                puts(respXML->toString().toLocal8Bit());
                netReply->close();
                emit healthyRespRecieved();
            }
        }
    }
    else {
        //add some logging here
    }
}

bool SubRequestXML::isHTTPRedirect()
{
    int httpSC;
    httpSC = netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
             .toInt();

    if(httpSC == 302) {
        netReply->close();

        QUrl redirect_
            = netReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

        delete netReply;

        printf(netReq->url().host().toLocal8Bit()
               + " requests redirect to: " + redirect_.host().toLocal8Bit() + "\n");

        *host = redirect_.host();
        *port = redirect_.port();
        emit globalHostStringChanged();

        netReq->setUrl(getUrl());

        netReply = netAMan->get(*netReq);
        connect(netReply,SIGNAL(finished()), this, SLOT(recievedData()));

        return true;
    }
    return false;
}

int SubRequestXML::isHealthySubResp()
{
    respXML = new QDomDocument("ResponseDoc");
    QString* errmsg = new QString(100, '0');
    int errln, errcol;

    if(respXML->setContent(netReply->readAll(),false,errmsg,&errln,&errcol)) {
        QDomElement responseNode = respXML->namedItem("subsonic-response")
                                   .toElement();

        if(!responseNode.isNull()) {
            if(responseNode.attribute("status",QString("NULL")).toLocal8Bit()
                == "failed") {
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

SubRequestXML::SubRequestXML(){}

