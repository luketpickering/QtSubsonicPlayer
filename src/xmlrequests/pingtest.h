#ifndef PINGTEST_H
#define PINGTEST_H

#include <QObject>
#include "subrequestxml.h"
#include "connectiondata.h"


class PingTest
    : public SubRequestXML
{
	Q_OBJECT
public:
	PingTest(ConnectionData* _conndata, QObject* parent);
	void test();

signals:
	void serverPingOk();
    void serverPingFailed();
    void serverPingServerError();

	private slots:
		void checkStatus(QDomDocument* _respXML);
private:
	PingTest();
};

#endif
