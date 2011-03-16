#ifndef RETRIEVEDIRECTORY_H
#define RETRIEVEDIRECTORY_H

#include <QObject>
#include "xmlrequests/subrequestxml.h"


class RetrieveDirectory : public SubRequestXML
{
	Q_OBJECT
public:
        RetrieveDirectory(QString* _host, int* _port, QString* _usr, QString* _pss,const QString& _dirID);
	void retrieve();

signals:

private slots:

private:
	RetrieveDirectory();

};


#endif
