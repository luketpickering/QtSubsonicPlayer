#include "xmlrequests/retrieveindex.h"

RetrieveIndex::RetrieveIndex(QString* _host,
                             int* _port,
                             QString* _usr,
                             QString* _pss)
    : SubRequestXML (_host, _port, _usr, _pss)
{
    endpointLocation = "rest/getIndexes.view";
    params.insert("v","1.5.0");
    params.insert("c","QtSubsonicPlayer");
}

void RetrieveIndex::retrieve()
{
    makeXMLReq();
    connect(this,SIGNAL(healthyRespRecieved()),
            this, SLOT(printResp()));
}

void RetrieveIndex::printResp()
{
    //puts(respXML->toString().toLocal8Bit());
}
