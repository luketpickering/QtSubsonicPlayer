#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml/QDomElement>
#include <phonon/phononnamespace.h>

#include "ui_mainwindow.h"


class RequestProcessor;
class ConnectionData;
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
    ConnectionData *cd;
    RequestProcessor *rp;

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

    void requestAlbums(QModelIndex);
    void requestTracks(QModelIndex);
    void changeArtists(QStringList*);
    void changeAlbums(QStringList*,QString);
    void changeTracks(QStringList*,QString,QString);
};

#endif // MAINWINDOW_H
