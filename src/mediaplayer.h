#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>

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

public slots:
    void playClicked();
    void pauseClicked();
    void stopClicked();

    void gotTrack(QBuffer);
};

#endif // MEDIAPLAYER_H
