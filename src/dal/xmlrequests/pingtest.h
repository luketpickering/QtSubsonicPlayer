#ifndef PINGTEST_H
#define PINGTEST_H

#include <QObject>
#include "subrequestxml.h"
#include "../connectiondata.h"

// Forward Class Definitions
class QDomDocument;

/*
  Class to perform a ping request to the server
  Check the response and emit a signal depending
  on the server response;
*/

class PingTest
    : public SubRequestXML
{
	Q_OBJECT

public:
	PingTest(ConnectionData* _conndata, QObject* parent);
	void test();

signals:
	void serverPingOk();
        void serverPingServerError(int _errorCode);

protected:
	//implementing the base class' pure virtual function
	void specificXMLHandler(QDomDocument* _respXML);
		
};

#endif
