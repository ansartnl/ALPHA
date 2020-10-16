#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QtCore/QObject>

class QTimer;

namespace Phonon {
class MediaObject;
}

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(const QString &path, int interval = 0, QObject *parent = 0);

public slots:
    void play();
    void stop();

private slots:
    void onTimeout();

private:
    Phonon::MediaObject *mMediaObject;
    QTimer *mTimer;
};

#endif // AUDIOPLAYER_H
