#ifndef CONNECTIONDATA_H
#define CONNECTIONDATA_H

#include <qstring.h>

/*
	Holds the connection data for all server requests
*/

class ConnectionData
{
public:
	QString host;
	QString usr;
	QString pss;
	int port;

	ConnectionData();
	ConnectionData(QString host,
					QString usr,
					QString pss,
					int port);

};

#endif
