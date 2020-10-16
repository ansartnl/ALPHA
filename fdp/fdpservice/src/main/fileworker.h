#ifndef AFTNWORKER_H
#define AFTNWORKER_H

#include "aftnworkerbase.h"

#include "aftnmessageprocessor.h"
#include "allcodecs.h"

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QLatin1String>

class AFTNMessageProcessor;
class AFTNClientMessage;
class MessageProcessor;

/**
 * @brief Read messages from file, parse it, and insert into database. Invalid messages inserts into man_aftn table
 *
 * @class AFTNWorker aftnworker.h
 */
class FileWorker: public AFTNWorkerBase
{
public:
    FileWorker(QObject *parent = 0);
    virtual void run();
};

class FileWorkerBase: public WorkerBase
{
    Q_OBJECT
    QScopedPointer<AFTNMessageProcessor> mProcessor;
    QDir messagesDir;
    QDir messagesDoneDir;
    QString fileExt;
    quint16 mesNum;
    int mTelegramDelay;

    mutable QDateTime sendTime;
public:
    FileWorkerBase(QObject* parent = 0);

public:

    bool start();
    void stop();

protected:
    void loadSettings();
    void initDefaultSettings();

    void sendCatalogOrganizer();
    void mayBeSend(AFTNClientMessage& message);
    void writeSentAftn(const AFTNClientMessage& message);
    QByteArray toByteArray(const AFTNClientMessage& message) const;
    void sendMessage(const AFTNClientMessage& message);
    quint32 nextMesNum();
    QString mesNumToString() const;
    QString channelId() const;
    QString aftnName() const;
    void onCheckNextNum(quint16& nextNum);

    void timerEvent(QTimerEvent *event);

    QString mType;
    QString mFillTime;

private:
    MessageProcessor *processor;

    int timerId;
    int interval;

    QByteArray codec;
    QString filter;
    QTextCodec* codecCatalogOrganizer;
    QScopedPointer<QITA2TextCodec>  ita2codec;
    QScopedPointer<QKOI7TextCodec>  ita5codec;
    QScopedPointer<MTK2Codec>       mtk2codec;
    QScopedPointer<MTK5Codec>       mtk5codec;
    QScopedPointer<QTextEncoder> encoderCatalogOrganizer;
    QScopedPointer<QTextDecoder> decoderCatalogOrganizer;
};

#endif // AFTNWORKER_H
