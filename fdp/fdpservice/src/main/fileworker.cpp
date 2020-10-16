#include <QtDebug>

#include <QtCore/QTextCodec>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "aftnservice.h"
#include "fileworker.h"
#include "messageprocessor.h"
#include "aftnmessageprocessor.h"

#include "catalogorganizer.h"
#include "aftnclient.h"
#include "qtautotransaction.h"

#include "DBSettings.h"
#include "pulsar_fdp_settings.h"

#if defined(DUSHANBE_BUILD)
#include "fpl/fpltan.h"
#include "DBOCI.h"
#endif

FileWorker::FileWorker(QObject *parent) :
        AFTNWorkerBase(parent)
{
    if (objectName().isEmpty())
        setObjectName("file_worker");
}

void FileWorker::run()
{
    FileWorkerBase ptr;
    setWorker(&ptr);

    qloginfo(tr("Processing file"));

    ptr.start();

    exec();

    ptr.stop();
    setWorker(0);

    qloginfo(tr("Stop file processing"));
}

// ------------------------------------------------
FileWorkerBase::FileWorkerBase(QObject* parent) :
    WorkerBase(parent), mProcessor(new AFTNMessageProcessor()), mesNum(0), mTelegramDelay(0), processor(0),
        codecCatalogOrganizer(0), ita2codec(new QITA2TextCodec()), ita5codec(new QKOI7TextCodec()),
        mtk2codec(new MTK2Codec()), mtk5codec(new MTK5Codec())
{
    if (objectName().isEmpty())
        setObjectName("file_worker");
    init();
}

void FileWorkerBase::initDefaultSettings()
{
    qloginfo("Init default settings");
    settings()->beginGroup("aftn_file_worker");
    settings()->setValue("comment",
                         "Настройки модуля парсинга AFTN сообщений из файлов.");
    settings()->setValue("timer_interval", 5);
    settings()->setValue("timer_interval/comment",
                         "Таймер в СЕКУНДАХ для чтения директории.");
    settings()->setValue("incoming_dir", "./messages");
    settings()->setValue("incoming_dir/comment", "Директория входящих сообщений.");
    settings()->setValue("processed_dir", "./messages_done");
    settings()->setValue("processed_dir/comment",
                         "Директория обработанных сообщений.");
    settings()->setValue("file_ext", "lod");
    settings()->setValue("file_ext/comment", "Расширение файлов с сообщениями. Deprecated");
    settings()->setValue("file_pattern", "*R.*");
    settings()->setValue("file_pattern/comment", "Маска файлов с сообщениями.");
    settings()->setValue("codec", "cp1251");
    settings()->setValue("codec/comment", "Кодировка файлов cp1251, ibm866, etc.");
    settings()->setValue("filter", "(\\(\\(PPL|\\(RPL)");
    settings()->setValue("filter/comment", "Фильтрация сообщений по регулярному выражению.");
    settings()->setValue("telegram_delay", 10);
    settings()->setValue("telegram_delay/comment", "Задержка отправки телеграмм (в секундах).");

    settings()->beginGroup("aftn_message_processor");
    settings()->setValue("comment", "Настройки модуля парсинга AFTN сообщений.");
    settings()->setValue("fir_name", "UMMV");
    settings()->setValue("fir_name/comment", "FIR");
    settings()->setValue("cut_fir", true);
    settings()->setValue("cut_fir/comment", "Вырезать FIR из маршрута.");
    settings()->setValue("validate_route", true);
    settings()->setValue("validate_route/comment",
                         "Проверять корректность маршрута.");
    settings()->setValue("validate_route_lite", false);
    settings()->setValue("validate_route_lite/comment",
                         "Проверять корректность маршрута (слабая проверка).");
    settings()->setValue("FPL2012", false);
    settings()->setValue("FPL2012/comment", "Формат сообщений FPL2012 или FPL2007.");
    settings()->endGroup();

    settings()->beginGroup("aftn_catalog");
    settings()->setValue("comment", "Настройки AFTN каталога.");
    settings()->setValue("disable", true);
    settings()->setValue("disable/comment", "Отключить.");
    settings()->setValue("incoming", "./incoming");
    settings()->setValue("incoming/comment", "Директория для входящих.");
    settings()->setValue("outgoing", "./outgoing");
    settings()->setValue("outgoing/comment", "Директория для исходящих.");
    settings()->setValue("channelId", "000");
    settings()->setValue("channelId/comment", "Идентификатор канала (3 символа).");
    settings()->setValue("aftnName", "XXXXXXXX");
    settings()->setValue("aftnName/comment", "Имя абонента.");
    settings()->setValue("codec", "cp866");
    settings()->setValue("codec/comment", "Кодировка сообщений.");
    settings()->setValue("file_pattern", "PRI*R.*");
    settings()->setValue("file_pattern/comment", "Маска файлов с сообщениями.");
    settings()->endGroup();

    settings()->endGroup();
}

