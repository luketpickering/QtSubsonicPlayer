#ifndef CONNECTTOSERVERDIALOG_H
#define CONNECTTOSERVERDIALOG_H

#include <QDialog>
#include "ui_connecttoserverdialog.h"

class ConnectToServerDialog
    : public QDialog,
      public Ui::ConnectToServerDialog
{
    Q_OBJECT

public:
    QString host;
    QString usr;
    QString pss;
    int port;

    ConnectToServerDialog(QWidget *parent = 0);

private slots:
    void serverData();

signals:
    void serverDetailsEntered(QString&,QString&,QString&,int&);
};

#endif // CONNECTTOSERVERDIALOG_H
