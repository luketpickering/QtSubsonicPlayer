#ifndef RETRIEVEINDEX_H
#define RETRIEVEINDEX_H

#include <QObject>
#include "xmlrequests/subrequestxml.h"


class RetrieveIndex
    : public SubRequestXML
{
    Q_OBJECT

public:
    RetrieveIndex(QString* _host, int* _port, QString* _usr, QString* _pss);
    void retrieve();

signals:

private slots:

private:
    RetrieveIndex();
};


#endif
