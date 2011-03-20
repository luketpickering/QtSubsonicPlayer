#ifndef RETRIEVEDIRECTORY_H
#define RETRIEVEDIRECTORY_H

#include <QObject>

#include "dal/xmlrequests/subrequestxml.h"
#include "dal/connectiondata.h"

// Forward Class Definitions
class QDomDocument;

/*
  Class to retrieve a directory contents XML from the server
*/

class RetrieveDirectory : public SubRequestXML
{
        Q_OBJECT

public:
    RetrieveDirectory(ConnectionData* _conndata, QString _dirID, QObject* parent);
    void retrieve();


protected:
    //implementing the base class' pure virtual function
    void specificXMLHandler(QDomDocument* _respXML);
};


#endif
