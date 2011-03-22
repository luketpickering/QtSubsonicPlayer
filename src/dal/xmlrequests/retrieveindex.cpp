#include "retrieveindex.h"
#include "subrequestxml.h"
#include <QObject>
#include <QString>
#include <QPair>
#include "../connectiondata.h"

// Constructor
RetrieveIndex::RetrieveIndex(ConnectionData* _conndata, QObject* parent) 
	: SubRequestXML (_conndata,parent)
{
    endpointLocation = "rest/getIndexes.view";
    params.append(QPair<QString,QString>("v","1.5.0"));
    params.append(QPair<QString,QString>("c","QtSubsonicPlayer"));
}

QString RetrieveIndex::serialiseRequest()
{
	return QString("IndexRequest");
}

/*
  Inherited Virtual Functions
*/

void RetrieveIndex::specificXMLHandler(QDomDocument* _respXML)
{
	emit gotIndex(_respXML);
}

// END: Public Methods ********************************************************