void FileWorkerBase::loadSettings()
{
    qloginfo("Load settings : File");
    settings()->beginGroup("aftn_file_worker");

    interval = settings()->value("timer_interval").toInt();
//    messagesDir.setPath(QCoreApplication::applicationDirPath()
//                        + QDir::separator() + settings()->value("incoming_dir").toString());
//    messagesDoneDir.setPath(QCoreApplication::applicationDirPath()
//                            + QDir::separator() + settings()->value("processed_dir").toString());
    messagesDir.setPath(settings()->value("incoming_dir").toString());
    messagesDoneDir.setPath(settings()->value("processed_dir").toString());
    fileExt = settings()->value("file_pattern").toString();

    filter = settings()->value("filter").toString();
    codec = settings()->value("codec").toString().toLatin1();

    mTelegramDelay = settings()->value("telegram_delay", 0).toInt();

    settings()->beginGroup("aftn_message_processor");
    mProcessor->setFIRName(settings()->value("fir_name").toString());
    mProcessor->setSaveOnlyFIR(settings()->value("cut_fir").toBool());
    mProcessor->setValidateRoute(settings()->value("validate_route").toBool());
    mProcessor->setValidateRouteLite(settings()->value("validate_route_lite", false).toBool());
    mProcessor->setStandard(settings()->value("FPL2012").toBool() ? AFTNMessageProcessor::FPL_2012 : AFTNMessageProcessor::FPL_2007);
    settings()->endGroup();

    QString codecName = settings()->value("aftn_catalog/codec", "ITA2").toString();

    // FIXME Workaround as static text coder
    if (codecName.toUpper() == "ITA2") {
        codecCatalogOrganizer = ita2codec.data();
    } else if (codecName.toUpper() == "ITA5") {
        codecCatalogOrganizer = ita5codec.data();
    } else if (codecName.toUpper() == "MTK2") {
        codecCatalogOrganizer = mtk2codec.data();
    } else if (codecName.toUpper() == "MTK5") {
        codecCatalogOrganizer = mtk5codec.data();
    } else {
        codecCatalogOrganizer = QTextCodec::codecForName(codecName.toLatin1());
    }
    encoderCatalogOrganizer.reset(codecCatalogOrganizer->makeEncoder());
    decoderCatalogOrganizer.reset(codecCatalogOrganizer->makeDecoder());

    settings()->endGroup();
}

bool FileWorkerBase::start()
{
    if (!settings()->contains("aftn_file_worker/incoming_dir"))
        initDefaultSettings();

    loadSettings();

    mProcessor->setDatabase(QSqlDatabase::database(connectionName()));

    processor = new MessageProcessor(QSqlDatabase::database(connectionName()), mProcessor.data(), this);
    processor->setFilter(filter);
    processor->setSkipServiceMessages(false);

    timerId = startTimer(interval * 1000);
    return QSqlDatabase::database(connectionName()).isOpen();
}

void FileWorkerBase::stop()
{
    killTimer(timerId);
}

void FileWorkerBase::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == timerId && isDBConnectionValid()) {
        //    qloginfo(tr("Process file messages"));
        for(int i = 0; i < 2; ++i)
        {
            QFileInfoList fileInfoList;
            if(i == 0)
            {
                if(!settings()->value("aftn_file_worker/aftn_catalog/disable", true).toBool())
                {
                    sendCatalogOrganizer();

                    CatalogOrganizer corg;
                    corg.SetPaths(settings()->value("aftn_file_worker/aftn_catalog/incoming", "./incoming").toString(), settings()->value("aftn_file_worker/aftn_catalog/outgoing", "./outgoing").toString());
                    corg.SetCheck(true);
                    QDir corgDir;
                    corgDir.setPath(corg.GetIncomingPath());
                    fileInfoList = corgDir.entryInfoList(QStringList(settings()->value("aftn_file_worker/aftn_catalog/file_pattern", "PRI*R.*").toString()), QDir::Files, QDir::Name);
                }
                else
                {
                    ++i;
                }
            }
            if(i == 1)
            {
                if (!messagesDir.exists()) {
                    // There is no messages
                    return;
                }
                fileInfoList = messagesDir.entryInfoList(QStringList(fileExt), QDir::Files, QDir::Name);
            }

            QTextStream in;
            if(i == 0)
                in.setCodec(codecCatalogOrganizer);
            if(i == 1)
                in.setCodec(codec);
            QString messageArray;
            QString newFileName;

            foreach(const QFileInfo& fileInfo, fileInfoList)
            {
                QFile file(fileInfo.absoluteFilePath());
                QString fileName(fileInfo.fileName());

                qloginfo(tr("Processing %1").arg(fileName));

                if (!file.open(QIODevice::ReadOnly)) {
                    qlogwarn(tr("Could not open file %1").arg(fileName));
                    continue;
                }

                in.setDevice(&file);
                if(i == 0)
                    in.setCodec(codecCatalogOrganizer);
                if(i == 1)
                    in.setCodec(codec);
                messageArray = in.readAll();
                //messageArray = messageArray.simplified();
                file.close();

                if (processor)
                    processor->processFileMessages(messageArray);

                newFileName = messagesDoneDir.filePath(
                        QDateTime::currentDateTime().toUTC().toString(
                                "yyyy-MM-dd hh.mm.ss.zzz") + '-' + fileName);
                if (!messagesDoneDir.exists() && !messagesDoneDir.mkpath(
                        messagesDoneDir.absolutePath())) {
                    break;
                }

                if (!file.rename(newFileName)) {
                    qlogwarn(tr("Could not rename file: %1").arg(newFileName));
                    break;
                }
            }
        }
        return;
    }
    WorkerBase::timerEvent(event);
}

