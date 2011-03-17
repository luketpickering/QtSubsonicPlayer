#include <QtXml/qdom.h>

#include "pingtest.h"


PingTest::PingTest(ConnectionData* _conndata, QObject* parent)
    : SubRequestXML (_conndata,parent)
{
    endpointLocation = "rest/ping.view";
	params.append(QPair<QString,QString>("v","1.5.0"));
    params.append(QPair<QString,QString>("c","lukesapp"));
}

void PingTest::test()
{
    makeXMLReq();
    connect(this, SIGNAL(healthyRespRecieved(QDomDocument*)),
            this, SLOT(checkStatus(QDomDocument*)));
}

void PingTest::checkStatus(QDomDocument* _respXML)
{
    if(_respXML->namedItem("subsonic-response").toElement()
        .attribute("status", "NULL").toLocal8Bit() == "ok") {
        emit serverPingOk();
    }
    else
        emit serverPingServerError();
}
