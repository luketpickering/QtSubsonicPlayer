#include <QtXml/QDomDocument>
#include "retrievealbumdir.h"

// Constructor
RetrieveAlbumDir::RetrieveAlbumDir(ConnectionData* _conndata, 
		QString _dirID,QString _artist,QString _album,  QObject* parent)
		: SubRequestXML (_conndata,parent)
{
	endpointLocation = "rest/getMusicDirectory.view";
	params.append(QPair<QString,QString>("v","1.5.0"));
	params.append(QPair<QString,QString>("c","lukesapp"));
    params.append(QPair<QString,QString>("id",_dirID));

	artist = _artist;
	album = _album;
}

QString RetrieveAlbumDir::serialiseRequest()
{
	return QString("RAlD-Album:"+ album + ",Artist:" + artist);
}

// BEGIN: Public Methods ******************************************************
/*
	Inherited Virtual Functions
*/

void RetrieveAlbumDir::specificXMLHandler(QDomDocument* _respXML)
{
	emit gotAlbumDir(_respXML, artist, album);
}

// END: Public Methods ********************************************************
