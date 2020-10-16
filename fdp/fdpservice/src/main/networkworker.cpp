#include "networkworker.h"

#include "messageprocessor.h"

#include "aftnclient.h"
#include "aftnclientcta.h"
#include "aftnservice.h"
#include "aftnmessageprocessor.h"

#include "qita2textcodec.h"

#include "logmanager.h"
#include "telnetappender.h"
#include "mainthreadappender.h"

#include "qtautotransaction.h"

#include "DBSettings.h"
#include "pulsar_fdp_settings.h"

#if defined(DUSHANBE_BUILD)
#include "fpl/fpltan.h"
#include "DBOCI.h"
#endif

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDateTime>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#define TIMERAUTOSENT 3000

NetworkWorker::NetworkWorker(QObject* parent) :
AFTNWorkerBase(parent)
{
    if (objectName().isEmpty())
        setObjectName("network_worker");
}

void NetworkWorker::run()
{
    NetworkWorkerBase ptr;
    if (ptr.isDisabled())
        return;

    setWorker(&ptr);

    connect(&ptr, SIGNAL(aftnStatusChanged(bool)), this, SIGNAL(aftnStatusChanged(bool)));
    qloginfo(tr("Network processing"));

    ptr.start();

    // main loop
    exec();

    ptr.stop();
    setWorker(0);

    qloginfo(tr("Stop network processing"));
}

// --------------------------------------
NetworkWorkerBase::NetworkWorkerBase(QObject* parent) :
    WorkerBase(parent), processor(0), codec(0), ita2codec(new QITA2TextCodec()),
    ita5codec(new QKOI7TextCodec()), mtk2codec(new MTK2Codec()), mtk5codec(new MTK5Codec()),
    mPingTimeout(0), mAnswerOnError(false), mTelegramDelay(0), mSkipServiceMessages(true),
    mClient(0), mProcessor(new AFTNMessageProcessor(QSqlDatabase(), this)),
    attempt(0), disable(true)
{
    if (objectName().isEmpty())
        setObjectName("network_worker");

    disable = settings()->value("tcp_ip_connector/disable", false).toBool();
    if ( disable )
    {
        qlogwarn("Disabled");
        return;
    }

    QString sClient = settings()->value("tcp_ip_connector/client_type", "CKS").toString();
    if(sClient.toUpper() == "CKS")
    {
        mClient = new AFTNClient(this);
    } else if(sClient.toUpper() == "CTA")
    {
        mClient = new AFTNClientCTA(this);
        connect(mClient, SIGNAL(cksState(bool)), SLOT(cksStateChanged(bool)));
    } else
    {
        mClient = new AFTNClient(this);
    }

    init();

    using namespace Log4Qt;

    MainThreadAppender *mainThreadAppender = qobject_cast<MainThreadAppender*>(LogManager::rootLogger()->appender("mainThreadAppender"));
    if (mainThreadAppender)
    {
        telnetAppender = qobject_cast<TelnetAppender*>(mainThreadAppender->appender("telnetAppender"));
        if (telnetAppender)
            telnetAppender->setWelcomeMessage("AFTN client: unconnected");
    }
}

