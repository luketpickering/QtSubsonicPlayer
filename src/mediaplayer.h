#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <phonon/MediaSource>

class QBuffer;

class MediaPlayer : public QObject
{
Q_OBJECT
public:
    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;

    int tickInterval;

    explicit MediaPlayer(QObject *parent = 0);

signals:
    void tock(qint64);
    void stateChanged(Phonon::State _newState,
                      Phonon::State _oldState);
    void totalTimeChanged(qint64);

public slots:
    void playClicked();
    void pauseClicked();
    void stopClicked();

    void gotTrack(QBuffer*, qint64, qint64);
};

#endif // MEDIAPLAYER_H
