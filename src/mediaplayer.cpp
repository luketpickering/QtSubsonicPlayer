#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <Phonon/MediaSource>
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

    // set the MediaSource (single file on my (michael) server for now) if
    // trying this outq
    Phonon::MediaSource mediaSource("http://localhost:4040/rest/stream.view?u="
                                    "michael&p=america&c=QtSubsonicPlayer&v=1."
                                    "5.0&id=2f686f6d652f6d69636861656c2f4d7573"
                                    "69632f4d50332f4a6f686e204d617965722f48656"
                                    "176696572205468696e67732f303120436c617269"
                                    "74792e6d7033");

    mediaObject->setCurrentSource(mediaSource);

    connect(mediaObject, SIGNAL(tick(qint64)), this, SIGNAL(tock(qint64)));
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