void NetworkWorkerBase::initDefaultSettings()
{
    qloginfo("Init default settings");
    settings()->beginGroup("tcp_ip_connector");
    settings()->setValue("disabled", false);
    settings()->setValue("comment",
        "Настройки модуля tcp/ip подключения к AFTN серверу.");
    settings()->setValue("timer_interval", 5);
    settings()->setValue("timer_interval/comment", "Таймер проверки подключения.");
    settings()->setValue("host1", "127.0.0.1");
    settings()->setValue("host1/comment", "Адрес сервера AFTN сообщений.");
    settings()->setValue("host2", "192.168.1.1");
    settings()->setValue("host2/comment",
        "Адрес запасного сервера AFTN сообщений.");
    settings()->setValue("port1", "10200");
    settings()->setValue("port1/comment", "Порт сервера AFTN сообщений.");
    settings()->setValue("port2", "10200");
    settings()->setValue("port2/comment",
        "Порт запасного сервера AFTN сообщений.");
    settings()->setValue("aftn_name", "somename");
    settings()->setValue("aftn_name/comment", "Имя абонента.");
    settings()->setValue("aftn_name_server", "somename");
    settings()->setValue("aftn_name_server/comment", "Имя сервера ЦКС.");
    settings()->setValue("aftn_channel_id", "somename");
    settings()->setValue("aftn_channel_id/comment", "Идентификатор канала (3 символа).");
    settings()->setValue("ping_timeout", 10);
    settings()->setValue("ping_timeout/comment", "Таймер ping'а.");
    settings()->setValue("text_codec", "ITA2");
    settings()->setValue("text_codec/comment", "Кодировка сообщений.");
    settings()->setValue("answer_on_error", false);
    settings()->setValue("answer_on_error/comment", "Автоответ отправителю на ошибочное сообщение.");
    settings()->setValue("client_type", "CKS");
    settings()->setValue("client_type/comment", "Тип клиента (CKS,CTA).");
    settings()->setValue("telegram_delay", 10);
    settings()->setValue("telegram_delay/comment", "Задержка отправки телеграмм (в секундах).");
    settings()->setValue("skip_service_messages", true);
    settings()->setValue("skip_service_messages/comment", "Не отправлять служебные сообщения на ручную обработку.");
    settings()->setValue("send_CH", true);
    settings()->setValue("send_CH/comment", "Отправлять служебные сообщения CH.");

    settings()->beginGroup("aftn_message_processor");
    settings()->setValue("comment", "Настройки модуля парсинга AFTN сообщений.");
    settings()->setValue("fir_name", "UMMV");
    settings()->setValue("fir_name/comment", "FIR");
    settings()->setValue("cut_fir", true);
    settings()->setValue("cut_fir/comment", "Вырезать FIR из маршрута.");
    settings()->setValue("validate_route", true);
    settings()->setValue("validate_route/comment", "Проверять корректность маршрута.");
    settings()->setValue("validate_route_lite", false);
    settings()->setValue("validate_route_lite/comment",
                         "Проверять корректность маршрута (слабая проверка).");
    settings()->setValue("FPL2012", false);
    settings()->setValue("FPL2012/comment", "Формат сообщений FPL2012 или FPL2007");
    settings()->endGroup();

    settings()->endGroup();
}

void NetworkWorkerBase::loadSettings()
{
    qloginfo("Load settings : network");
    settings()->beginGroup("tcp_ip_connector");

    interval = settings()->value("timer_interval").toInt();

    mHost1 = settings()->value("host1").toString();
    mHost2 = settings()->value("host2").toString();
    mPort1 = settings()->value("port1").toInt();
    mPort2 = settings()->value("port2").toInt();

    codecName = settings()->value("text_codec").toString();

    // FIXME Workaround as static text coder
    if (codecName.toUpper() == "ITA2") {
        qDebug() << "Text codec is ITA2";
        codec = ita2codec.data();
    } else if (codecName.toUpper() == "ITA5") {
        qDebug() << "Text codec is ITA5";
        codec = ita5codec.data();
    } else if (codecName.toUpper() == "MTK2") {
        qDebug() << "Text codec is MTK2";
        codec = mtk2codec.data();
    } else if (codecName.toUpper() == "MTK5") {
        qDebug() << "Text codec is MTK5";
        codec = mtk5codec.data();
    } else {
        codec = QTextCodec::codecForName(codecName.toLatin1());
    }

    mAftnName = settings()->value("aftn_name").toString();
    mAftnNameServer = settings()->value("aftn_name_server").toString();
    mChannelId = settings()->value("aftn_channel_id").toString();
    mPingTimeout = settings()->value("ping_timeout", 10).toInt();
    mAnswerOnError = settings()->value("answer_on_error", false).toBool();
    mTelegramDelay = settings()->value("telegram_delay", 0).toInt();
    mSkipServiceMessages = settings()->value("skip_service_messages", true).toBool();
    mClient->setEnableCH(settings()->value("send_CH", true).toBool());

    settings()->beginGroup("aftn_message_processor");
    mProcessor->setFIRName(settings()->value("fir_name").toString());
    mProcessor->setSaveOnlyFIR(settings()->value("cut_fir").toBool());
    mProcessor->setValidateRoute(settings()->value("validate_route").toBool());
    mProcessor->setValidateRouteLite(settings()->value("validate_route_lite", false).toBool());
    mProcessor->setStandard(settings()->value("FPL2012").toBool() ? AFTNMessageProcessor::FPL_2012 : AFTNMessageProcessor::FPL_2007);
    settings()->endGroup();
    settings()->endGroup();
}

