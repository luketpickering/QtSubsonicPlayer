#ifndef CONNECTTOSERVERDIALOG_H
#define CONNECTTOSERVERDIALOG_H

#include <QDialog>
#include "ui_connecttoserverdialog.h"

class ConnectToServerDialog : public QDialog,
                              public Ui::ConnectToServerDialog
{
    Q_OBJECT

public:
    QString serverpath;
    QString username;
    QString password;

    ConnectToServerDialog(QWidget *parent = 0);

private slots:
    void serverData();

signals:
    void serverDataSet(QString &Server, QString &Username, QString &Password);
};

#endif // CONNECTTOSERVERDIALOG_H
