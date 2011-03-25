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
    host = pathLineEdit->text();
    port = portLineEdit->text();
    usr = usernameLineEdit->text();
    pss = passwordLineEdit->text();

    emit serverDetailsEntered(host, usr, pss, port);
}
