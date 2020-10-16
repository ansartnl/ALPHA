#ifndef FILECOPY_H
#define FILECOPY_H

#include <QThread>

class CFileCopy : public QThread
{
    Q_OBJECT
public:
    explicit CFileCopy(QObject *parent = 0);
    
    void run();

    QString source;
    QString dest;

signals:
    void tick();
    void done();

public slots:
    void cancel();

private:
    bool isRunning;
};

#endif // FILECOPY_H
