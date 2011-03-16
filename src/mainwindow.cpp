#include <QtGui>
#include <QString>

#include "mainwindow.h"
#include "connecttoserverdialog.h"
#include "mediaplayer.h"
#include "xmlrequests/xmlcachehandler.h"
#include "xmlrequests/pingtest.h"

// constructor for MainWindow
MainWindow::MainWindow()
{
    setupUi(this);
    mediaPlayer = new MediaPlayer(this);

    setMenuActions();
    setMediaActions();
}



// Menu Related Methods and Slots  ********************************************

/*
  setMenuActions() is a function which sets up the menu actions for the main
  menubar of MainWindow.
*/
void MainWindow::setMenuActions()
{
    // set up "About Qt" action
    connect(actionAboutQt, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));

    // set up "Connect To Server..." action
    connect(actionConnectToServer, SIGNAL(triggered()),
            this, SLOT(connectToServer()));

    // set up "Quit" action
    connect(actionQuit, SIGNAL(triggered()),
            this, SLOT(close()));

    // set up "About Subsonic Player" action
    connect(actionAbout, SIGNAL(triggered()),
            this, SLOT(about()));
}


/*
  about() is a private slot which shows the "About Subsonic Player" QMessageBox
*/
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Subsonic Player"),
     tr("<h2>  Subsonic Player 0.1 </h2>"
        "<p> Graphical front-end for the subsonic music server. </p>"
        "<p> By <a href=http://mnielsen.org.uk>Michael Nielsen</a> "
        "and <a href=http://ursaminorbeta.org.uk>Luke Pickering</a>. </p>"));
}

// END: Menu Related Methods and Slots ****************************************







// Media Related Methods and Slots ********************************************

/*
  setMediaActions() is a function that sets up the Media buttons and scroll
  bar and connects them with the mediaObject.
*/
void MainWindow::setMediaActions()
{
    // set up Play/Pause/Stop buttons
    connect(playButton, SIGNAL(clicked()), mediaPlayer, SLOT(playClicked()));
    connect(pauseButton, SIGNAL(clicked()), mediaPlayer, SLOT(pauseClicked()));
    connect(stopButton, SIGNAL(clicked()), mediaPlayer, SLOT(stopClicked()));

    // set up time labels
    connect(mediaPlayer, SIGNAL(tock(qint64)),
            this, SLOT(setTimeElapsedLabel(qint64)));
}

/*
  setTimeElapsedLabel(int) is a slot which sets the timeElapsedLabel text
  independently of the timeLeftLabel.
*/
void MainWindow::setTimeElapsedLabel(qint64 _time)
{
    int time = _time/mediaPlayer->tickInterval;
    QString timeElapsed;

    timeElapsed.append(QString::number(time/60));
    timeElapsed.append(":");

    if (time%60 < 10) {
        timeElapsed.append("0");
    }

    timeElapsed.append(QString::number(time%60));

    timeElapsedLabel->setText(timeElapsed);
}

// END: Media Related Methods and Slots ***************************************






// ConnectToServerDialog Related Methods and Sots *****************************

/*
  private slot shows the "Connect To Server" dialog
*/
void MainWindow::connectToServer()
{
    ConnectToServerDialog *connectToServerDialog =
            new ConnectToServerDialog(this);

    // connect the signal from connectToServerDialog to the setServerData slot
    connect(connectToServerDialog,
            SIGNAL(serverDataSet(QString&, QString&, QString&)),
            this, SLOT(setServerData(QString&,QString&,QString&)));

    connectToServerDialog->exec();
}


/*
  slot: setData(QString x3) sets the values of public members
  server, username and password from the values input by the user
  in the ConnectToServerDialog
*/
void MainWindow::setServerData(QString &_srvr,
                               QString &_uname,
                               QString &_passwd)
{
    serverpath = _srvr;
    username = _uname;
    password = _passwd;
}

// END: ConnectToServerDialog Related Methods and Slots ***********************
