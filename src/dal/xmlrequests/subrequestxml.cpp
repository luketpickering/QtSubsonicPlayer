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
	: SubRequest(_conndata, parent)
{
}

/*
	Destructor
*/
SubRequestXML::~SubRequestXML()
{
}

/*
	Inherited Virtual Functions
*/

/*check for subsonic error response if not hand over to 
child class*/
void SubRequestXML::specificHandler()
{
	int ec = isHealthySubResp();

	if(ec == -1)
	{
		//pass to child class for specific handling
		specificXMLHandler(respXML);
	}
	else if(ec == -2)
	{
		printf("Couldn't parse response XML\n");
	}
	else
	{
		printf("Subsonic error code: %i\n",ec);
	}
	netReply->close();
	netReply->deleteLater();
}

/*
	Protected Instance Functions
*/

/*Check that the response is not an error response*/
int SubRequestXML::isHealthySubResp()
{
    respXML = new QDomDocument("ResponseDoc");
    QString errmsg(100, '0');
    int errln, errcol;

	//try and parse the response into an XML document
    if(respXML->setContent(netReply->readAll(),false,&errmsg,&errln,&errcol)) 
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
    return -2;
}








