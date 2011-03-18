#include "retrievedirectory.h"

/*
	Constructors
*/

RetrieveDirectory::RetrieveDirectory(ConnectionData* _conndata, QString _dirID, QObject* parent) 
	: SubRequestXML (_conndata,parent)
{
	endpointLocation = "rest/getMusicDirectory.view";
	params.append(QPair<QString,QString>("v","1.5.0"));
	params.append(QPair<QString,QString>("c","lukesapp"));
    params.append(QPair<QString,QString>("id",_dirID));
}

/*
	Public Methods
*/

void RetrieveDirectory::retrieve()
{
	makeRequest();
}

/*
	Inherited Virtual Functions
*/

void RetrieveDirectory::specificXMLHandler(QDomDocument* _respXML)
{
	emit gedditWhileItsHot(_respXML);
}
