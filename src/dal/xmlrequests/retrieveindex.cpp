#include "dal/xmlrequests/retrieveindex.h"
#include "dal/xmlrequests/subrequestxml.h"

// Constructor
RetrieveIndex::RetrieveIndex(ConnectionData* _conndata, QObject* parent) 
	: SubRequestXML (_conndata,parent)
{
    endpointLocation = "rest/getIndexes.view";
    params.append(QPair<QString,QString>("v","1.5.0"));
    params.append(QPair<QString,QString>("c","QtSubsonicPlayer"));
}



// BEGIN: Public Methods ******************************************************

void RetrieveIndex::retrieve()
{
    makeRequest();
};

/*
  Inherited Virtual Functions
*/

void RetrieveIndex::specificXMLHandler(QDomDocument* _respXML)
{
	emit gedditWhileItsHot(_respXML);
}

// END: Public Methods ********************************************************
