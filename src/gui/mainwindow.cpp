#include <QtGui>
#include <QList>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <phonon/SeekSlider>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <phonon/MediaSource>

#include "gui/mainwindow.h"
#include "gui/connecttoserverdialog.h"
#include "controller/requestprocessor.h"
#include "dal/connectiondata.h"

#include  <iostream>

// constructor for MainWindow
MainWindow::MainWindow()
{
    setupUi(this);
    setupMenu();

    mediaObject = new Phonon::MediaObject(this);
    audioOutput = new Phonon::AudioOutput(this);
    Phonon::createPath(mediaObject, audioOutput);
    tickInterval = 1000;
    setupMedia();

    showingTracks = false;

    cd = new ConnectionData();
    rp = new RequestProcessor(cd, this);
    setupRequests();
}


// BEGIN: Menu Related Methods and Slots  *************************************

/*
  private method which sets up the menu actions for the main menubar of
  MainWindow.
*/
void MainWindow::setupMenu()
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
  slot which shows the "About Subsonic Player" QMessageBox
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




// BEGIN: ConnectToServerDialog Related Methods and Sots **********************

/*
  private slot shows the "Connect To Server" dialog
*/
void MainWindow::connectToServer()
{
    ConnectToServerDialog *connectToServerDialog = new ConnectToServerDialog(this);

    connect(connectToServerDialog,
            SIGNAL(serverDetailsEntered(QString&,QString&,QString&,int&)),
            this, SLOT(setServerData(QString&,QString&,QString&,int&)));

    connectToServerDialog->exec();
}

/*
  slot: setData(QString x3) sets the values of public members
  server, username and password from the values input by the user
  in the ConnectToServerDialog
*/
void MainWindow::setServerData(QString& _host, QString& _usr, QString& _pss, int& _port)
{
    cd->changeDetails(_host, _usr, _pss, _port);
    cd->gotServerIP = false;

    rp->requestPing();
}

// END: ConnectToServerDialog Related Methods and Slots ***********************




// BEGIN: MediaPlayer Related Methods and Slots *******************************

/*
  private method that sets up the Media buttons and scroll bar and connects
  them with the mediaObject.
*/
void MainWindow::setupMedia()
{
    // set the SeekSlider to work with mediaPlayer's mediaObject
    seekSlider->setMediaObject(mediaObject);

    // set up Play/Pause/Stop buttons
    connect(playPauseButton, SIGNAL(clicked(bool)),
            this, SLOT(playPauseClicked()));

    connect(stopButton, SIGNAL(clicked()),
            this, SLOT(stopClicked()));

    // set up time labels
    connect(mediaObject, SIGNAL(tick(qint64)),
            this, SLOT(setTimeLabels(qint64)));

    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(moStateChanged(Phonon::State)));

    connect(previousButton, SIGNAL(clicked()),
            this, SLOT(previousClicked()));

    connect(this, SIGNAL(setTrack(QString)),
            this, SLOT(playTrack(QString)));
}

void MainWindow::playTrack(QString trackName)
{
    int currentTrackIndex = searchCurrentTrackPairList(trackName);

    Phonon::MediaSource
            mediaSource(*getUrl(currentTrackPairList->at(currentTrackIndex).second));

    mediaObject->setCurrentSource(mediaSource);

    QList<QUrl> queueList;
    for (int i = currentTrackIndex -1; i >= 0; --i)
    {
        queueList.append(*getUrl(currentTrackPairList->at(i).second));
    }

    mediaObject->setQueue(queueList);

    // for some reason the track will only play immediately is played, paused
    // and then played again.
    mediaObject->play();
    mediaObject->pause();
    mediaObject->play();
}


QUrl *MainWindow::getUrl(QString Id)
{
    QUrl *url = new QUrl("http://" + cd->host + ":" + QString::number(cd->port)
                         + "/rest/stream.view?u=" + cd->usr
                         + "&p=" + cd->pss + "&v=1.5.0" + "&c=QtSubsonicPlayer"
                         + "&id=" + Id);

    return url;
}

int MainWindow::searchCurrentTrackPairList(QString trackName)
{
    int index = 0;

    for (int i = currentTrackPairList->size() - 1; i >= 0; --i)
    {
        if (currentTrackPairList->at(i).first == trackName)
            index = i;
    }

    return index;
}

void MainWindow::playPauseClicked()
{
    if (playPauseButton->isChecked())
    {
        playPauseButton->setIcon(QIcon(":/mediaicons/media-playback-pause.png"));
        mediaObject->play();
    }
    else
    {
        mediaObject->pause();
        playPauseButton->setIcon(QIcon(":/mediaicons/media-playback-start.png"));
    }
}

void MainWindow::stopClicked()
{
    if (playPauseButton->isChecked())
    {
        playPauseButton->setIcon(QIcon(":/mediaicons/media-playback-start.png"));
        playPauseButton->setChecked(false);
    }

    mediaObject->stop();
}

