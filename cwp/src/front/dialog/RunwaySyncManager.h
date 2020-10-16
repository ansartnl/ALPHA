#ifndef RUNWAYSYNCMANAGER_H
#define RUNWAYSYNCMANAGER_H

#include <QTimer>
#include <QTcpSocket>

#include "qttcpdatagramprotocol.h"

struct Runway{
    Runway():numPeople(0), numAuto(0), numOther(0)
    {}

    int numPeople;
    int numAuto;
    int numOther;

    inline bool isDirty()
    {return numPeople != 0 || numAuto !=0 || numOther != 0;}
};
Q_DECLARE_METATYPE(Runway)

QDataStream &operator<<(QDataStream &out, const Runway &runway);
QDataStream &operator>>(QDataStream &in, Runway &runway);


class RunwaySyncManager : public QObject
{
    Q_OBJECT
    typedef QHash<QString, Runway> TData;
public:
    explicit RunwaySyncManager(QObject *parent = 0);
    virtual ~RunwaySyncManager(){}

    inline void setAirportName(const QString& name)
    {mAirport = name;}
    inline const QString& airportName() const
    {return mAirport;}

signals:
    void newData(QString runway, int numPeople, int numAuto, int numOther);
    
public slots:
    void onPendingPeople(int count);
    void onPendingAuto(int count);
    void onPendingOther(int count);

    void onShutdown();

    void startUpdating();

private slots:
    void onUpdate();
    void flushPendingChanges(TData *state);
    void sendState(const TData& state);

private:
    QString     mAirport;
    QTimer      mUpdateTimer;

    QString     mHost;
    quint16     mPort;
    TData       mData;

    QtTcpDatagramProtocol* mDatagramProto;
};

#endif // RUNWAYSYNCMANAGER_H