bool NetworkWorkerBase::start()
{
    connect(mClient, SIGNAL(aftnStateChanged(AFTNClient::AftnState)), this,
        SLOT(clientStateChanged(AFTNClient::AftnState)));
    connect(mClient, SIGNAL(messages(QString)), this, SLOT(processMessages(QString)));
    connect(mClient, SIGNAL(canSend(AFTNClientMessage&)), this, SLOT(mayBeSend(AFTNClientMessage&)), Qt::DirectConnection);
    connect(mClient, SIGNAL(sendedMessage(const AFTNClientMessage&, const QString &)), this,
            SLOT(OnSendedMessage(const AFTNClientMessage&, const QString&)), Qt::DirectConnection);
    connect(mClient, SIGNAL(checkNextNum(quint16&)), this, SLOT(onCheckNextNum(quint16&)), Qt::DirectConnection);

    if (!settings()->contains("tcp_ip_connector/host1"))
        initDefaultSettings();

    loadSettings();

    mProcessor->setDatabase(QSqlDatabase::database(connectionName()));

    processor = new MessageProcessor(QSqlDatabase::database(connectionName()), mProcessor, this);
    processor->setSkipServiceMessages(mSkipServiceMessages);
    processor->setAnswerOnError(mAnswerOnError);
    processor->setAftnServerName(mClient->aftnServerName());

    mClient->setTextCodec(codec);
    mClient->setProperty("lastServer", 2);
    timerId = startTimer(interval * 1000);
    //qDebug() << interval << timerId;

    timerIdAutoSent = startTimer(TIMERAUTOSENT);

    //timerId2 = startTimer(1000);
    mClient->disconnectFromHost();

    if(QSqlDatabase::database(connectionName()).isOpen())
    {
        DBSettings dbsettings(QSqlDatabase::database(connectionName()), this);
        dbsettings.setValue(PulsarSettings::autoAnswerOnError, mAnswerOnError ? 1 : 0);
        setServerState(true);
    }
    return QSqlDatabase::database(connectionName()).isOpen();
}

void NetworkWorkerBase::stop()
{
    killTimer(timerId);

    setServerState(false);

    checkParameters();

    if (mClient->aftnState() != AFTNClient::AftnUnconnected)
        mClient->disconnectFromHost();

    mProcessor->setDatabase(QSqlDatabase());
}

void NetworkWorkerBase::processMessages(const QString &nonsplittedmessage)
{
    qlogdebug(tr("Incoming network message: %1").arg(nonsplittedmessage));

    if (processor)
        processor->processNetworkMessages(nonsplittedmessage);
}

