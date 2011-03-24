#include <QtGui>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QThread>
#include <phonon/SeekSlider>

#include <iostream>

#include "gui/mainwindow.h"
#include "gui/connecttoserverdialog.h"
#include "mediaplayer.h"
#include "controller/requestprocessor.h"
#include "dal/connectiondata.h"

// constructor for MainWindow
MainWindow::MainWindow()
{
    setupUi(this);
    mediaPlayer = new MediaPlayer(this);

    // for testing
    mediaPlayer->serverpath = "hotblack.subsonic.org";
    mediaPlayer->username = "michael";
    mediaPlayer->password = "america";

    cd = new ConnectionData(mediaPlayer->serverpath,
                            mediaPlayer->username,
                            mediaPlayer->password,-1);

    rp = new RequestProcessor(cd, this);
    showingTracks = false;

    setMenuConnections();
    setMediaConnections();
    setRequestConnections();

    rp->getIndex();
}



// BEGIN: Menu Related Methods and Slots  *************************************

/*
  setMenuActions() is a function which sets up the menu actions for the main
  menubar of MainWindow.
*/
void MainWindow::setMenuConnections()
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





// BEGIN: MediaPlayer Related Methods and Slots *******************************

/*
  setMediaActions() is a function that sets up the Media buttons and scroll
  bar and connects them with the mediaObject.
*/
void MainWindow::setMediaConnections()
{
    // set the SeekSlider to work with mediaPlayer's mediaObject
    seekSlider->setMediaObject(mediaPlayer->mediaObject);

    // set up Play/Pause/Stop buttons
    connect(playPauseButton, SIGNAL(clicked(bool)),
            this, SLOT(playPauseClicked(bool)));

    connect(stopButton, SIGNAL(clicked()),
            this, SLOT(stopButtonClicked()));

    // set up time labels
    connect(mediaPlayer, SIGNAL(tock(qint64)),
            this, SLOT(setTimeElapsedLabel(qint64)));

    // connect
    connect(rp, SIGNAL(retrievedTrackData(QString,int)),
            mediaPlayer, SLOT(gotTrack(QString,int)));


    connect(mediaPlayer, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(mediaPlayerChange(Phonon::State)));

    connect(mediaPlayer, SIGNAL(totalTimeChanged(qint64)),
            seekSlider, SLOT(update()));
}

void MainWindow::mediaPlayerChange(Phonon::State _newState)
{
    if (_newState == Phonon::PlayingState)
    {
        playPauseButton->
                setIcon(QIcon(":/mediaicons/media-playback-pause.png"));
        playPauseButton->setChecked(true);
    }
    else if (_newState == Phonon::PausedState
        || _newState == Phonon::StoppedState)
    {
        playPauseButton->
                setIcon(QIcon(":/mediaicons/media-playback-start.png"));
        playPauseButton->setChecked(false);
    }
}

void MainWindow::playPauseClicked(bool play)
{
    if (!play)
    {
        playPauseButton->
                setIcon(QIcon(":/mediaicons/media-playback-start.png"));
        mediaPlayer->pauseClicked();
    }
    else
    {
        playPauseButton->
                setIcon(QIcon(":/mediaicons/media-playback-pause.png"));
        mediaPlayer->playClicked();
    }
}

