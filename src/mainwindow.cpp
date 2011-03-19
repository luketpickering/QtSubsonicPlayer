#include <QtGui>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>
#include <QString>
#include <QStringList>
#include <QStringListModel>

#include "mainwindow.h"
#include "connecttoserverdialog.h"
#include "mediaplayer.h"
#include "controller/xmlcachehandler.h"
#include "dal/connectiondata.h"

// constructor for MainWindow
MainWindow::MainWindow()
{
    cd = new ConnectionData("hotblack.subsonic.org","49MR","49",-1);
    xch = new XMLCacheHandler(cd,this);

    setupUi(this);
    mediaPlayer = new MediaPlayer(this);

    setMenuActions();
    setMediaActions();
    setRequestConnections();

    xch->requestArtistList();
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




// ListView Population Methods and Slots **************************************

/*
  setRequestConnections is a public method that sets up the connections between
  the ListView objects and the requests to the xmlcache for the xml elements
  in order to set the list views.
*/
void MainWindow::setRequestConnections()
{
    connect(xch, SIGNAL(takeThisIndexOffMeItsCrampingMyStyle(QDomElement)),
            this, SLOT(changeArtists(QDomElement)));

    connect(artistListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(requestAlbums(QModelIndex)));

    connect(xch, SIGNAL(takeThisArtistDirectoryAwayItsJustGettingInTheWay(QDomElement)),
            this, SLOT(changeAlbums(QDomElement)));

    connect(albumTracksListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(requestTracks(QModelIndex)));

    connect(xch, SIGNAL(takeThisAlbumWhileStocksLast(QDomElement)),
            this, SLOT(changeTracks(QDomElement)));
}


/*
  changeArtists(QDomElement) is a slot which takes a QDomElement argument uses
  the xml to create a QStringList with all of the artist names and then uses
  this to create a QStringModelList with which is represented by the
  artistListView.
*/
void MainWindow::changeArtists(QDomElement artistsElement)
{
    QStringList *artistList = getValuesList(artistsElement, "artist", "name");

    artistListModel = new QStringListModel(*artistList, this);
    artistListView->setModel(artistListModel);
}


/*
  requestAlbums is a slot which takes a QModelIndex argument from the
  artistListView clicked(QModelIndex) slot and uses it to find the artist
  name that was clicked from the artistListModel and then sends a request to
  the xml cache for the artist's albums.
*/
void MainWindow::requestAlbums(QModelIndex _index)
{
    currentArtist = artistListModel->data(_index, 2).toString();


    xch->requestArtistAlbums(currentArtist);
}


/*
  changeAlbums(QDomElement) is a slot which takes a QDomElement arguement
  from the xml cache's signal (which was requested by
  requestAlbums(QModelIndex)) and uses that QDomElement get a QStringList of
  albums for the artist and creates a QStringListModel from it. It sets
  the QStringListModel as the albumTracksListView model.
*/
void MainWindow::changeAlbums(QDomElement artistElement)
{
    QStringList *albumList = getValuesList(artistElement, "album", "title");

    albumListModel = new QStringListModel(*albumList, this);
    albumTracksListView->setModel(albumListModel);
}


/*
  requestTracks(QModelIndex) is a slot which takes a QModelIndex argument from
  the albumTracksListView doubleclicked(QModelIndex) slot and uses it to find
  the album title that was clicked from the albumListModel and then sends a
  request to the xml cache for the album's tracks.
*/
void MainWindow::requestTracks(QModelIndex _index)
{
    QString album;
    album = albumListModel->data(_index, 2).toString();

    xch->requestAlbum(currentArtist,album);
}


/*
  changeTracks(QDomElement) is a slot which takes a QDomElement arguement
  from the xml cache's signal (which was requested by
  requestTracks(QModelIndex)) and uses that QDomElement get a QStringList of
  tracks for the album and creates a QStringListModel from it. It sets
  the QStringListModel as the albumTracksListView model.
*/
void MainWindow::changeTracks(QDomElement albumElement)
{
    //std::cout << qPrintable(albumElement.tagName()) << std::endl;
    QStringList *trackList = getValuesList(albumElement, "track", "title");

    trackListModel = new QStringListModel(*trackList, this);
    albumTracksListView->setModel(trackListModel);
}


/*
  getValuesList(toomanyicantbebotheredtowritethemout) takes a QDomElement
  and returns a list of attribute values for a given attribute (attributeName)
  for all child elements with a tage name (tagName).
*/
QStringList *MainWindow::getValuesList(const QDomElement element,
                                   const QString tagName,
                                   const QString attributeName)
{
    QStringList *valuesList = new QStringList;

    QDomNodeList nodeList = element.elementsByTagName(tagName);
    QDomNode node;
    QDomElement elem;class QDomElement;

    for(unsigned int i = 0; i < nodeList.length(); ++i)
    {
        node = nodeList.at(i);
        elem = node.toElement();
        if (!elem.isNull())
        {
            valuesList->append(elem.attribute(attributeName));
        }
    }

    return valuesList;
}

/*
  getValue(evenmoresoimdefinitelynotlistingthem) takes a QDomElement and
  returns the value for an attribute (returnAttributeName) for an element
  with tag name (tagName) and an attribute (attributeName) with a
  specified value (attributeValue).
*/
QString *MainWindow::getValue(QDomElement element,
                          QString tagName,
                          QString attributeName,
                          QString attributeValue,
                          QString returnAttributeName)
{
    QString *value = new QString;
    QDomNodeList nodeList = element.elementsByTagName(tagName);
    QDomNode node;
    QDomElement elem;

    for(unsigned int i = 0; i < nodeList.length(); ++i)
    {
        node = nodeList.at(i);
        elem = node.toElement();
        if (!elem.isNull())
        {
            if (elem.attribute(attributeName) == attributeValue)
            {
                *value = elem.attribute(returnAttributeName);
                return value;
            }
        }
    }

    return 0;
}
