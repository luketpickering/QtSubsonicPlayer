#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPair>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>

#include "ui_mainwindow.h"


class RequestProcessor;
class ConnectionData;
class QString;
class QUrl;
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

    QList< QPair<QString,QString> > *currentTrackPairList;
    QStringListModel *artistListModel;
    QStringListModel *albumListModel;
    QStringListModel *trackListModel;

    // Methods
    MainWindow();

private:
    void setupMenu();
    void setupMedia();
    void setupRequests();
    int searchCurrentTrackPairList(QString);
    QUrl *getUrl(QString);

signals:
    void setTrack(QString);

public slots:
    void about();
    void connectToServer();
    void setServerData(QString&,QString&,QString&,int&);

    void setTimeLabels(qint64);
    void setTimeElapsedLabel(qint64); 
    void setTimeRemainingLabel(qint64);

    void stopClicked();
    void playPauseClicked();
    void previousClicked();
    void moStateChanged(Phonon::State);

    void resetCache();
    void getIndex();
    void requestAlbums(QModelIndex);
    void requestTracks(QModelIndex);
    void changeArtists(QStringList*);
    void changeAlbums(QStringList*,QString);
    void changeTracks(QList< QPair<QString,QString> >*,QString,QString);
    void playTrack(QString);
};

#endif // MAINWINDOW_H
