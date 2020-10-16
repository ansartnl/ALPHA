#include "RunwaySyncManager.h"

#include "main.h"
#include "vpp.h"

static int i0 = qRegisterMetaTypeStreamOperators < Runway >();

QDataStream &operator<<(QDataStream &out, const Runway &runway)
{
    out << runway.numPeople << runway.numAuto << runway.numOther;
    return out;
}

QDataStream &operator>>(QDataStream &in, Runway &runway)
{
    in >> runway.numPeople >> runway.numAuto >> runway.numOther;
    return in;
}

RunwaySyncManager::RunwaySyncManager(QObject *parent) :
    QObject(parent), mUpdateTimer(this)
{
    mHost = networkSettings()->value("simulator/runway/ip", "127.0.0.1").toString();
    mPort = networkSettings()->value("simulator/runway/port", "3416").toInt();

    connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));
    mUpdateTimer.setSingleShot(false);

    mDatagramProto = new QtTcpDatagramProtocol(new QTcpSocket(this), this);
}

void RunwaySyncManager::onUpdate()
{
    mDatagramProto->socket()->connectToHost(mHost, mPort);
    QHash<QString, TData> income;
    //bool isUpdated(false);

    if(mDatagramProto->socket()->waitForConnected(1000))
    {
        if(mDatagramProto->socket()->waitForReadyRead(1000))
        {
            QByteArray arr;
            arr = mDatagramProto->nextDatagram();
            QDataStream stream(&arr, QIODevice::ReadOnly);

            stream >> income;

            flushPendingChanges(&income[mAirport]);

            //isUpdated = true;
        }
    }
    mDatagramProto->socket()->disconnectFromHost();

    //if(isUpdated)     // allows working standalone (whithout server & synchronization)
        sendState(income[mAirport]);
}

void RunwaySyncManager::flushPendingChanges(TData* state)
{
    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    QHash<QString, TData> outcome;
    foreach(const QString& runway, mData.keys())
    {
        if(mData[runway].isDirty())
        {
            if(!outcome.contains(mAirport))
                outcome.insert(mAirport, TData());

            Runway rw;
            rw.numPeople = ( (*state)[runway].numPeople += mData[runway].numPeople);
            rw.numAuto   = ( (*state)[runway].numAuto   += mData[runway].numAuto);
            rw.numOther  = ( (*state)[runway].numOther  += mData[runway].numOther);
            outcome[mAirport][runway] = rw;
        }
    }

    if(outcome.size())
    {
        stream << outcome;
        mDatagramProto->writeDatagram(arr);
    }
}

void RunwaySyncManager::sendState(const TData &state)
{
    if(state.isEmpty())
    {
        foreach(const QString& runway, mData.keys())
        {
            emit newData(runway, 0,
                                 0,
                                 0);

            mData[runway].numPeople = 0;
            mData[runway].numAuto = 0;
            mData[runway].numOther = 0;
        }
    }
    else
    {
        foreach(const QString& runway, state.keys())
        {
            emit newData(runway, state[runway].numPeople,
                                 state[runway].numAuto,
                                 state[runway].numOther);

            mData[runway].numPeople = 0;
            mData[runway].numAuto = 0;
            mData[runway].numOther = 0;
        }
    }
}


void RunwaySyncManager::onPendingPeople(int count)
{
    QString runway = ((Vpp*)sender())->runwayName();
    mData[runway].numPeople += count;
}

void RunwaySyncManager::onPendingAuto(int count)
{
    QString runway = ((Vpp*)sender())->runwayName();
    mData[runway].numAuto += count;
}

void RunwaySyncManager::onPendingOther(int count)
{
    QString runway = ((Vpp*)sender())->runwayName();
    mData[runway].numOther += count;
}

void RunwaySyncManager::onShutdown()
{
    mDatagramProto->socket()->disconnectFromHost();
    mDatagramProto->deleteLater();
    deleteLater();
}

void RunwaySyncManager::startUpdating()
{
    mUpdateTimer.start(5000);
}
