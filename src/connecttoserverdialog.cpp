#include "connecttoserverdialog.h"

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
    serverpath = pathLineEdit->text();
    username = usernameLineEdit->text();
    password = passwordLineEdit->text();

    emit serverDataSet(serverpath, username, password);
}
