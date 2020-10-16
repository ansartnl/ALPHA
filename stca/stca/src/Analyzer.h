#ifndef ANALYZER_H
#define ANALYZER_H

#include "Airplane.h"
#include "AirplaneCollision.h"
#include <QtCore/QObject>
#include <QtNetwork/QHostAddress>
#include <QtCore/QTimer>


namespace rewarn {
    class WarnNotifier;
}
class AirplaneThread;
class QtUdpSocket;

class QUdpSocket;
class Analyzer : public QObject
{
    Q_OBJECT
public:
    Analyzer(QObject *parent = 0);
    bool start(const QHostAddress &listenAddress, quint16 listenPort);
    void stop();

    void setWarnNotifier(rewarn::WarnNotifier *wn);
    void setAirplaneConflictNotifier(AirplaneThread *at);

signals:
    void collectGarbage();

private slots:
    void onUdpData();

    void onPlaneInsideZone(quint16 boardNumber, int typeZone, quint32 idZone);
    void onZoneOnPlaneCourse(quint16 boardNumber, int typeZone, quint32 idZone);

    void onPlaneInsideMeteoZone(quint16 boardNumber, int typeZone, quint32 idZone);
    void onMeteoZoneOnPlaneCourse(quint16 boardNumber, int typeZone, quint32 idZone);

    void onNotifyAboutCollision(int id1, int id2, int RVSM, AirplaneCollision::Type);

private:
    QUdpSocket  *socketListen;
    QtUdpSocket *socketNotify;

    QMap<int, Airplane> plains;

public:
    QHostAddress notifyAddress;
    quint16 notifyPort;
    QString stcaIdentifier;
    int minHmeters;

    /*! Garbage collector - removes airplanes that are not updating */
    QTimer      *mGCTimer;

    /*! True when need to log UDP buffer: Asterix */
    bool isAsterixKeepLogging;

    /*! True when logging of restriction zones should be on */
    bool isWarnKeepLogging;
    /*! Reference to restriction zone notifier */
    rewarn::WarnNotifier *warnNotifier;
    /*! Reference to airplane collision thread */
    AirplaneThread *airplaneThread;
};

#endif // ANALYZER_H
