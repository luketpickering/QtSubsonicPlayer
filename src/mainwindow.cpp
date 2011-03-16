#include <QtGui>
#include <QString>
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/AudioOutput>

#include "mainwindow.h"
#include "connecttoserverdialog.h"

// constructor for MainWindow
MainWindow::MainWindow()
{
    setupUi(this);
    this->setMenuActions();
    this->setPhonon();
    this->setMediaActions();

    // set the MediaSource (file in folder for now)
    Phonon::MediaSource mediaSource("gravity.mp3");
    mediaObject->setCurrentSource(mediaSource);
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
  setPhonon() is a function create the Phonon MediaObject and AudioOutput
  and links them together. It sets mediaObject's tickInterval and connects
  the mediaObject->tick(qint64) signal to the setTimeLables(qint64 slot)
*/
void MainWindow::setPhonon()
{
    this->mediaObject = new Phonon::MediaObject(this);
    this->audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(this->mediaObject, this->audioOutput);

    tickInterval = 1000;
    this->mediaObject->setTickInterval(tickInterval);
    connect(mediaObject, SIGNAL(tick(qint64)),
            this, SLOT(setTimeLabels(qint64)));
}


/*
  setMediaActions() is a function that sets up the Media buttons and scroll
  bar and connects them with the mediaObject. This must appear after
  setPhonon() function in the constructor. This must appear after the source
  in the constructor.
*/
void MainWindow::setMediaActions()
{
    // set up "Play/Pause" button
    connect(playButton, SIGNAL(toggled(bool)), this, SLOT(playPause(bool)));
    connect(this, SIGNAL(mediaPlay()), mediaObject, SLOT(play()));
    connect(this, SIGNAL(mediaPause()), mediaObject, SLOT(pause()));


    // set up "Stop" button
    connect(stopButton, SIGNAL(clicked()), mediaObject, SLOT(stop()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stopPressed()));


    // set up Next/Previous buttons
    connect(previousButton, SIGNAL(clicked()), this, SLOT(previousPressed()));


    // set up Slider
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(setSliderMaximum(Phonon::State,Phonon::State)));
    connect(mediaObject, SIGNAL(tick(qint64)),
            this, SLOT(setTrackData(qint64)));
    /*
    connect(trackSlider, SIGNAL(valueChanged(int)),
            this, SLOT(seekTrack(int)));
    */
}


/*
  playPause(bool) is a slot receives bool pp and emits a play or pause signal
  depending on whether the play/pause button is toggled on or off. The emitted
  signal is connected to the mediaObject play() and pause() slots.
*/
void MainWindow::playPause(bool pp)
{
    if (pp) {
        emit mediaPlay();
    }
    if (!pp) {
        emit mediaPause();
    }
}


/*
  setTimeLabels(qint64) is a slot which connects to mediaObject.tick(qint64) to
  set the timeElapsedLabel and the timeLeftLabel. This must appear after
  setPhonon() in the constructor.
*/
void MainWindow::setTimeLabels(qint64 _time)
{
    int time = _time/tickInterval;
    int timeRemaining = (mediaObject->totalTime())/tickInterval - time;
    QString timeElapsed, timeLeft;

    timeElapsed.append(QString::number(time/60));
    timeElapsed.append(":");

    timeLeft.append(QString::number(timeRemaining/60));
    timeLeft.append(":");

    if (time%60 < 10) {
        timeElapsed.append("0");
    }

    if (timeRemaining%60 < 10) {
        timeLeft.append("0");
    }

    timeElapsed.append(QString::number(time%60));
    timeLeft.append(QString::number(timeRemaining%60));

    timeElapsedLabel->setText(timeElapsed);
    timeLeftLabel->setText(timeLeft);
}


/*
  setTrackSliderPosition(qint64) is a slot which changes the position of
  trackSlider depending on the time passed to it (in ms).
*/
void MainWindow::setTrackData(qint64 _time)
{
    int position = _time/tickInterval;
    trackSlider->setValue(position);

    // set nowPlayingLabel to the current file name - this needs to be
    // improved to read ID3 tags to display <Artist Name> - <Track Name>
    nowPlayingLabel->setText(mediaObject->currentSource().fileName());
}


/*
  setSliderMaximum(Phonon::State,Phonon::State) is a slot which changes the
  maximum value of trackSlider when the mediaObject->totalTime() becomes
  available in the loading state.
*/
void MainWindow::setSliderMaximum(Phonon::State _a, Phonon::State _b)
{
    if (_a == 0 || _b==0) {
        trackSlider->setMaximum(mediaObject->totalTime()/tickInterval);
    }
}


/*
  seekTrack(int) is a slot which take input int from trackSlider which
  represents a time in seconds, and converts its to qint64 and milliseconds
  in order to seek to the given time.
*/
void MainWindow::seekTrack(int _time)
{
    mediaObject->seek(_time*tickInterval);
}

/*
  previousPressed() is a slot which is connected to previousButton->clicked()
  which resets the track to the beginning if it is more than 2 seconds into
  the song, or changes to the previous track if less, i.e. to change to a
  previous track you double click.
*/
void MainWindow::previousPressed()
{
    if (mediaObject->remainingTime() > (mediaObject->totalTime() - 2000))
    {
        // previous track
    }
    else {
        mediaObject->seek(0);
    }
}

/*
  stopPressed() is a lot which is connect to stopButton->clicked() which
  resets the state of the playButton when stop is pressed.
*/
void MainWindow::stopPressed()
{
    playButton->setChecked(false);
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
    this->serverpath = _srvr;
    this->username = _uname;
    this->password = _passwd;
}

// END: ConnectToServerDialog Related Methods and Slots ***********************
