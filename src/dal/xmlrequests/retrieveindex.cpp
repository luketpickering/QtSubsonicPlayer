#include "retrieveindex.h"
#include "connectiondata.h"

RetrieveIndex::RetrieveIndex(ConnectionData* _conndata, QObject* parent) 
	: SubRequestXML (_conndata,parent)
{
    endpointLocation = "rest/getIndexes.view";
    params.append(QPair<QString,QString>("v","1.5.0"));
    params.append(QPair<QString,QString>("c","QtSubsonicPlayer"));
}

void RetrieveIndex::retrieve()
{
    makeXMLReq();
    connect(this,SIGNAL(healthyRespRecieved(QDomDocument*)),
            this, SIGNAL(gedditWhileItsHot(QDomDocument*)));
};