void NetworkWorkerBase::clientStateChanged(AFTNClient::AftnState state)
{
    QString message;
    switch (state) {
    case AFTNClient::AftnUnconnected:
        message = tr("AFTN client: unconnected");
        if (telnetAppender)
            telnetAppender->setWelcomeMessage(message);
        emit aftnStatusChanged(false);
        break;
    case AFTNClient::AftnAuthorizating:
        message = tr("AFTN client: authorizating");
        break;
    case AFTNClient::AftnHostLookup:
        message = tr("AFTN client: host lookup");
        break;
    case AFTNClient::AftnConnecting:
        message = tr("AFTN client: connecting...");
        break;
    case AFTNClient::AftnConnected:
        message = tr("AFTN client: connected");
        if (telnetAppender)
            telnetAppender->setWelcomeMessage(message);
        emit aftnStatusChanged(true);
        break;
    case AFTNClient::AftnWaitForAuthorizationInfo:
        message = tr("AFTN client: wating for authorization");
        break;
    case AFTNClient::AftnReceiving:
        message = tr("AFTN client: receiving");
        break;
    case AFTNClient::AftnInterrupted:
        message = tr("AFTN client: interrupted");
        break;
    case AFTNClient::AftnClosing:
        message = tr("AFTN client: closing");
        break;
    }

    //    qloginfo(message);
}

void NetworkWorkerBase::cksStateChanged(bool isConnected)
{
    if(QSqlDatabase::database(connectionName()).isOpen())
    {
        DBSettings dbsettings(QSqlDatabase::database(connectionName()), this);
        dbsettings.setValue(PulsarSettings::cksIsConnected, isConnected);
    }
}

void NetworkWorkerBase::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == timerId)
    {
        checkParameters();
        if (mClient->state() == QAbstractSocket::UnconnectedState) {
            cksStateChanged(false);
            if (pulsar)
            {
                if (attempt == 300)
                {
                    attempt = 0;
                    pulsar->deactivate();
                } else {
#warning("FOR TASHKENT")
                    //++attempt;
                }
            }

            if (mClient->property("lastServer").toInt() == 2) {
//                qlogwarn(tr("Connecting to host %1").arg(mHost1));
                mClient->setAftnName(mAftnName);
                mClient->setAftnServerName(mAftnNameServer);
                mClient->setChannelId(mChannelId);
                mClient->setPingTimeout(mPingTimeout);
                mClient->connectToHost(mHost1, mPort1);
                mClient->setProperty("lastServer", 1);
            } else {
//                qlogwarn(tr("Connecting to host %1").arg(mHost2));
                mClient->setAftnName(mAftnName);
                mClient->setAftnServerName(mAftnNameServer);
                mClient->setChannelId(mChannelId);
                mClient->setPingTimeout(mPingTimeout);
                mClient->connectToHost(mHost2, mPort2);
                mClient->setProperty("lastServer", 2);
            }
            if (processor)
                processor->setAftnServerName(mClient->aftnServerName());
        }
        return;
    } else if (event->timerId() == timerIdAutoSent && isDBConnectionValid()) {
        staticMetaObject.invokeMethod(this, "autoSentTelegrams", Qt::QueuedConnection);
        staticMetaObject.invokeMethod(this, "checkChannelId", Qt::QueuedConnection);
    }

    WorkerBase::timerEvent(event);
}

void NetworkWorkerBase::mayBeSend(AFTNClientMessage& message)
{
    if ( !isDBConnectionValid() )
        return;

    {
        QtAutoTransaction transaction(QSqlDatabase::database(connectionName()));
        QSqlQuery delAftnQuery(QSqlDatabase::database(connectionName()));
        delAftnQuery.prepare("DELETE FROM outputtelegrams WHERE created <= systimestamp - interval '1' day");
        if(!delAftnQuery.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(delAftnQuery.lastQuery(), delAftnQuery.lastError().text()));
        }
        else
        {
            transaction.commit();
        }
    }
    QString querySelect(QString("SELECT id, upper(text), upper(header), upper(message), upper(addresses), "
                                "upper(type), upper(fill_time) "
                        "FROM outputtelegrams "
                         "WHERE created < (SYS_EXTRACT_UTC(SYSTIMESTAMP - interval '%1' second)) "
                                "OR header LIKE '%SS %' OR send_now = 1 ORDER BY id")
            .arg(QString::number(mTelegramDelay)));
    QSqlQuery mesAftnQuery(QSqlDatabase::database(connectionName()));
    mesAftnQuery.prepare(querySelect);
    if(!mesAftnQuery.exec())
    {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                 .arg(mesAftnQuery.lastQuery(), mesAftnQuery.lastError().text()));
    }
    else
    {
        if(mesAftnQuery.next())
        {
            message.setDBID(mesAftnQuery.value(0).toInt());
            message.setText(mesAftnQuery.value(1).toString());
            message.setHeader(mesAftnQuery.value(2).toString());
            message.setMessage(mesAftnQuery.value(3).toString());
            message.setAddresses(mesAftnQuery.value(4).toString());
            mType = mesAftnQuery.value(5).toString();
            mFillTime = mesAftnQuery.value(6).toString();
        }
    }
}

