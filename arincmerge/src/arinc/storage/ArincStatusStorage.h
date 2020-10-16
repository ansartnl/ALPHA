#ifndef ARINCSTATUSSTORAGE_H
#define ARINCSTATUSSTORAGE_H

#include "ArincNativeStorage.h"
#include <QObject>

class ArincStatusStorage : public QObject, public ArincNativeStorage
{
    Q_OBJECT
public:
    explicit ArincStatusStorage(QObject *parent = 0);
    inline virtual ~ArincStatusStorage() {}
    inline virtual void add(const FirUir &in) { ArincNativeStorage::add(in); emit statusChanged(tr("Fir/Uir: %1").arg(in.name)); }
    inline virtual void add(const ControlledAirspace &in) { ArincNativeStorage::add(in); emit statusChanged(tr("Controlled airspace: %1").arg(in.name)); }
    inline virtual void add(const RestrictedAirspace &in) { ArincNativeStorage::add(in); emit statusChanged(tr("Restricted airspace: %1").arg(in.name)); }
    inline virtual void add(const VhfNavaid &in) { ArincNativeStorage::add(in); emit statusChanged(tr("VHF navaid: %1").arg(in.name)); }
    inline virtual void add(const NdbNavaid &in) { ArincNativeStorage::add(in); emit statusChanged(tr("NDB navaid: %1").arg(in.name)); }
    inline virtual void add(const EnrouteWaypoint &in) { ArincNativeStorage::add(in); emit statusChanged(tr("Enroute waypoint: %1").arg(in.name)); }
    inline virtual void add(const Airway &in) { ArincNativeStorage::add(in); emit statusChanged(tr("Airway: %1").arg(in.id)); }
    inline virtual void add(const Holding &in) { ArincNativeStorage::add(in); emit statusChanged(tr("Holding: %1").arg(in.icaoCode)); }
    inline virtual void add(const AirportRefPoint &in) { ArincNativeStorage::add(in); emit statusChanged(tr("Airport reference point: %1").arg(in.name)); }
    inline virtual void add(const TerminalWaypoint &in) { ArincNativeStorage::add(in); emit statusChanged(tr("Terminal waypoint: %1").arg(in.name)); }
    inline virtual void add(const Sid &in) { ArincNativeStorage::add(in); emit statusChanged(tr("SID: %1").arg(in.id)); }
    inline virtual void add(const Star &in) { ArincNativeStorage::add(in); emit statusChanged(tr("STAR: %1").arg(in.id)); }
    inline virtual void add(const Approach &in) { ArincNativeStorage::add(in); emit statusChanged(tr("Approach: %1").arg(in.id)); }
    inline virtual void add(const Runway &in) { ArincNativeStorage::add(in); emit statusChanged(tr("Runway: %1").arg(in.id)); }
    inline virtual void add(const NdbTerminal &in) { ArincNativeStorage::add(in); emit statusChanged(tr("NDB terminal: %1").arg(in.name)); }

signals:
    void statusChanged(const QString &status);
};

#endif // ARINCSTATUSSTORAGE_H
