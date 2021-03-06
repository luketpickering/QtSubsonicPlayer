#include <QString>
#include "connectiondata.h"

// BEGIN: Constructors ********************************************************

ConnectionData::ConnectionData()
{
    port = -1;
	gotServerIP = false;
}


ConnectionData::ConnectionData(QString _host, QString _usr,
                               QString _pss, int _port)
{
    host = _host;
    usr = _usr;
    pss = _pss;
    port = _port;
    gotServerIP = false;
}

// END: Constructors **********************************************************


void ConnectionData::changeDetails(QString _host, QString _usr, QString _pss, int _port)
{
    host = _host;
    usr = _usr;
    pss = _pss;
    port = _port;
    gotServerIP = false;
}
