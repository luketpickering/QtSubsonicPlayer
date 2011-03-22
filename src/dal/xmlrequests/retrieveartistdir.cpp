#include <QtXml/QDomDocument>
#include "retrieveartistdir.h"

// Constructor
RetrieveArtistDir::RetrieveArtistDir(ConnectionData* _conndata, 
		QString _dirID,QString _artist,  QObject* parent)
		: SubRequestXML (_conndata,parent)
{
	endpointLocation = "rest/getMusicDirectory.view";
	params.append(QPair<QString,QString>("v","1.5.0"));
	params.append(QPair<QString,QString>("c","lukesapp"));
    params.append(QPair<QString,QString>("id",_dirID));

	artist = _artist;
}

QString RetrieveArtistDir::serialiseRequest()
{
	return QString("RArD-Artist:" + artist);
}

// BEGIN: Public Methods ******************************************************
/*
	Inherited Virtual Functions
*/

void RetrieveArtistDir::specificXMLHandler(QDomDocument* _respXML)
{
	emit gotArtistDir(_respXML, artist);
}

// END: Public Methods ********************************************************
