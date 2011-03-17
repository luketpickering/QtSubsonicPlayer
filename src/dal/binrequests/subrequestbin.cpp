#include "subrequestbin.h"
#include <qbuffer.h>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <qobject.h>



SubRequestBin::SubRequestBin(ConnectionData* _conndata,QObject* parent ) 
	: QObject(parent)
{

}