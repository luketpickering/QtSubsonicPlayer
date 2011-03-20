#include <QBuffer>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QObject>
#include <QVariant>

#include <iostream>

#include "subrequestbin.h"
#include "../subrequest.h"



SubRequestBin::SubRequestBin(ConnectionData* _conndata, QObject* parent ) 
    : SubRequest(_conndata,parent)
{
}

void SubRequestBin::specificHandler()
{
    buf = new QBuffer();
    buf->open(QIODevice::ReadWrite);
    connect(netReply, SIGNAL(readyRead()), this, SLOT(writeToBuffer()));
    connect(netReply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(checkProgress(qint64,qint64)));
    connect(netReply, SIGNAL(finished()),
            this, SLOT(finishedDownloading()));

}

void SubRequestBin::writeToBuffer()
{
    if(netReply->bytesAvailable() > 0)
    {
        buf->write(netReply->read(1024));

    }
}

void SubRequestBin::checkProgress(qint64 _cur, qint64 _tot)
{
    if(_cur > 2*1024*1024)
    {
        disconnect(netReply, SIGNAL(downloadProgress(qint64,qint64)),
                   this, SLOT(checkProgress(qint64,qint64)));
        emit gedditWhileItsHot(buf,_cur,_tot);
    }
    std::cout << "." << std::endl;
}

void SubRequestBin::finishedDownloading()
{
    netReply->close();
    netReply->deleteLater();
    emit finishedBuffering(netReply->size());
}
