#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>

#include "ui_mainwindow.h"


class RequestProcessor;
class ConnectionData;
class QString;
class QStringListModel;
class ConnectToServerDialog;

class MainWindow
    : public QMainWindow,
      public Ui::MainWindow
{
    Q_OBJECT

public:
    // Members
    ConnectionData *cd;
    RequestProcessor *rp;

    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;

    int tickInterval;
    int currentTrackTotalTime;
    QString currentTrack;

    bool showingTracks;
    QString listViewCurrentArtist;
    QString listViewCurrentAlbum;

    QStringListModel *artistListModel;
    QStringListModel *albumListModel;
    QStringListModel *trackListModel;

    QString serverpath;
    QString username;
    QString password;

    // Methods
    MainWindow();

signals:

private:
    void setupMenu();
    void setupMedia();
    void setupRequests();

public slots:
    void about();
    void connectToServer(bool = false);
    void setServerData(QString&, QString&, QString&);
    void connectToServerFirstRunErr();

    void setTimeLabels(qint64);
    void setTimeElapsedLabel(qint64); 
    void setTimeRemainingLabel(qint64);

    void stopClicked();
    void playPauseClicked();
    void moStateChanged(Phonon::State);

    void requestAlbums(QModelIndex);
    void requestTracks(QModelIndex);
    void changeArtists(QStringList*);
    void changeAlbums(QStringList*,QString);
    void changeTracks(QStringList*,QString,QString);
    void recieveTrack(QString,int);
};

#endif // MAINWINDOW_H
