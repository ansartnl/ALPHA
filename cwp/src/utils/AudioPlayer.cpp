#include "AudioPlayer.h"

#include <QtCore/QTimer>
#include <phonon/AudioOutput>
#include <phonon/MediaObject>

AudioPlayer::AudioPlayer(const QString &path, int interval, QObject *parent) : QObject(parent)
{
    mMediaObject = new Phonon::MediaObject(this);
    mMediaObject->setCurrentSource(path);
    Phonon::createPath(mMediaObject, new Phonon::AudioOutput(Phonon::NotificationCategory, this));

    mTimer = new QTimer(this);
    mTimer->setInterval(interval * 1000);
    connect(mTimer, SIGNAL(timeout()), SLOT(onTimeout()));
}

void AudioPlayer::play()
{
    if (mTimer->isActive())
        return;

    onTimeout();
    mTimer->start();
}

void AudioPlayer::stop()
{
    mMediaObject->stop();
    mTimer->stop();
}

void AudioPlayer::onTimeout()
{
    mMediaObject->stop();
    mMediaObject->play();
}
