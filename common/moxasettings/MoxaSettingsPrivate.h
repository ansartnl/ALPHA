#ifndef MOXASETTINGSPRIVATE_H
#define MOXASETTINGSPRIVATE_H

#include "MoxaSettingsCommon.h"

class QTcpSocket;

// Внутри много магии, основанной на общении по телнету

class MoxaSettingsPrivate : public QObject
{
    Q_OBJECT
public:
    explicit MoxaSettingsPrivate(QObject *parent = 0);

public slots:
    void setHost(const QString &h);
    void setPort(quint16 p);

    void getValue(const QString &path, const QString &leave, const QString &rexp, int cap);
    void setValue(const QString &path, const QString &save, const QChar &index);

signals:
    void getFinished(const QString &v);
    void setFinished();

    void error(const QString &text);
    void debug(const QString &text);

protected:
    void start();
    void process();
    void stepIn();
    void stepOut();
    void getData();
    void setData();

    const QByteArray charToData(const QChar &c) const;
    const QString dataToString(const QByteArray &data) const;

protected slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError();

protected:
    SettingInfo mInfo;
    QChar mSaveIndex;

    int mNextStep;
    QString mResult;

    QString mHost;
    quint16 mPort;
    QTcpSocket *socket;

    QString mData;
};

#endif // MOXASETTINGSPRIVATE_H
