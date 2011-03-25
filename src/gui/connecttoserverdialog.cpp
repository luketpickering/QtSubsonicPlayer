#include "gui/connecttoserverdialog.h"

// constructor for ConnectToServerDialog
ConnectToServerDialog::ConnectToServerDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    connect(buttonBox, SIGNAL(accepted()),
            this, SLOT(serverData()));
}


/*
  serverData() is a slot which gathers the text from the QLineEdits in the
  ConnectToServerDialog instance and emits a signal containing them which
  can be picked up by the MainWindow.
*/
void ConnectToServerDialog::serverData()
{
    if (portLineEdit->text().length() > 0)
        port = portLineEdit->text().toInt();
    else
        port = -1;

    host = pathLineEdit->text();
    host.remove("http://");

    QStringList host_port = host.split(':');
    host = host_port.at(0);
    if (host_port.length() == 2)
        port = host_port.at(1).toInt();

    usr = usernameLineEdit->text();
    pss = passwordLineEdit->text();

    emit serverDetailsEntered(host, usr, pss, port);
}
