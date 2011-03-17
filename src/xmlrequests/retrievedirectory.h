#ifndef RETRIEVEDIRECTORY_H
#define RETRIEVEDIRECTORY_H

#include <QObject>
#include "subrequestxml.h"
#include "connectiondata.h"


class RetrieveDirectory : public SubRequestXML
{
	Q_OBJECT
public:
    RetrieveDirectory(ConnectionData* _conndata, QString _dirID, QObject* parent);
	void retrieve();

signals:

private slots:

private:
	RetrieveDirectory();

};


#endif
