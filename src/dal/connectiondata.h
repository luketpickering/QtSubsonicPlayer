#ifndef CONNECTIONDATA_H
#define CONNECTIONDATA_H

// Forward Class Defintions
class QString;

/*
  Holds the connection data for all server requests
*/

class ConnectionData
{
public:
	ConnectionData();
	ConnectionData(QString host,
                       QString usr,
                       QString pss,
                       int port);
	QString host;
	QString usr;
	QString pss;
	bool gotServerIP;
	int port;

};

#endif
