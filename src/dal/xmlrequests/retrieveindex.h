#ifndef RETRIEVEINDEX_H
#define RETRIEVEINDEX_H

#include <QObject>

#include "subrequestxml.h"
#include "../connectiondata.h"

/*
	Class to retrieve a folder index from server
*/

class RetrieveIndex
    : public SubRequestXML
{
    Q_OBJECT

public:
    RetrieveIndex(ConnectionData* _conndata, QObject* parent);
    void retrieve();

protected:
	//implementing the base class' pure virtual function
	void specificXMLHandler(QDomDocument* _respXML);

};


#endif
