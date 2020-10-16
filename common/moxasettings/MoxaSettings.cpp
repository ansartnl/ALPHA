#include "MoxaSettings.h"

#include "MoxaSettingsPrivate.h"

#include <QtCore/QThread>

MoxaSettings::MoxaSettings(QObject *parent) :
    QObject(parent)
{
    setObjectName("MoxaSettings");

    d = new MoxaSettingsPrivate(this);
    thread = new QThread(this);
    d->moveToThread(thread);

    connect(d, SIGNAL(error(QString)), SIGNAL(error(QString)));
    connect(d, SIGNAL(debug(QString)), SIGNAL(debug(QString)));

    connect(d, SIGNAL(getFinished(QString)), SLOT(getFinished(QString)));
    connect(d, SIGNAL(setFinished()), SLOT(setFinished()));

    thread->start();
}

MoxaSettings::~MoxaSettings()
{
    thread->quit();
    thread->wait();
}

void MoxaSettings::setHost(const QString &h)
{
    staticMetaObject.invokeMethod(d, "setHost", Qt::QueuedConnection, Q_ARG(QString, h));
}

void MoxaSettings::setPort(quint16 p)
{
    staticMetaObject.invokeMethod(d, "setPort", Qt::QueuedConnection, Q_ARG(quint16, p));
}

void MoxaSettings::getBaudRate()
{
    mRequestList.enqueue(Request::get(Field::F_BAUD_RATE));
    if (mCurrentRequest.field == Field::F_NONE)
        processNextRequest();
}

void MoxaSettings::setBaudRate(int rate)
{
    mRequestList.enqueue(Request::set(Field::F_BAUD_RATE, baudRateIndex(rate)));
    if (mCurrentRequest.field == Field::F_NONE)
        processNextRequest();
}

void MoxaSettings::processNextRequest()
{
    if (mRequestList.isEmpty())
    {
        mCurrentRequest.clear();
        return;
    }
    mCurrentRequest = mRequestList.dequeue();
    if (infoMap().contains(mCurrentRequest.field))
    {
        SettingInfo info = infoMap()[mCurrentRequest.field];
        if (mCurrentRequest.isSetRequest)
        {
            staticMetaObject.invokeMethod(d, "setValue", Qt::QueuedConnection
                                          , Q_ARG(QString, info.path)
                                          , Q_ARG(QString, info.save)
                                          , Q_ARG(QChar, mCurrentRequest.index));
        } else
        {
            staticMetaObject.invokeMethod(d, "getValue", Qt::QueuedConnection
                                          , Q_ARG(QString, info.path)
                                          , Q_ARG(QString, info.leave)
                                          , Q_ARG(QString, info.rexp)
                                          , Q_ARG(int, info.cap));
        }
    } else
    {
        emit error(MoxaSettingsTr::tr("Could not find information about field %1")
                   .arg(Field::toString(mCurrentRequest.field)));
        processNextRequest();
    }

}

void MoxaSettings::getFinished(const QString &v)
{
    switch (mCurrentRequest.field)
    {
    case Field::F_NONE:
        emit error(MoxaSettingsTr::tr("Unknown get finish came: %1").arg(v));
        break;
    case Field::F_BAUD_RATE:
        emit baudRate(v.toInt());
        break;
    }
    processNextRequest();
}

void MoxaSettings::setFinished()
{
    switch (mCurrentRequest.field)
    {
    case Field::F_NONE:
        emit error(MoxaSettingsTr::tr("Unknown set finish came: %1"));
        break;
    case Field::F_BAUD_RATE:
        emit baudRateSet();
        break;
    }
    processNextRequest();
}
