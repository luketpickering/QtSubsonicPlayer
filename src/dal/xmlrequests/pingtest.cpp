#include <QtXml/qdom.h>
#include <iostream>
#include "pingtest.h"

// Constructor
PingTest::PingTest(ConnectionData* _conndata, QObject* parent)
    : SubRequestXML (_conndata,parent)
{
    endpointLocation = "rest/ping.view";
	params.append(QPair<QString,QString>("v","1.5.0"));
    params.append(QPair<QString,QString>("c","lukesapp"));
}

QString PingTest::serialiseRequest()
{
	return QString("PingTest-H:" + connData->host);
}


// BEGIN: Public Methods ******************************************************
/*
  Inherited virtual functions
*/

void PingTest::specificXMLHandler(QDomDocument* _respXML)
{
    //check the response xml says the server is okay
    if(_respXML->namedItem("subsonic-response").toElement()
        .attribute("status", "NULL").toLocal8Bit() == "ok") 
	{
		emit pingResponded();
        emit serverPingOk();
		std::cout << "Server responds A-Okay!\n" << std::endl;
    }
    //otherwise emit why not
    //TODO -- emit the error string from the response aussi
    else
	{
		emit pingResponded();
        emit serverPingServerError(_respXML->namedItem("subsonic-response").toElement()
                                   .attribute("code", "0").toInt());
	}
}

// END: Public Methods ********************************************************
