#ifndef SUBREQUESTXML_H
#define SUBREQUESTXML_H

#include <QObject>

#include "../connectiondata.h"
#include "../subrequest.h"

// Forward Class Definitions
class QDomDocument;

/*
	Base class for all requests that return XML
*/
class SubRequestXML: public SubRequest
{
	Q_OBJECT

public:
	virtual QString serialiseRequest() = 0;

protected:
	//Instance Functions
	SubRequestXML(ConnectionData* _conndata, QObject* parent);
	SubRequestXML(QObject* parent);

	int isHealthySubResp();
	void specificHandler();

	void virtual specificXMLHandler(QDomDocument* _respXML) = 0;

	~SubRequestXML();

private:
	//------Members
	QDomDocument* respXML;
};

#endif