void NetworkWorkerBase::checkParameters()
{
    if(isDBConnectionValid())
    {
        DBSettings dbsettings(QSqlDatabase::database(connectionName()), this);
        QVariant val = dbsettings.value(PulsarSettings::autoAnswerOnError);
        if(!val.isNull())
        {
            bool bVal = val.toInt() == 1;
            if(bVal != mAnswerOnError)
            {
                mAnswerOnError = bVal;
                settings()->setValue("tcp_ip_connector/answer_on_error", mAnswerOnError);
                if (processor)
                    processor->setAnswerOnError(mAnswerOnError);
            }
        }
    }
}

void NetworkWorkerBase::OnSendedMessage(const AFTNClientMessage& message, const QString &serialNumber)
{
    mSerialNumber = serialNumber;

    if(message.getDBID() && isDBConnectionValid())
    {
        QtAutoTransaction transaction(QSqlDatabase::database(connectionName()));
        QSqlQuery updateAftnQuery(QSqlDatabase::database(connectionName()));
        updateAftnQuery.prepare("UPDATE outputtelegrams SET header = :2, message = :3 WHERE id = :1");
        updateAftnQuery.bindValue(":1", message.getDBID());
        updateAftnQuery.bindValue(":2", message.header());
        updateAftnQuery.bindValue(":3", message.message());
        if(!updateAftnQuery.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(updateAftnQuery.lastQuery(), updateAftnQuery.lastError().text()));
        }
        QSqlQuery delAftnQuery(QSqlDatabase::database(connectionName()));
        delAftnQuery.prepare("DELETE FROM outputtelegrams WHERE id = :1");
        delAftnQuery.bindValue(":1", message.getDBID());
        if(!delAftnQuery.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(delAftnQuery.lastQuery(), delAftnQuery.lastError().text()));
        }
        else
        {
            transaction.commit();
        }
    }

#if defined(BAN_BUILD)
    if(isDBConnectionValid())
    {
        QDateTime created = QDateTime::currentDateTimeUtc();
        QtAutoTransaction transaction(QSqlDatabase::database(connectionName()));
        QSqlQuery query(QSqlDatabase::database(connectionName()));
//        query.prepare("INSERT INTO sent_aftn (created, message) "
//                      "VALUES (:created, :message)");
//        query.bindValue(":created", created);
//        query.bindValue(":message", message.message());

        query.prepare("INSERT INTO sent_aftn (id, created, text, header, message, addresses, type, serial_number, fill_time) "
                      "VALUES (DEFAULT, :created, :text, :header, :message, :addresses, :type, :serial_number, :fill_time)");
        query.bindValue(":created", created);
        query.bindValue(":text", message.text());
        query.bindValue(":header", message.header());
        query.bindValue(":message", message.message());
        query.bindValue(":addresses", message.addresses());
        query.bindValue(":type", mType);
        query.bindValue(":serial_number", mSerialNumber);
        query.bindValue(":fill_time", mFillTime);
        if (!query.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(query.lastQuery(), query.lastError().text()));
        }
        else
        {
            transaction.commit();
        }
    }
#endif
}

