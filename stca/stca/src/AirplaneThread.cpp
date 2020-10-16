#include "AirplaneThread.h"
#include "WarnNotifier.h"
#include "Vincenty.h"
#include <QtCore/QLineF>
#include <qmath.h>

// Maximum number of points in the airplane trace.
static const int g_numberOfTracePoints = 10;

static void printConflictLog(const Airplane &a, const Airplane &b)
{
    qDebug() << AirplaneThread::tr("Airplane '%1' asterix: ").arg(a.trackNumber) << a.buffer.toHex();
    qDebug() << AirplaneThread::tr("Airplane '%1' asterix: ").arg(b.trackNumber) << b.buffer.toHex();
}


AirplaneThread::AirplaneThread(QObject *parent)
    : QThread(parent), isAirplanesKeepLogging(false), isAsterixConflictLogging(false),
      mWarnNotifier(0), mIsStopped(true)
{
    if (objectName().isEmpty())
        setObjectName("airplanes_worker");

    crashDistance = 100;
    flightLevelLow = 300;
    flightLevelHeight = 600;
    botHeight = 290 * 30.48;
    topHeight = 410 * 30.48;
    counterDistance = 100000;
    counterDistanceWithClimb = 30000;
    counterTime = 300;
    followingDistance = 10000;
    followingTime = 300;
    counterDistanceWithClimb = 20000;
    perpendicularDistance = 10000;
    perpendicularTime = 300;
    perpendicularDelta = 40;
    maxDistance = 30000;
    maxTime = 300;
    followingAngle = 45;
    counterAngle = 135;

    allowedCorridorHeight = 120; // meters
    minClimbLevel = 2; // (min vz) meters per second

    moveToFutureSeconds = 0;
}

void AirplaneThread::setWarnNotifier(rewarn::WarnNotifier *wn)
{
    mWarnNotifier = wn;
}

void AirplaneThread::addAirplane(const Airplane &plain)
{
    QMutexLocker locker(&mUpdateMutex);
    planesNew.push(plain);
    mAirplaneWait.wakeAll();
}

