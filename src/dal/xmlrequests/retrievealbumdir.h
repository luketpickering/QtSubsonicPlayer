#ifndef RETRIEVEALBUMDIR_H
#define RETRIEVEALBUMDIR_H

#include <QObject>
#include "subrequestxml.h"
#include "../connectiondata.h"

// Forward Class Definitions
class QDomDocument;

/*
  Class to retrieve a directory contents XML from the server
*/

class RetrieveAlbumDir : public SubRequestXML
{
        Q_OBJECT
public:
    RetrieveAlbumDir(ConnectionData* _conndata, 
		QString _dirID,QString _artist,QString _album, QObject* parent);
	QString serialiseRequest();

signals:
	void gotAlbumDir(QDomDocument* _respXML, QString _artist, 
		QString _album);

protected:
    //implementing the base class' pure virtual function
    void specificXMLHandler(QDomDocument* _respXML);

	QString artist;
	QString album;
};


#endif
