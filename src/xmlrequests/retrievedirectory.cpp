#include "xmlrequests/retrievedirectory.h"

RetrieveDirectory::RetrieveDirectory(QString* _host, int* _port, QString* _usr, QString* _pss, const QString& _dirID) : SubRequestXML (_host, _port, _usr, _pss)
{
	endpointLocation = "rest/getMusicDirectory.view";
	params.insert("v","1.5.0");
	params.insert("c","lukesapp");
        params.insert("id",_dirID);
}

void RetrieveDirectory::retrieve()
{
	makeXMLReq();
        connect(this,SIGNAL(healthyRespRecieved()), this, SIGNAL(gedditWhileItsHot()));
}
