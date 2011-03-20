#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <Phonon/MediaSource>
#include <QBuffer>

#include "mediaplayer.h"

/*
  Constructor for MediaPlayer
*/
MediaPlayer::MediaPlayer(QObject *parent) :
    QObject(parent)
{
    mediaObject = new Phonon::MediaObject(this);
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(mediaObject, audioOutput);

    // set the tickinterval to 1000ms
    tickInterval = 1000;
    mediaObject->setTickInterval(tickInterval);
    connect(mediaObject, SIGNAL(tick(qint64)), this, SIGNAL(tock(qint64)));
}

void MediaPlayer::gotTrack(QBuffer* buffer, qint64 currentSize, qint64 totalSize)
{
    Phonon::MediaSource mediaSource(buffer);
    mediaObject->setCurrentSource(mediaSource);
    mediaObject->play();
}


/*
  playClicked() is a slot that starts/resumes the playback of mediaObject when
  MainWindow's playButton is clicked
*/
void MediaPlayer::playClicked()
{
    mediaObject->play();
}



/*
  pauseClicked() is a slot that pauses the playback of mediaObject when
  MainWindow's pauseButton is clicked
*/
void MediaPlayer::pauseClicked()
{
    mediaObject->pause();
}



/*
  stopClicked() is a slot that stops the playback of mediaObject when
  MainWindow's stopButton is clicked
*/
void MediaPlayer::stopClicked()
{
    mediaObject->stop();
}