void NetworkWorkerBase::onCheckNextNum(quint16 &nextNum)
{
    if (isDBConnectionValid())
    {
        DBSettings dbsettings(QSqlDatabase::database(connectionName()), this);
        QVariant val = dbsettings.value(PulsarSettings::telegramNextNumNet);
        if (!val.isNull())
        {
            bool bOk = false;
            quint16 iVal = val.toUInt(&bOk);
            if (bOk)
            {
                nextNum = iVal % 1000;

                val = dbsettings.value(PulsarSettings::telegramNextNumNetDate);
                if (!val.isNull())
                {
                    QDate date = val.toDate();
                    if (date.isValid())
                    {
                        if (date != QDateTime::currentDateTime().date())
                            nextNum = 1;
                    }
                }
            }
        }
        dbsettings.setValue(PulsarSettings::telegramNextNumNetDate, QDateTime::currentDateTime().date());
        dbsettings.setValue(PulsarSettings::telegramNextNumNet, (nextNum + 1) % 1000);
    }
}

bool NetworkWorkerBase::enabledAutoSent()
{
    bool enabled = settings()->value("tcp_ip_connector/auto_sent/enabled", false).toBool();
    return enabled;
}

void NetworkWorkerBase::sentChangeStateChannel(const QString &state)
{
    settings()->beginGroup("tcp_ip_connector");
    settings()->beginGroup("auto_sent");
    QString text = settings()->value(state, "").toString();
    settings()->endGroup();
    settings()->endGroup();

    if (!text.isEmpty())
    {
        QString header;

        header = "ZCZC";
        header += " ";
        header += mClient->channelId() + QString("@@@");
        header += "     ";
        header += "\r\n";
        header += "FF";
        header += " ";
        header += mAftnNameServer;
        header += "\r\n";
        header += QDateTime::currentDateTime().toString("ddhhmm");
        header += " ";
        header += settings()->value("tcp_ip_connector/aftn_name", "XXXXXXXX").toString();

        AFTNClientMessage msg(text, mAftnNameServer, header);

        mClient->sendMessage(msg);
    }
}

void NetworkWorkerBase::autoSentTelegrams()
{
    if (enabledAutoSent())
    {
        DBSettings dbsettings(QSqlDatabase::database(connectionName()), this);
        int state  = dbsettings.value(PulsarSettings::autoSent, PulsarSettings::NONE).toInt();

        switch (state)
        {
        case PulsarSettings::START:
            sentChangeStateChannel("start_channel");
            dbsettings.setValue(PulsarSettings::autoSent, PulsarSettings::NONE);
            break;
        case PulsarSettings::STOP:
            sentChangeStateChannel("stop_channel");
            dbsettings.setValue(PulsarSettings::autoSent, PulsarSettings::NONE);
            break;
        default:
            break;
        }
    }
}

void NetworkWorkerBase::checkChannelId()
{
    DBSettings s(QSqlDatabase::database(connectionName()));
    QString newChannelId = s.value(PulsarSettings::channelId).toString();
    if (!newChannelId.isEmpty())
    {
        settings()->beginGroup("tcp_ip_connector");
        settings()->setValue("aftn_channel_id", newChannelId);
        settings()->endGroup();
        settings()->sync();

        mChannelId = newChannelId;
        mClient->setChannelId(mChannelId);

        s.setValue(PulsarSettings::channelId, QString());
    }
}

void NetworkWorkerBase::setServerState(bool bMain)
{
#if defined(DUSHANBE_BUILD) || defined(TASHKENT_BUILD)
    qloginfo(tr("NetworkWorkerBase::setServerState %1 %2").arg(bMain ? 1 : 0).arg(QSqlDatabase::database(connectionName()).isOpen() ? "true" : "false"));

    if(QSqlDatabase::database(connectionName()).isOpen())
    {
        QtAutoTransaction trFDP(QSqlDatabase::database(connectionName()));
        QSqlQuery sqlquery(QSqlDatabase::database(connectionName()));
        sqlquery.prepare("UPDATE fpl_cfg SET pps = :1");
        sqlquery.bindValue(":1", bMain ? 1 : 0);
        if(!sqlquery.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(sqlquery.lastQuery(), sqlquery.lastError().text()));
        }
        else
        {
            trFDP.commit();
        }
    }
#endif
}
