#ifndef ARINCPARSERTHREAD_H
#define ARINCPARSERTHREAD_H

#include <QtCore/QThread>

class ArincFilePrimitiveIterator;
class ArincEmptyStorage;

class ArincParserThread : public QThread
{
    Q_OBJECT
public:
    ArincParserThread(QObject *parent = 0);
    virtual ~ArincParserThread();

    bool startParser(ArincFilePrimitiveIterator *iterator, ArincEmptyStorage *storage);
    void cancel();
    inline bool isCanceled() const { return bCancel; }

protected:
    virtual void run();

private:
    bool bCancel;
    ArincFilePrimitiveIterator *it;
    ArincEmptyStorage *st;
};

#endif // ARINCPARSERTHREAD_H
