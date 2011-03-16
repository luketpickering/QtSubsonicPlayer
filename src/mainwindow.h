#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include "ui_mainwindow.h"

class ConnectToServerDialog;

class MainWindow
    : public QMainWindow,
      public Ui::MainWindow
{
    Q_OBJECT

public:
    QString serverpath;
    QString username;
    QString password;

    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;

    int tickInterval;

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
    void playPause(bool);
    void setTimeLabels(qint64);
    void setTrackData(qint64);
    void setSliderMaximum(Phonon::State,Phonon::State);
    void seekTrack(int);
    void previousPressed();
    void stopPressed();
};

#endif // MAINWINDOW_H