void MainWindow::stopButtonClicked()
{
    mediaPlayer->stopClicked();

    playPauseButton->setChecked(false);
    playPauseButton->setIcon(QIcon(":/mediaicons/media-playback-start.png"));
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




// BEGIN: ConnectToServerDialog Related Methods and Sots **********************

/*
  private slot shows the "Connect To Server" dialog
*/
void MainWindow::connectToServer()
{
    ConnectToServerDialog *connectToServerDialog =
            new ConnectToServerDialog(this);

    // connect the signal from connectToServerDialog to the setServerData slot
    connect(connectToServerDialog,
            SIGNAL(serverDetailsEntered(QString&, QString&, QString&)),
            this, SLOT(setServerData(QString&,QString&,QString&)));

    QMessageBox::warning(this, tr("ERROR ERROR ERROR!"),
                         tr("<p>This does nothing yet!!!!!!!1!!!one!!!</p>"));

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
    mediaPlayer->serverpath = _srvr;
    mediaPlayer->username = _uname;
    mediaPlayer->password = _passwd;
}

// END: ConnectToServerDialog Related Methods and Slots ***********************




// BEGIN: ListView Population Methods and Slots *******************************

/*
  setRequestConnections is a public method that sets up the connections between
  the ListView objects and the requests to the xmlcache for the xml elements
  in order to set the list views.
*/
void MainWindow::setRequestConnections()
{
    connect(rp, SIGNAL(retrievedIndex(QStringList*)),
            this, SLOT(changeArtists(QStringList*)));

    connect(artistListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(requestAlbums(QModelIndex)));

    connect(rp, SIGNAL(retrievedArtistListing(QStringList*,QString)),
            this, SLOT(changeAlbums(QStringList*,QString)));

    connect(albumTracksListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(requestTracks(QModelIndex)));

    connect(rp, SIGNAL(retrievedAlbumListing(QStringList*,QString,QString)),
            this, SLOT(changeTracks(QStringList*,QString,QString)));
}


/*
  changeArtists(QDomElement) is a slot which takes a QDomElement argument uses
  the xml to create a QStringList with all of the artist names and then uses
  this to create a QStringModelList with which is represented by the
  artistListView.
*/
void MainWindow::changeArtists(QStringList* artistList)
{
    artistListModel = new QStringListModel(*artistList, this);
    artistListView->setModel(artistListModel);
    delete artistList;
}


/*
  requestAlbums is a slot which takes a QModelIndex argument from the
  artistListView clicked(QModelIndex) slot and uses it to find the artist
  name that was clicked from the artistListModel and then sends a request to
  the xml cache for the artist's albums.
*/
void MainWindow::requestAlbums(QModelIndex _index)
{
    albumListModel = new QStringListModel(QStringList("Loading..."), this);
    albumTracksListView->setModel(albumListModel);
    albumTracksListView->setSelectionMode(QAbstractItemView::NoSelection);

    listViewCurrentArtist = artistListModel->data(_index, 2).toString();
    rp->getArtist(listViewCurrentArtist);

    showingTracks = false;
}


/*
  changeAlbums(QDomElement) is a slot which takes a QDomElement arguement
  from the xml cache's signal (which was requested by
  requestAlbums(QModelIndex)) and uses that QDomElement get a QStringList of
  albums for the artist and creates a QStringListModel from it. It sets
  the QStringListModel as the albumTracksListView model.
*/
void MainWindow::changeAlbums(QStringList* albumList, QString artist)
{
    if (artist != listViewCurrentArtist)
    {
        QMessageBox::warning(this, "Error",
                             "<p> The album showing does not belong to the"
                             "artist you selected.</p>");
    }

    albumListModel = new QStringListModel(*albumList, this);
    albumTracksListView->setModel(albumListModel);

    albumTracksListView->setSelectionMode(QAbstractItemView::SingleSelection);

    showingTracks = false;
    delete albumList;
}


/*
  requestTracks(QModelIndex) is a slot which takes a QModelIndex argument from
  the albumTracksListView doubleclicked(QModelIndex) slot and uses it to find
  the album title that was clicked from the albumListModel and then sends a
  request to the xml cache for the album's tracks.
*/
void MainWindow::requestTracks(QModelIndex _index)
{
    if (!showingTracks)
    {
        trackListModel = new QStringListModel(QStringList("Loading..."), this);
        albumTracksListView->setModel(trackListModel);
        albumTracksListView->setSelectionMode(QAbstractItemView::NoSelection);

        listViewCurrentAlbum = albumListModel->data(_index, 2).toString();
        rp->getAlbum(listViewCurrentArtist,
                     listViewCurrentAlbum);
    }
    else
    {
        QString track;
        track = trackListModel->data(_index, 2).toString();

        std::cout << "Requesting Stream" << std::endl;
        rp->getTrack(listViewCurrentArtist,
                     listViewCurrentAlbum,
                     track);
    }
}


/*
  changeTracks(QDomElement) is a slot which takes a QDomElement arguement
  from the xml cache's signal (which was requested by
  requestTracks(QModelIndex)) and uses that QDomElement get a QStringList of
  tracks for the album and creates a QStringListModel from it. It sets
  the QStringListModel as the albumTracksListView model.
*/
void MainWindow::changeTracks(QStringList* trackList,
                              QString artist, QString album)
{
    if (artist != listViewCurrentArtist)
    {
        QMessageBox::warning(this, "Error",
                             "<p>TITS (Artist)</p>");
    }
    if (album != listViewCurrentAlbum)
    {
        QMessageBox::warning(this, "Error",
                             "<p>TITS (Album)</p>");
    }

    showingTracks = true;
    trackListModel = new QStringListModel(*trackList, this);
    albumTracksListView->setModel(trackListModel);
    albumTracksListView->setSelectionMode(QAbstractItemView::SingleSelection);
    delete trackList;
}

