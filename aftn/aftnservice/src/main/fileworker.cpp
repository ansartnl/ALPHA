#include <QtDebug>

#include <QtCore/QDateTime>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "aftnservice.h"
#include "fileworker.h"
#include "aftnmessageprocessor.h"

FileWorker::FileWorker(QObject *parent) :
  AFTNWorkerBase(parent)
{
  if (objectName().isEmpty())
    setObjectName("file_worker");
}

void FileWorker::run()
{
  FileWorkerBase ptr;
  qloginfo(tr("Processing file"));
  if (!ptr.start())
    return;
  exec();
  ptr.stop();
  qloginfo(tr("Stop file processing"));
}

// ------------------------------------------------
FileWorkerBase::FileWorkerBase(QObject* parent) :
  WorkerBase(parent), mProcessor(new AFTNMessageProcessor())
{
  init();
}

FileWorkerBase::~FileWorkerBase()
{
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
  settings()->setValue("incoming_dir", "messages");
  settings()->setValue("incoming_dir/comment", "Директория входящих сообщений.");
  settings()->setValue("processed_dir", "messages_done");
  settings()->setValue("processed_dir/comment",
    "Директория обработанных сообщений.");
  settings()->setValue("file_ext", "lod");
  settings()->setValue("file_ext/comment", "Расширение файлов с сообщениями.");

  settings()->beginGroup("aftn_message_processor");
  settings()->setValue("comment", "Настройки модуля парсинга AFTN сообщений.");
  settings()->setValue("fir_name", "UMMV");
  settings()->setValue("fir_name/comment", "FIR");
  settings()->setValue("cut_fir", true);
  settings()->setValue("cut_fir/comment", "Вырезать FIR из маршрута.");
  settings()->setValue("validate_route", true);
  settings()->setValue("validate_route/comment",
    "Проверять корректность маршрута.");
  settings()->endGroup();

  settings()->endGroup();
}

void FileWorkerBase::loadSettings()
{
  qloginfo("Load settings");
  settings()->beginGroup("aftn_file_worker");

  interval = settings()->value("timer_interval").toInt();
  messagesDir.setPath(QCoreApplication::applicationDirPath()
      + QDir::separator() + settings()->value("incoming_dir").toString());
  messagesDoneDir.setPath(QCoreApplication::applicationDirPath()
      + QDir::separator() + settings()->value("processed_dir").toString());
  fileExt = settings()->value("file_ext").toString();

  settings()->beginGroup("aftn_message_processor");
  mProcessor->setFIRName(settings()->value("fir_name").toString());
  mProcessor->setSaveOnlyFIR(settings()->value("cut_fir").toBool());
  mProcessor->setValidateRoute(settings()->value("validate_route").toBool());
  settings()->endGroup();

  settings()->endGroup();
}

bool FileWorkerBase::start()
{
  if (!settings()->contains("aftn_file_worker/incoming_dir"))
    initDefaultSettings();

  loadSettings();

  mProcessor->setDatabase(QSqlDatabase::database(connectionName()));
  timerId = startTimer(interval * 1000);
  return QSqlDatabase::database(connectionName()).isOpen();
}

void FileWorkerBase::stop()
{
  killTimer(timerId);
}

void FileWorkerBase::timerEvent(QTimerEvent* event)
{
  if (event->timerId() == timerId) {
    //    qloginfo(tr("Process file messages"));
    if (!messagesDir.exists()) {
      // There is no messages
      return;
    }

    QFileInfoList fileInfoList = messagesDir.entryInfoList(QStringList("*."
        + fileExt), QDir::Files, QDir::Name);

    QSqlQuery arcAftnQuery(TBL_ARC_AFTN_INSERT, QSqlDatabase::database(
      connectionName()));
    QSqlQuery manAftnQuery(TBL_MAN_AFTN_INSERT, QSqlDatabase::database(
      connectionName()));

    foreach(const QFileInfo& fileInfo, fileInfoList)
      {
        QFile file(fileInfo.absoluteFilePath());
        QString fileName(fileInfo.fileName());

        qloginfo(tr("Processing %1").arg(fileName));

        if (!file.open(QIODevice::ReadOnly)) {
          qlogwarn(tr("Could not open file %1").arg(fileName));
          continue;
        }

        QTextStream in(&file);
        int lineIndex = 0;
        QString buffer;

        while (!in.atEnd()) {

          buffer += in.readLine();
          ++lineIndex;

          QRegExp rexp("ZCZC([^N][^N][^N][^N]*)NNNN");
          buffer = buffer.simplified();
          QStringList messages;
          int pos = 0;
          int lastPos = -1;
          while ((pos = rexp.indexIn(buffer, pos)) != -1) {
            messages << rexp.cap(1);
            pos += rexp.matchedLength();
            lastPos = pos;
          }

          if (lastPos > 0) {
            buffer = buffer.mid(lastPos);
          } else {
            messages << buffer;
            buffer.clear();
          }

          Q_FOREACH (QString message, messages)
            {
              message.replace(MESSAGE_CONCAT_SYMBOL, MESSAGE_CONCAT_REPLACE);

              if (message.isEmpty())
                continue;

              int messageBeginIndex = message.indexOf(MESSAGE_BEGIN);
              if (messageBeginIndex == -1)
                // Message not found
                continue;

              QString header;
              header = message.left(messageBeginIndex - 1);

              int messageEndIndex = message.lastIndexOf(MESSAGE_END);
              if (messageEndIndex == -1 || messageBeginIndex > messageEndIndex)
                // Invalid message format
                continue;

              message = message.mid(messageBeginIndex, messageEndIndex
                  - messageBeginIndex + 1);
              arcAftnQuery.addBindValue(message);
              arcAftnQuery.addBindValue(header);

              if (!arcAftnQuery.exec()) {
                qlogwarn(tr("Query execution error. Executed query: %1. Error: %2")
                    .arg(arcAftnQuery.lastQuery(), arcAftnQuery.lastError().text()));
                break;
              }

              AFTNMessageProcessor::AftnMessageErrors errorCodes;
              QVariantMap messageMap = mProcessor->processMessage(message,
                errorCodes);
              if (errorCodes.isEmpty()) {
                mProcessor->insertMessage(messageMap, errorCodes);

                if (!errorCodes.isEmpty()) {
                  // TODO send to some output
                }
              }

              if (!errorCodes.isEmpty()) {
                QString warningLine;
                QStringList errors = mProcessor->errorStrings(errorCodes);
                warningLine
                    = tr(
                      "AFTNMessage::processMessage failed (line %1) with errors %2").arg(
                      lineIndex).arg(errors.join("\n"));
                qlogwarn(warningLine);

                manAftnQuery.addBindValue(message);
                manAftnQuery.addBindValue(header);
                if (!manAftnQuery.exec()) {
                  qlogwarn(tr("Query execution error. Executed query: %1. Error: %2")
                      .arg(manAftnQuery.lastQuery(), manAftnQuery.lastError().text()));
                  break;
                }
              }
            }
        }

        file.close();

        QString newFileName = messagesDoneDir.filePath(
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
    return;
  }
  WorkerBase::timerEvent(event);
}
