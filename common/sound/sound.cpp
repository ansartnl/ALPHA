#include "sound.h"

#include <phonon/AudioOutput>
#include <phonon/MediaObject>


namespace sound
{

void play(const QString &filename)
{
    Phonon::MediaObject *mediaObject = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource(filename));
    mediaObject->play();
    QObject::connect(mediaObject, SIGNAL(finished()), mediaObject, SLOT(deleteLater()));
}

}