void MainWindow::previousClicked()
{
    mediaObject->seek(0);
}

void MainWindow::moStateChanged(Phonon::State state)
{
    if(state == Phonon::PlayingState)
    {
        playPauseButton->setIcon(QIcon(":/mediaicons/media-playback-pause.png"));
        playPauseButton->setChecked(true);
    }
}

void MainWindow::setTimeLabels(qint64 _time)
{
    setTimeElapsedLabel(_time);
    setTimeRemainingLabel(_time);
}

/*
  slot which sets the timeElapsedLabel text
*/
void MainWindow::setTimeElapsedLabel(qint64 _time)
{
    int time = _time/tickInterval;
    QString timeElapsed;

    timeElapsed.append(QString::number(time/60));
    timeElapsed.append(":");

    if (time%60 < 10) {
        timeElapsed.append("0");
    }

    timeElapsed.append(QString::number(time%60));

    timeElapsedLabel->setText(timeElapsed);
}

void MainWindow::setTimeRemainingLabel(qint64 currentTime_ms)
{
    int currentTime = currentTime_ms/tickInterval;
    int time = currentTrackTotalTime - currentTime;

    QString timeRemaining;

    timeRemaining.append(QString::number(time/60));
    timeRemaining.append(":");

    if (time%60 < 10) {
        timeRemaining.append("0");
    }

    timeRemaining.append(QString::number(time%60));

    timeRemainingLabel->setText(timeRemaining);
}

// END: Media Related Methods and Slots ***************************************




// BEGIN: ListView Population and Requests Methods and Slots ******************

void MainWindow::setupRequests()
{
    connect(rp, SIGNAL(retrievedIndex(QStringList*)),
            this, SLOT(changeArtists(QStringList*)));

    connect(artistListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(requestAlbums(QModelIndex)));

    connect(rp, SIGNAL(retrievedArtistListing(QStringList*,QString)),
            this, SLOT(changeAlbums(QStringList*,QString)));

    connect(albumTracksListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(requestTracks(QModelIndex)));

    connect(rp, SIGNAL(retrievedAlbumListing(QList< QPair<QString,QString> >*,QString,QString)),
            this, SLOT(changeTracks(QList< QPair<QString,QString> >*,QString,QString)));

    connect(rp, SIGNAL(retrievedTrackData(QString,int)),
            this, SLOT(recieveTrack(QString,int)));

    connect(rp,SIGNAL(cacheReset()),
            this, SLOT(getIndex()));

    connect(rp, SIGNAL(pingSucceded()),
            this, SLOT(resetCache()));
}

void MainWindow::resetCache()
{
    rp->hardResetCache();
}

void MainWindow::getIndex()
{
    rp->getIndex();
}


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
}


void MainWindow::changeAlbums(QStringList* albumList, QString artist)
{
    albumListModel = new QStringListModel(*albumList, this);
    albumTracksListView->setModel(albumListModel);

    albumTracksListView->setSelectionMode(QAbstractItemView::SingleSelection);

    albumTracksLabel->setText("Albums");
    showingTracks = false;
    delete albumList;
}


void MainWindow::requestTracks(QModelIndex _index)
{
    if (showingTracks)
    {
        timeElapsedLabel->setText("-");
        timeRemainingLabel->setText("-");
        this->setWindowTitle("QtSubsonicPlayer: Buffering");
        mediaObject->stop();

        QString track;
        track = trackListModel->data(_index, 2).toString();
        if (track == "Play all...")
        {
            currentTrack
                = trackListModel
                    ->data(_index.sibling(_index.row() + 2, 0), 2).toString();
        }
        else if (track == " ")
        {
            // Do nothing
        }
        else
        {
            currentTrack = track;
        }

        emit setTrack(currentTrack);
    }
    else
    {
        trackListModel = new QStringListModel(QStringList("Loading..."), this);
        albumTracksListView->setModel(trackListModel);
        albumTracksListView->setSelectionMode(QAbstractItemView::NoSelection);

        listViewCurrentAlbum = albumListModel->data(_index, 2).toString();
        rp->getAlbum(listViewCurrentArtist, listViewCurrentAlbum);
    }
}

void MainWindow::changeTracks(QList< QPair<QString,QString> >* trackPairList,
                              QString artist, QString album)
{
    currentTrackPairList = trackPairList;

    QStringList *trackList = new QStringList();
    for (int i = trackPairList->size() -1; i >= 0; --i)
    {
        trackList->append(trackPairList->at(i).first);
    }

    showingTracks = true;
    albumTracksLabel->setText("Tracks");
    trackList->insert(0, " ");
    trackList->insert(0, "Play all...");
    trackListModel = new QStringListModel(*trackList, this);
    albumTracksListView->setModel(trackListModel);
    albumTracksListView->setSelectionMode(QAbstractItemView::SingleSelection);
    delete trackList;
}
