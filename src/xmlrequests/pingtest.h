#ifndef PINGTEST_H
#define PINGTEST_H

#include <QObject>
#include "xmlrequests/subrequestxml.h"


class PingTest
    : public SubRequestXML
{
	Q_OBJECT
public:
	PingTest(QString* _host, int* _port, QString* _usr, QString* _pss);
	void test();

signals:
	void serverPingOk();
        void serverPingFailed();
        void serverPingServerError();

	public slots:
		void checkStatus();
private:
	PingTest();
};

#endif
