#ifndef STDINREADER_H
#define STDINREADER_H

#include <QtCore/QThread>

class StdinReader : public QThread
{
    Q_OBJECT

public:
    StdinReader(QObject *parent = 0);
    void run();
signals:
    void received(const QString &msg);
};

#endif // STDINREADER_H
