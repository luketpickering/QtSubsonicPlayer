#include <QtXml/qdom.h>

#include "pingtest.h"


PingTest::PingTest(QString* _host, int* _port, QString* _usr, QString* _pss)
    : SubRequestXML (_host, _port, _usr, _pss)
{
    endpointLocation = "rest/ping.view";
    params.insert("v","1.5.0");
    params.insert("c","lukesapp");
}

void PingTest::test()
{
    makeXMLReq();
    connect(this, SIGNAL(healthyRespRecieved()),
            this, SLOT(checkStatus()));
}

void PingTest::checkStatus()
{
    if(respXML->namedItem("subsonic-response").toElement()
        .attribute("status", "NULL").toLocal8Bit() == "ok") {
        //printf("Server responds to ping: A-Okay\n");
    }

    emit serverPingOk();
    emit gedditWhileItsHot();
}
