#ifndef RETRIEVEARTISTDIR_H
#define RETRIEVEARTISTDIR_H

#include <QObject>
#include "subrequestxml.h"
#include "../connectiondata.h"

// Forward Class Definitions
class QDomDocument;

/*
  Class to retrieve a directory contents XML from the server
*/

class RetrieveArtistDir : public SubRequestXML
{
        Q_OBJECT

public:
    RetrieveArtistDir(ConnectionData* _conndata, 
		QString _dirID,QString _artist, QObject* parent);
	QString serialiseRequest();

signals:
	void gotArtistDir(QDomDocument* _respXML, QString _artist);

protected:
    //implementing the base class' pure virtual function
    void specificXMLHandler(QDomDocument* _respXML);

	QString artist;
};


#endif
