#include "MoxaSettingsPrivate.h"

#include <QtCore/QRegExp>
#include <QtCore/QStringList>

#include <QtNetwork/QTcpSocket>

MoxaSettingsPrivate::MoxaSettingsPrivate(QObject *parent) :
    QObject(0), mPort(0)
{
    setObjectName("MoxaSettingsPrivate");

    infoMap(true);

    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(deleteLater()));

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), SLOT(onConnected()));
    connect(socket, SIGNAL(disconnected()), SLOT(onDisconnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(onReadyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onError()));
}

void MoxaSettingsPrivate::setHost(const QString &h)
{
    mHost = h;
}

void MoxaSettingsPrivate::setPort(quint16 p)
{
    mPort = p;
}

void MoxaSettingsPrivate::getValue(const QString &path, const QString &leave, const QString &rexp, int cap)
{
    emit debug(Q_FUNC_INFO);
    emit debug(QString("path = '%1'; leave = '%2'; rexp = '%3'").arg(path).arg(leave).arg(rexp));

    mInfo.path = path;
    mInfo.save.clear();
    mInfo.leave = leave;
    mInfo.rexp = rexp;
    mInfo.cap = cap;

    mSaveIndex = QChar();

    start();
}

void MoxaSettingsPrivate::setValue(const QString &path, const QString &save, const QChar &index)
{
    emit debug(Q_FUNC_INFO);
    emit debug(QString("path = '%1'; save = '%2'; index = '%3'").arg(path).arg(save).arg(index));

    mInfo.path = path;
    mInfo.save = save;
    mInfo.leave.clear();
    mInfo.rexp.clear();
    mInfo.cap = -1;

    mSaveIndex = index;

    start();
}

void MoxaSettingsPrivate::start()
{
    mNextStep = 0;
    mResult.clear();
    if (mPort && !mHost.isEmpty())
    {
        socket->connectToHost(mHost, mPort);
    } else
    {
        if (!mPort)
            emit error(MoxaSettingsTr::tr("Port == 0"));
        else
            emit error(MoxaSettingsTr::tr("Host is empty"));
    }
}

void MoxaSettingsPrivate::process()
{
    emit debug(Q_FUNC_INFO);
    emit debug(mData);
    if (mNextStep < mInfo.path.size())
    {
        stepIn();
    } else if (mNextStep == mInfo.path.size())
    {
        if (mSaveIndex.isNull())
        {
            getData();
            stepOut();
        } else
        {
            setData();
        }
    } else if (mNextStep < (mInfo.path.size() + 1 + mInfo.backPath().size()))
    {
        stepOut();
    } else
    {
        socket->disconnectFromHost();
    }
    mData.clear();
}

void MoxaSettingsPrivate::stepIn()
{
    socket->write(charToData(mInfo.path[mNextStep]));
    ++mNextStep;
}

void MoxaSettingsPrivate::stepOut()
{
    int index = mNextStep - (mInfo.path.size() + 1);
    if (mInfo.backPath().size() > index)
        socket->write(charToData(mInfo.backPath()[index]));
    ++mNextStep;
}

void MoxaSettingsPrivate::getData()
{
    QRegExp rexp(mInfo.rexp);
    rexp.indexIn(mData);
    mResult = rexp.cap(mInfo.cap);
    ++mNextStep;
}

void MoxaSettingsPrivate::setData()
{
    socket->write(charToData(mSaveIndex));
    ++mNextStep;
}

const QByteArray MoxaSettingsPrivate::charToData(const QChar &c) const
{
    if (c == '\033')
        return QString(c).toLocal8Bit();
    return QString(c).toLocal8Bit() + "\r\n";
}

const QString MoxaSettingsPrivate::dataToString(const QByteArray &data) const
{
    QString s;

    foreach (uchar c, data)
    {
        if ((c >= 32 && c < 128) || c == 10 || c == 13)
            s += c;
        else if (c != 0)
            s += " 0x" + QString("%1").arg(c, 2, 16, QChar('0')).toUpper();
    }

    return s;
}

void MoxaSettingsPrivate::onConnected()
{
    emit debug(Q_FUNC_INFO);
}

void MoxaSettingsPrivate::onDisconnected()
{
    emit debug(Q_FUNC_INFO);
    if (mResult.isEmpty())
        emit setFinished();
    else
        emit getFinished(mResult);
}

void MoxaSettingsPrivate::onReadyRead()
{
    emit debug(Q_FUNC_INFO);
    QString data = dataToString(socket->readAll());
//    emit debug(data);
    mData += data;

    bool needProcess = false;
    foreach (const QString &s, endLines())
        if (mData.contains(s))
            needProcess = true;
    if (needProcess)
        process();
}

void MoxaSettingsPrivate::onError()
{
    if (socket->error() != QAbstractSocket::RemoteHostClosedError)
    {
        emit error(socket->errorString());
        socket->abort();
    }
}
