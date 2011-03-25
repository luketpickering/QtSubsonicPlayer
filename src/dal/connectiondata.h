#ifndef CONNECTIONDATA_H
#define CONNECTIONDATA_H

// Forward Class Defintions
class QString;

/*
  Holds the connection data for all server requests
*/

//make modifications go via methods

class ConnectionData
{
public:
	ConnectionData();
        ConnectionData(QString _host,
                       QString _usr,
                       QString _pss,
                       int _port);

        void changeDetails(QString _host,
                      QString _usr,
                      QString _pss,
                      int _port);

	QString host;
	QString usr;
	QString pss;
	bool gotServerIP;
	int port;

};

#endif
