#include "aftnclientcta.h"

#include <QTimer>
#include <QStringList>
#include <QSettings>
#include <QDateTime>

#include <QHostAddress>

#include "allcodecs.h"
#include "ChTelegram.h"
#include "TelegramParser.h"
#include "TelegramTypes.h"
#include "Transliteration.h"
#include "AftnTelegram.h"

#include "HttpPinger.h"

AFTNClientCTA::AFTNClientCTA(QObject *parent)
    : AFTNClient(parent)
{
    chTimer = new QTimer(this);
    chTimer->setInterval(60 * 1000); // Do not edit time!!!
    connect(chTimer, SIGNAL(timeout()), SLOT(onChTimer()));

    pinger = new HttpPinger(this);
    pinger->setMaximumErrorsCount(2);
    pinger->setInterval(1);
    pinger->setRequestType(HttpPingGlobal::RT_GET);
    connect(pinger, SIGNAL(connectionLost()), SLOT(onConnectionLost()));
    connect(pinger, SIGNAL(error(QString)), SLOT(onHttpError(QString)));

    connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            SLOT(onStateChanged(QAbstractSocket::SocketState)));
}

AFTNClientCTA::~AFTNClientCTA()
{
}

void AFTNClientCTA::sendAuthorizationRequest()
{
    setAftnState(AftnReceiving);
    if (isEnableCH())
        chTimer->start();
    startPingTimer(3 * 1000);
}

void AFTNClientCTA::closeChannel()
{
    chTimer->stop();
    emit cksState(false);
    AFTNClient::closeChannel();
}

void AFTNClientCTA::pingTimeoutTimer()
{
}

void AFTNClientCTA::ping(bool bSend)
{
    AFTNClientMessage message;
    if(bSend)
        emit canSend(message);
    if(!message.isEmpty())
    {
        sendMessage(message);
    }
}

int AFTNClientCTA::processMessages()
{
    QByteArray& buffer = getBuffer();
    QString string;

    string = textCodec()->makeDecoder()->toUnicode(buffer);
    //string = string.simplified();

    TelegramParser telParse;
    Telegram* pTel = telParse.parse(string.simplified());
    AftnTelegram* pAftnTel = dynamic_cast<AftnTelegram*>(pTel);
    if((pTel && TelegramType::isCh(pTel->type())) || (pAftnTel /*&& (pAftnTel->mAftn.senderAddress == aftnServerName())*/))
    {
        QSettings settings;
        bool bOk = settings.status() == QSettings::NoError;
        if(bOk)
        {
            settings.setValue("sysCksChannelState", QDateTime::currentDateTimeUtc());
            emit cksState(true);
        }
    }

    emit messages(string);

    buffer.clear();
    /////////////////////
//    ping(false);
//    qWarning() << "AFTNClient: send confirmation " << string.count("NNNN");
//    for (int i = 0; i < string.count("NNNN"); ++i)
//        write(QByteArray("\xFD\x01", 2));
    //////////////////////////
    return 0;
}

void AFTNClientCTA::onChTimer()
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    if (!(now.time().minute() % 20))
    {
        nextMesNum();
        ChStruct s;
        s.version = ParseVersion::V_TC95;
        s.transmissionIdentificator = Transliteration::toTC95(channelId() + mesNumToString());
        s.additionServiceIndication = QDateTime::currentDateTimeUtc().toString("hhmm");
#if defined (BAN_BUILD)
        s.lr = ParseVersion::toTC95(channelId() + mesNumToString());
#endif
        ChTelegram m(s);
        AFTNClientMessage mes;
        mes.setMessage(m.data());
        sendMessage(mes, false);
    }

    now.setTime(QTime(now.time().hour(), now.time().minute()));
    if (!((now.time().minute() - 3) % 20))
    {
        // check incoming CH
        bool bGood(false);
        QSettings settings;
        bool bOk = settings.status() == QSettings::NoError;
        if(bOk)
        {
            QDateTime dt = settings.value("sysCksChannelState").toDateTime();
            bOk = settings.status() == QSettings::NoError;
            if(bOk)
            {
                if(dt.msecsTo(now) < 5 * 60 * 1000)
                    bGood = true;
            }
        }
        if(!bGood)
        {
            //disconnectFromHost();
        }
        emit cksState(bGood);
    }
}

void AFTNClientCTA::onConnectionLost()
{
    qWarning() << "AFTNClient: ping timeout";
    disconnectFromHost();
}

void AFTNClientCTA::onHttpError(const QString &errorString)
{
    qWarning() << QString("AFTNClient: http error - '%1'").arg(errorString);
}

void AFTNClientCTA::onStateChanged(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::ConnectedState)
        pinger->start(peerAddress().toString());
    else if (state == QAbstractSocket::UnconnectedState)
        pinger->stop();
}
