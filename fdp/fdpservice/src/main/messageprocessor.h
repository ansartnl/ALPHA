#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include "aftnmessageprocessor.h"
#include "AftnTelegram.h"

#include <QtCore/QObject>
#include <QtCore/QVariantMap>

#include <QtSql/QSqlDatabase>

class AFTNMessageProcessor;
class Telegram;

class QSettings;

struct AftnData : public AftnTelegram
{
    QString type;
    QString text;
    QString header;
    QString message;

    AftnData(const AftnTelegram &aftn, const QString &t, const QString &w, const QString &h, const QString &m)
        : AftnTelegram(aftn)
    {
        type = t;
        text = w;
        header = h;
        message = m;
    }
};

class MessageProcessor : public QObject
{
    Q_OBJECT
public:
    explicit MessageProcessor(const QSqlDatabase &db, AFTNMessageProcessor *processor, QObject *parent = 0);

    void setSkipServiceMessages(bool skip);
    void setAnswerOnError(bool answer);
    void setAftnServerName(const QString &name);

    void setFilter(const QString &filter);

    void processNetworkMessages(const QString &nonsplittedmessage);
    void processFileMessages(const QString &nonsplittedmessage);

protected:
    bool needSkip(Telegram *pTel);

    bool processMessage(const QString &telegram, const QString &header, const QString &message, bool answer = false);

    void addAddress(const QString &address);
    void messageToTAN(const QString& sHeader, const QString& sMessage, const QVariantMap& map, bool bError);
    void answerOnError(const QString& header, const QString& message, const AFTNMessageProcessor::AftnMessageErrors& errorCodes);
    void checkParameters();

    bool writeArchive(const QString &header, const QString &message);
    bool writeManual(const AftnData &aftnData);
    bool writeReceived(const AftnData &aftnData);

    QSettings* settings() {return mSettings;}
    const QSettings* settings() const {return mSettings;}

protected:
    QSqlDatabase mDb;
    QSettings *mSettings;

    bool mSkipServiceMessages;
    bool mAnswerOnError;
    QString mAftnServerName;

    QString mFilter;

    AFTNMessageProcessor *mProcessor;

    QString wholeMessage;
};

#endif // MESSAGEPROCESSOR_H
