#ifndef RETRIEVEINDEX_H
#define RETRIEVEINDEX_H

#include <QObject>
#include "subrequestxml.h"
#include "connectiondata.h"



class RetrieveIndex
    : public SubRequestXML
{
    Q_OBJECT

public:
    RetrieveIndex(ConnectionData* _conndata, QObject* parent);
    void retrieve();

signals:

private slots:

private:
    RetrieveIndex();
};


#endif