void FileWorkerBase::sendCatalogOrganizer()
{
    AFTNClientMessage message;
    mayBeSend(message);
    setActive(true);
    sendMessage(message);
    writeSentAftn(message);
    setActive(true);
}

void FileWorkerBase::mayBeSend(AFTNClientMessage& message)
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

void FileWorkerBase::writeSentAftn(const AFTNClientMessage &message)
{
    QDateTime created = QDateTime::currentDateTimeUtc();
    QString text = message.text();
    if (text.isEmpty())
        return;
    QString header = message.header();
    QString fillTime = mFillTime;
    if (header.isEmpty())
    {
        header = "ZCZC";
        header += " ";
        header += channelId() + mesNumToString();
        header += "     ";
        header += "\r\n";
        header += "FF";
        header += " ";
        header += message.addresses();
        header += "\r\n";
        fillTime = sendTime.toString("ddhhmm");
        header += fillTime;
        header += " ";
        header += aftnName();
    }
    header += "\r\n";
    QString msg = header + text + "\r\n\n\n\n\n\n\n\nNNNN";
    QString serialNumber = mesNumToString();
    msg.replace("@@@", serialNumber);
    msg.replace("@SEND_TIME@", created.toString("hhmm"));

    if(message.getDBID() && isDBConnectionValid())
    {
        QtAutoTransaction transaction(QSqlDatabase::database(connectionName()));
        QSqlQuery updateAftnQuery(QSqlDatabase::database(connectionName()));
        updateAftnQuery.prepare("UPDATE outputtelegrams SET header = :2, message = :3 WHERE id = :1");
        updateAftnQuery.bindValue(":1", message.getDBID());
        updateAftnQuery.bindValue(":2", header);
        updateAftnQuery.bindValue(":3", msg);
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
    QSqlQuery query(QSqlDatabase::database(connectionName()));
    query.prepare("INSERT INTO sent_aftn (id, created, text, header, message, addresses, type, serial_number, fill_time) "
                  "VALUES (DEFAULT, :created, :text, :header, :message, :addresses, :type, :serial_number, :fill_time)");
    query.bindValue(":created", created);
    query.bindValue(":text", text);
    query.bindValue(":header", header);
    query.bindValue(":message", msg);
    query.bindValue(":addresses", message.addresses());
    query.bindValue(":type", mType);
    query.bindValue(":serial_number", serialNumber);
    query.bindValue(":fill_time", fillTime);
    if (!query.exec())
    {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                 .arg(query.lastQuery(), query.lastError().text()));
    }
#endif
}
/*
QByteArray FileWorkerBase::toByteArray(const AFTNClientMessage& message) const
{
    QString sRet;
    if(!message.message().isEmpty())
    {
        sRet = message.message();
    }
    else if(!message.header().isEmpty())
    {
        sRet = message.header();
        sRet.replace("@@@", mesNumToString());
        sRet.replace("@SEND_TIME@", QDateTime::currentDateTimeUtc().toString("hhmm"));
        sRet += "\n\r";
        sRet += message.text();
        sRet += "\n\n\r";
        sRet += "\r\r\r\r\r\r\r";
        sRet += "NNNN";
    }
    else if(!message.text().isEmpty())
    {
        QDateTime time = QDateTime::currentDateTimeUtc();

        sRet = "ZCZC";
        sRet += " ";
        sRet += channelId() + mesNumToString();
        sRet += "     ";
        sRet += "\n\n\r";
        sRet += "FF";
        sRet += " ";
        sRet += message.addresses();
        sRet += "\n\n\r";
        sRet += time.toString("ddhhmm");
        sRet += " ";
        sRet += aftnName();
        sRet += "\n\n\r";
        sRet += message.text();
        sRet += "\n\n\r";
        sRet += "\r\r\r\r\r\r\r";
        sRet += "NNNN";
    }
    QByteArray ret = encoderCatalogOrganizer->fromUnicode(sRet);
    //ret.prepend((char)0);
    return ret;
}
*/
QByteArray FileWorkerBase::toByteArray(const AFTNClientMessage& message) const
{
    QString sRet;
    if(!message.message().isEmpty())
    {
        sRet = message.message();
    }
    else if(!message.header().isEmpty())
    {
        sRet = message.header();
        sRet.replace("@@@", mesNumToString());
        sRet.replace("@SEND_TIME@", QDateTime::currentDateTimeUtc().toString("hhmm"));
        sRet += "\r\n";
        sRet += message.text();
        sRet += "\r\n";
        sRet += "\n\n\n\n\n\n\n";
        sRet += "NNNN";
    }
    else if(!message.text().isEmpty())
    {
        sendTime = QDateTime::currentDateTimeUtc();

        sRet = "ZCZC";
        sRet += " ";
        sRet += channelId() + mesNumToString();
        sRet += "     ";
        sRet += "\r\n";
        sRet += "FF";
        sRet += " ";
        sRet += message.addresses();
        sRet += "\r\n";
        sRet += sendTime.toString("ddhhmm");
        sRet += " ";
        sRet += aftnName();
        sRet += "\r\n";
        sRet += message.text();
        sRet += "\r\n";
        sRet += "\n\n\n\n\n\n\n";
        sRet += "NNNN";
    }
    QByteArray ret = encoderCatalogOrganizer->fromUnicode(sRet);
    //ret.prepend((char)0);
    return ret;
}