void AirplaneThread::run()
{
    mIsStopped = false;

    qDebug() << tr("Worker thread started successfully ...");

    Airplane a;

    forever
    {
        if ( mIsStopped )
            break;

        QMutexLocker mutexForeverLocker(&mForeverMutex);
        if ( planesNew.empty() )
            mAirplaneWait.wait(&mForeverMutex);

        if ( mIsStopped )
            break;

        // Retrieve one airplane from the stack.
        {
            QMutexLocker locker(&mUpdateMutex);

            if ( planesNew.size() > 10000 )
            {
                qWarning() << tr("Processing stack of airplanes '%1' is too big. Optimization is required here.");
                // Cut stack as number of airplanes too big.
                planesNew.remove(1000, planesNew.size()-1000);
                qWarning() << tr("Processing stack is cut to 1000 airplanes number.");
            }

            a = planesNew.pop();
        }

        // Patch airplane info. Future positions based on vx, vy, vz speed vector
        // Patched plane.position, plane.z
        QPointF pointf = Vincenty::toCartesian(a.position, a.position);
        pointf += QPointF(a.vx * moveToFutureSeconds, a.vy * moveToFutureSeconds);
        a.position = Vincenty::toGeographic(pointf, a.position);
        a.z += a.vz * moveToFutureSeconds; // move airplane to future position

        // If plane is inside 'No Conflict' zone -> continue.
        if ( mWarnNotifier )
        {
            rewarn::AirplaneInfo airplaneInfo;
            airplaneInfo.x = a.position.latitude();
            airplaneInfo.y = a.position.longitude();
            airplaneInfo.height = a.z;

            if ( mWarnNotifier->isInsideNFZone( airplaneInfo ) )
            {
                insertAirplane(a);
                continue;
            }
        }

        // Test collision of new airplane with others.

        for ( TAirplanesMap::const_iterator itc = planes.constBegin();
              itc != planes.constEnd(); ++itc )
        {
            const Airplane &b = (*itc).a;
            // Avoid of checking itself.
            if ( b.trackNumber == a.trackNumber )
                continue;

            double dz = qAbs(b.z - a.z);

            // flight Level > 410 eshelon: conflict when < 600 meters and
            //                                 confirmed conflict - < 600 meters
            // flight Level 290 - 410 eshelon: conflict when < 600 meters and
            //                                 confirmed conflict - < 300 meters
            // flight Level < 290 eshelon: conflict when < 300 meters and
            //                             confirmed conflict - < 300 meters

            double conflictDistance = qMax(findConflictDistance(a), findConflictDistance(b));
            double confirmedConflictDistance = qMax(findConfirmedConflictDistance(a), findConfirmedConflictDistance(b));

            if ( dz < conflictDistance ) // planes have a conflict
            {
                int RVSM = 0;
                if(dz < confirmedConflictDistance) RVSM = 1;
                QPointF position = Vincenty::toCartesian(b.position, a.position); // a(0,0); b(x,y);
                double dx = position.x();
                double dy = position.y();
                double planeDistance = qSqrt(dx*dx + dy*dy);

                double aV = qSqrt(a.vx*a.vx +
                                  a.vy*a.vy +
                                  a.vz*a.vz);
                double bV = qSqrt(b.vx*b.vx +
                                  b.vy*b.vy +
                                  b.vz*b.vz);
                double time = planeDistance / (aV + bV);

                if (planeDistance < maxDistance || time < maxTime)
                {
                    double distance = qSqrt(dx*dx + dy*dy + dz*dz);
                    //qDebug() << "distance" << distance;
                    if (distance < crashDistance)
                    {
#ifndef QT_NO_DEBUG
                        qDebug() << tr("Collision(Crash) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#else
                        if ( isAirplanesKeepLogging )
                            qDebug() << tr("Collision(Crash) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#endif
                        if ( isAsterixConflictLogging )
                            printConflictLog(a, b);

                        emit notifyAboutCollision(a.trackNumber, b.trackNumber, 1, AirplaneCollision::Crash);
                        continue;
                    }

                    QLineF va(0, 0, a.vx, a.vy);
                    QLineF vb(0, 0, b.vx, b.vy);                    

                    double angle = va.angleTo(vb);
                    if (angle > 180)
                        angle = 360 - angle;

                    if (angle < followingAngle)
                    {
                        if (planeDistance < followingDistance)
                        {
#ifndef QT_NO_DEBUG
                            qDebug() << tr("Collision(Following) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#else
                            if ( isAirplanesKeepLogging )
                                qDebug() << tr("Collision(Following) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#endif
                            if ( isAsterixConflictLogging )
                                printConflictLog(a, b);

                            emit notifyAboutCollision(a.trackNumber, b.trackNumber, RVSM, AirplaneCollision::FollowingWarning);
                            continue;
                        }
                        else if (planeDistance < followingDistanceWithClimb && isTendencyExisted(a, b))
                        {
#ifndef QT_NO_DEBUG
                            qDebug() << tr("Collision(Following:tendency) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#else
                            if ( isAirplanesKeepLogging )
                                qDebug() << tr("Collision(Following:tendency) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#endif
                            if ( isAsterixConflictLogging )
                                printConflictLog(a, b);

                            emit notifyAboutCollision(a.trackNumber, b.trackNumber, RVSM, AirplaneCollision::FollowingWarning);
                            continue;
                        }
                        else if (planeDistance / qAbs(aV - bV) <= followingTime)
                        {
#ifndef QT_NO_DEBUG
                            qDebug() << tr("Collision(Following:time) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#else
                            if ( isAirplanesKeepLogging )
                                qDebug() << tr("Collision(Following:time) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#endif
                            if ( isAsterixConflictLogging )
                                printConflictLog(a, b);

                            emit notifyAboutCollision(a.trackNumber, b.trackNumber, RVSM, AirplaneCollision::FollowingWarning);
                            continue;
                        }
                    }
                    else if (angle > counterAngle)
                    {
                        if (planeDistance < counterDistance)
                        {
#ifndef QT_NO_DEBUG
                            qDebug() << tr("Collision(Counter) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#else
                            if ( isAirplanesKeepLogging )
                                qDebug() << tr("Collision(Counter) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#endif
                            if ( isAsterixConflictLogging )
                                printConflictLog(a, b);

                            emit notifyAboutCollision(a.trackNumber, b.trackNumber, RVSM, AirplaneCollision::CounterWarning);
                            continue;
                        }
                        else if (planeDistance < counterDistanceWithClimb && isTendencyExisted(a, b))
                        {
#ifndef QT_NO_DEBUG
                            qDebug() << tr("Collision(Counter:tendency) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#else
                            if ( isAirplanesKeepLogging )
                                qDebug() << tr("Collision(Counter:tendency) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#endif
                            if ( isAsterixConflictLogging )
                                printConflictLog(a, b);

                            emit notifyAboutCollision(a.trackNumber, b.trackNumber, RVSM, AirplaneCollision::CounterWarning);
                            continue;
                        }
                        else if (planeDistance / (aV + bV) <= counterTime)
                        {
#ifndef QT_NO_DEBUG
                            qDebug() << tr("Collision(Counter:time) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#else
                            if ( isAirplanesKeepLogging )
                                qDebug() << tr("Collision(Counter:time) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#endif
                            if ( isAsterixConflictLogging )
                                printConflictLog(a, b);

                            emit notifyAboutCollision(a.trackNumber, b.trackNumber, RVSM, AirplaneCollision::CounterWarning);
                            continue;
                        }
                    }
                    else
                    {
                        if (planeDistance < perpendicularDistance)
                        {
#ifndef QT_NO_DEBUG
                            qDebug() << tr("Collision(Perpend) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#else
                            if ( isAirplanesKeepLogging )
                                qDebug() << tr("Collision(Perpend) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#endif
                            if ( isAsterixConflictLogging )
                                printConflictLog(a, b);

                            emit notifyAboutCollision(a.trackNumber, b.trackNumber, RVSM, AirplaneCollision::PerpendicularWarning);
                            continue;
                        }
                        else
                        {
                            QPointF intersection;
                            QLineF lineA(va);
                            QLineF lineB(position, position + vb.p2());

                            if(lineA.intersect(lineB, &intersection) != QLineF::NoIntersection)
                            {
                                QLineF p2A(lineA.p2(), intersection);
                                QLineF p2B(lineB.p2(), intersection);

                                lineA.setP2(intersection);
                                lineB.setP2(intersection);

                                bool isDirection = (lineA.length() > p2A.length() &&
                                                    lineB.length() > p2B.length());

                                double ta = lineA.length()/aV;
                                double tb = lineB.length()/bV;

                                if(qAbs( ta - tb ) <= perpendicularDelta &&
                                        isDirection &&
                                        (ta <= perpendicularTime || tb <= perpendicularTime))
                                {
#ifndef QT_NO_DEBUG
                                    qDebug() << tr("Collision(Perpend:time) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#else
                                    if ( isAirplanesKeepLogging )
                                        qDebug() << tr("Collision(Perpend:time) between ") << a.trackNumber << "(" << a.position << a.z << tr(") and") << b.trackNumber << "(" << b.position << b.z << ")";
#endif
                                    if ( isAsterixConflictLogging )
                                        printConflictLog(a, b);

                                    emit notifyAboutCollision(a.trackNumber, b.trackNumber, RVSM, AirplaneCollision::PerpendicularWarning);
                                    continue;
                                }
                            }
                        }
                    }
                }
            }
        }

        insertAirplane(a);
    }

    qDebug() << tr("Worker thread was stopped ...");
}

void AirplaneThread::stop()
{
    mIsStopped = true;
    mAirplaneWait.wakeAll();
}

void AirplaneThread::insertAirplane(const Airplane &a)
{
    if ( planes.contains(a.trackNumber) )
    {
        AirplaneEx &aex = planes[a.trackNumber];
        aex.trace.enqueue(Location(aex.a.position, aex.a.z));
        aex.a = a;
        aex.dt = QDateTime::currentDateTime();

        // Leave only 10 points in the trace.
        while ( aex.trace.size() > g_numberOfTracePoints )
            aex.trace.dequeue();
    }
    else
    {
        AirplaneEx aex;
        aex.a = a;
        aex.dt = QDateTime::currentDateTime();

        planes.insert(a.trackNumber, aex);
    }
}

bool AirplaneThread::isTendencyExisted(const Airplane &a, const Airplane &b) const
{
    // Для варианта с тенденцией изменения высоты и в том случае когда
    // тенденция изменения высоты больше установленного значения (например:
    // 2 м/с) выполняется отдельная проверка сближения самолетов, даже
    // если они находятся на разных эшелонах.
    double a_vz = findVerticalSpeed(a.trackNumber),
            b_vz = findVerticalSpeed(b.trackNumber);
    if ( qAbs(a_vz) > minClimbLevel || qAbs(b_vz) > minClimbLevel )
    {
        // Если самолеты расходятся - то их не рассматриваем (один летит вверх
        // второй вниз).
        if ( (a_vz >= 0 && b_vz <= 0 && a.z >= b.z) ||
             (a_vz <= 0 && b_vz >= 0 && a.z <= b.z) )
            return false;

        return true;
    }
    return false;
}

double AirplaneThread::findVerticalSpeed(int trackNumber) const
{
    if ( !planes.contains(trackNumber) )
        return 0.;

    const AirplaneEx &aex = planes[trackNumber];

    // If vertical airplane speed is defined -> use it immedeately.
    if ( !qIsNull(aex.a.vz) )
        return aex.a.vz;

    if ( aex.trace.size() >= g_numberOfTracePoints )
    {
        double minv = 1.e30, maxv = -1.e30;
        // Find corridor of airplane altitudes.
        foreach ( const Location &l, aex.trace )
        {
            minv = qMin(minv, l.z);
            maxv = qMax(maxv, l.z);
        }

        // Test that airplane is inside allowed corridor for direct flight.
        if ( maxv-minv < allowedCorridorHeight )
            return 0.;
    }

    if ( aex.trace.size() > 2 )
    {
        double vz = 0.;
        // Calculate average climb descent using 3 latest points.
        for ( int i = 1; i < 3; i++ )
            vz += aex.trace[i].z - aex.trace[i-1].z;
        return vz / 2.;
    }

    return 0.;
}

double AirplaneThread::findConflictDistance(const Airplane &plain) const
{
    if (plain.z < botHeight)
        return flightLevelLow;
    return flightLevelHeight;
}

double AirplaneThread::findConfirmedConflictDistance(const Airplane &plain) const
{
    if (plain.z < botHeight)
        return flightLevelLow;
    else if (plain.z > topHeight)
        return flightLevelHeight;
    else
        return flightLevelLow;
}

void AirplaneThread::collectGarbage()
{
    QMutexLocker mutexForeverLocker(&mForeverMutex);

    QDateTime currentDateTime = QDateTime::currentDateTime();
    //const int secsToDel = 5*60; // 5 minutes
    const int secsToDel = 2*60; // 2 minutes

    QString idDelString;

    // Remove planes that were not updated more than 'secsToDel' seconds before.
    foreach ( int id, planes.keys() )
    {
        const QDateTime &dt = planes.value(id).dt;
        if ( dt.secsTo(currentDateTime) > secsToDel )
        {
            planes.remove(id);
            idDelString += QString("'%1', ").arg(id);
        }
    }

    if ( !idDelString.isEmpty() )
    {
        idDelString = idDelString.left(idDelString.length()-2);
        qDebug() << tr("Airplanes %1 were removed from processing list as they were not updated more than '%2' seconds")
                    .arg(idDelString).arg(secsToDel);
    }
}
