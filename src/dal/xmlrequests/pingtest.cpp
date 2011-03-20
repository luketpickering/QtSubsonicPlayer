#include <QtXml/qdom.h>

#include "pingtest.h"

// Constructor
PingTest::PingTest(ConnectionData* _conndata, QObject* parent)
    : SubRequestXML (_conndata,parent)
{
    endpointLocation = "rest/ping.view";
	params.append(QPair<QString,QString>("v","1.5.0"));
    params.append(QPair<QString,QString>("c","lukesapp"));
}


// BEGIN: Public Methods ******************************************************

void PingTest::test()
{
    makeRequest();
}

/*
  Inherited virtual functions
*/

void PingTest::specificXMLHandler(QDomDocument* _respXML)
{
    //check the response xml says the server is okay
    if(_respXML->namedItem("subsonic-response").toElement()
        .attribute("status", "NULL").toLocal8Bit() == "ok") 
	{
        emit serverPingOk();
        printf("server respondes: A - OKAY!\n");
    }
    //otherwise emit why not
    //TODO -- emit the error string from the response aussi
    else
        emit serverPingServerError(_respXML->namedItem("subsonic-response").toElement()
                                   .attribute("code", "0").toInt());
}

// END: Public Methods ********************************************************
