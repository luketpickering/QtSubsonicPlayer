#ifndef SUBREQUESTXML_H
#define SUBREQUESTXML_H

#include <QObject>
#include <QtXml/qdom.h>
#include "../connectiondata.h"
#include "../subrequest.h"

/*
	Base class for all requests that return XML
*/

class SubRequestXML: public SubRequest
{
	Q_OBJECT

signals:
	void gedditWhileItsHot(QDomDocument* _responsexml);

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