void FileWorkerBase::sendMessage(const AFTNClientMessage& message)
{
    if(!message.isEmpty())
    {
        CatalogOrganizer corg;
        corg.SetPaths(settings()->value("aftn_file_worker/aftn_catalog/incoming", "./incoming").toString(), settings()->value("aftn_file_worker/aftn_catalog/outgoing", "./outgoing").toString());
        corg.SetCheck(true);
        QString sPath = corg.GetOutgoingPath();
        if(!sPath.isEmpty())
        {
            QFile file(sPath + QDateTime::currentDateTime().toString("hhmmss") + "." + mesNumToString());
            if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                nextMesNum();
                QByteArray msg = toByteArray(message);
                qDebug() << "Send message\n" << decoderCatalogOrganizer->toUnicode(msg);
//                QDataStream ds(&file);
//                ds << msg;
                file.write(msg);
                file.close();
                QString newFileName = sPath + QString("TLG%1P.%2").arg(QDateTime::currentDateTime().toString("MMdd")).arg(mesNumToString());
                if(file.rename(newFileName))
                {
                    qloginfo(tr("Write file '%1'.").arg(newFileName));
                }
                else
                {
                    qlogwarn(tr("Could not rename file '%1'.").arg(newFileName));
                }
            }
            else
            {
                qlogwarn(tr("Could not create file '%1'.").arg(file.fileName()));
            }
        }
        else
        {
            qlogwarn(tr("Could not create outgoing path."));
        }
    }
}

quint32 FileWorkerBase::nextMesNum()
{
    ++mesNum %= 1000;
    onCheckNextNum(mesNum);
    return mesNum;
}

QString FileWorkerBase::mesNumToString() const
{
    return QString("%1").arg(mesNum, 3, 10, QLatin1Char('0'));
}

QString FileWorkerBase::channelId() const
{
    return settings()->value("aftn_file_worker/aftn_catalog/channelId", "000").toString();
}

QString FileWorkerBase::aftnName() const
{
    return settings()->value("aftn_file_worker/aftn_catalog/aftnName", "XXXXXXXX").toString();
}

void FileWorkerBase::onCheckNextNum(quint16 &nextNum)
{
    if (isDBConnectionValid())
    {
        DBSettings dbsettings(QSqlDatabase::database(connectionName()), this);
        QVariant val = dbsettings.value(PulsarSettings::telegramNextNumFile);
        if (!val.isNull())
        {
            bool bOk = false;
            quint16 iVal = val.toUInt(&bOk);
            if (bOk)
            {
                nextNum = iVal % 1000;

                val = dbsettings.value(PulsarSettings::telegramNextNumFileDate);
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
        dbsettings.setValue(PulsarSettings::telegramNextNumFileDate, QDateTime::currentDateTime().date());
        dbsettings.setValue(PulsarSettings::telegramNextNumFile, (nextNum + 1) % 1000);
    }
}
