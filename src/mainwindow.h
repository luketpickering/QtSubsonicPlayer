#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include "ui_mainwindow.h"
#include "xmlrequests/pingtest.h"

class ConnectToServerDialog;
class MediaPlayer;
class QBuffer;

class MainWindow
    : public QMainWindow,
      public Ui::MainWindow
{
    Q_OBJECT

public:
    QString serverpath;
    QString username;
    QString password;

    MediaPlayer *mediaPlayer;

    QBuffer *currentTrackBuffer;

    MainWindow();

signals:
    void mediaPlay();
    void mediaPause();

private:
    void setMenuActions();
    void setPhonon();
    void setMediaActions();

private slots:
    void connectToServer();
    void about();
    void setServerData(QString&, QString&, QString&);
    void setTimeElapsedLabel(qint64); 
};

#endif // MAINWINDOW_H
