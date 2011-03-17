#include "retrievedirectory.h"

RetrieveDirectory::RetrieveDirectory(ConnectionData* _conndata, QString _dirID, QObject* parent) 
	: SubRequestXML (_conndata,parent)
{
	endpointLocation = "rest/getMusicDirectory.view";
	params.append(QPair<QString,QString>("v","1.5.0"));
	params.append(QPair<QString,QString>("c","lukesapp"));
    params.append(QPair<QString,QString>("id",_dirID));
}

void RetrieveDirectory::retrieve()
{
	makeXMLReq();
        connect(this,SIGNAL(healthyRespRecieved(QDomDocument*)), this, SIGNAL(gedditWhileItsHot(QDomDocument*)));
}