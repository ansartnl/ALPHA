#ifndef NETWORKWORKER_H
#define NETWORKWORKER_H

#include "aftnworkerbase.h"
#include "aftnclient.h"
#include "allcodecs.h"

#include "aftnmessageprocessor.h"

class MessageProcessor;
class NetworkWorkerBase;
class QSettings;

namespace Log4Qt {
class TelnetAppender;
}

class NetworkWorker: public AFTNWorkerBase
{
    Q_OBJECT
public:
    explicit NetworkWorker(QObject* parent = 0);
    virtual void run();

signals:
    void aftnStatusChanged(bool connected);
};

/* --------------------------------------------------
 * Internal useonly. We create this class, because we need signals,slots,events for timer and tcpsocket in thread.
 */
class NetworkWorkerBase: public WorkerBase
{
    Q_OBJECT
public:
    NetworkWorkerBase(QObject* parent = 0);

    bool start();
    void stop();

    bool isDisabled() const { return disable; }

signals:
    void aftnStatusChanged(bool connected);

public slots:
    void processMessages(const QString& nonsplittedmessage);
    void clientStateChanged(AFTNClient::AftnState state);
    void cksStateChanged(bool isConnected);

protected:
    void loadSettings();
    void initDefaultSettings();

    void checkParameters();

    virtual void timerEvent(QTimerEvent *event);

    bool enabledAutoSent();
    void sentChangeStateChannel(const QString &state);

    void setServerState(bool bMain);

    QString mType;
    QString mFillTime;
    QString mSerialNumber;

protected slots:
    void autoSentTelegrams();
    void checkChannelId();

    void mayBeSend(AFTNClientMessage& message);
    void OnSendedMessage(const AFTNClientMessage& message, const QString &serialNumber);
    void onCheckNextNum(quint16& nextNum);

private:
    MessageProcessor *processor;

    int timerId;
    int interval;
    int timerIdAutoSent;

    QString mHost1;
    QString mHost2;
    int mPort1;
    int mPort2;

    QString codecName;
    QTextCodec* codec;
    QScopedPointer<QITA2TextCodec>  ita2codec;
    QScopedPointer<QKOI7TextCodec>  ita5codec;
    QScopedPointer<MTK2Codec>       mtk2codec;
    QScopedPointer<MTK5Codec>       mtk5codec;

    QString mAftnName;
    QString mAftnNameServer;
    QString mChannelId;
    int mPingTimeout;
    bool mAnswerOnError;
    int mTelegramDelay;
    bool mSkipServiceMessages;

    AFTNClient *mClient;
    AFTNMessageProcessor *mProcessor;
    Log4Qt::TelnetAppender *telnetAppender;

    int attempt;

    bool disable;
    QString wholeMessage;
};
#endif //#ifndef NETWORKWORKER_H
