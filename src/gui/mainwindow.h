#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml/QDomElement>
#include "ui_mainwindow.h"
#include <Phonon>

class XMLCacheHandler;
class ConnectionData;
class PingTest;
class QDomDocument;
class QString;
class QStringListModel;
class ConnectToServerDialog;
class MediaPlayer;
class QBuffer;

class MainWindow
    : public QMainWindow,
      public Ui::MainWindow
{
    Q_OBJECT

public:
    // Members
    XMLCacheHandler *xch;
    ConnectionData *cd;
    PingTest *pingtest;

    bool showingTracks;
    QString listViewCurrentArtist;
    QString listViewCurrentAlbum;

    QStringListModel *artistListModel;
    QStringListModel *albumListModel;
    QStringListModel *trackListModel;

    QString serverpath;
    QString username;
    QString password;

    MediaPlayer *mediaPlayer;

    // Methods
    MainWindow();

signals:
    void mediaPlay();
    void mediaPause();

private:
    void setMenuConnections();
    void setMediaConnections();
    void setRequestConnections();

public slots:
    void about();

    void connectToServer();
    void setServerData(QString&, QString&, QString&);

    void mediaPlayerChange(Phonon::State);
    void playPauseClicked(bool);
    void stopButtonClicked();
    void setTimeElapsedLabel(qint64); 

    void requestArtists();
    void changeArtists(QDomElement);
    void requestAlbums(QModelIndex);
    void changeAlbums(QDomElement);
    void requestTracks(QModelIndex);
    void changeTracks(QDomElement);
};

#endif // MAINWINDOW_H
