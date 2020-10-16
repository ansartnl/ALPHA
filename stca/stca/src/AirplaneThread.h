#ifndef AIRPLANE_THREAD_H
#define AIRPLANE_THREAD_H

#include "Airplane.h"
#include "AirplaneCollision.h"

#include <QtCore/QThread>
#include <QtCore/QStack>
#include <QtCore/QQueue>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>

namespace rewarn {
    class WarnNotifier;
}


//! Thread to do searching for intersection and notification about
//! it in case of airplane collision.
class AirplaneThread : public QThread
{
    Q_OBJECT

public:
    AirplaneThread(QObject *parent = 0);

    //! Set reference to warning notifier object.
    /*! Need this notifier to see whether airplane is inside 'No Conflict' zone.
    */
    void setWarnNotifier(rewarn::WarnNotifier *wn);

    void run();

    //! Stop the thread.
    void stop();
    bool isStopped() { return mIsStopped; }

    //! Add new airplane for processing.
    void addAirplane(const Airplane &plain);

public slots:
    //! Remove old airplanes that are not updating for a long time (5 minutes).
    /*! Remove airplanes that are not guided any more.
    */
    void collectGarbage();

signals:
    void notifyAboutCollision(int id1, int id2, int RVSM, AirplaneCollision::Type);

private:
    double findConflictDistance(const Airplane &plain) const;
    double findConfirmedConflictDistance(const Airplane &plain) const;

    //! Add or replace airplane in the map.
    void insertAirplane(const Airplane &a);

    //! Find vertical distance for airplane.
    double findVerticalSpeed(int trackNumber) const;

    //! Returns true if conflict exists for the case when tendency is used.
    bool isTendencyExisted(const Airplane &a, const Airplane &b) const;

public:
    double crashDistance;

    double flightLevelLow;
    double flightLevelHeight;
    double botHeight;
    double topHeight;

    double counterDistance;             //!< Встречное расстояние без тенденции
    double counterDistanceWithClimb;    //!< Встречное расстояние c тенденцией изменения высоты
    double counterTime;                 //!< Встречное время до столкновения
    double followingDistance;           //!< Попутное расстояние без тенденции
    double followingDistanceWithClimb;  //!< Попутное расстояние c тенденцией изменения высоты
    double followingTime;               //!< Попутное время до столкновения
    double perpendicularDistance;
    double perpendicularTime;           //!< Перпендикулярное время до столкновения
    double perpendicularDelta;          //!< Допустимый предел разницы прохождения точки столкновения
    double maxDistance;
    double maxTime;

    /** Минимальное значение вертикальной тенденции скорости, начиная с уровня которой
        начинает срабатывать второй проход алгоритма поиска конфликта */
    double minClimbLevel;
    /** Разрешенный размер корридора отклонения движения самолета где не рассматривается
        тенденция изменения высоты и считается что тенденция == 0 */
    double allowedCorridorHeight;

    double followingAngle;
    double counterAngle;

    int lifeTime;

    int moveToFutureSeconds;

    /*! True when logging of airplanes should be on */
    bool    isAirplanesKeepLogging;
    /*! True when logging of airplane codograms should be on */
    bool    isAsterixConflictLogging;

private:
    //! Extended airplane structure.
    struct AirplaneEx
    {
        typedef QQueue<Location>    TLocationTrace;

        /** Last updated dateTime */
        QDateTime       dt;
        /** Current airplane information */
        Airplane        a;
        /** Trace of flight */
        TLocationTrace  trace;
    };

private:
    /*! Update data mutex */
    QMutex      mUpdateMutex;
    /*! Forever loop mutex */
    QMutex      mForeverMutex;
    /*! Airplane wait condition */
    QWaitCondition mAirplaneWait;

    /*! Reference to "No Conflict" zone checker */
    rewarn::WarnNotifier *mWarnNotifier;

    typedef QMap<int, AirplaneEx>       TAirplanesMap;
    typedef QStack<Airplane>            TAirplanesStack;

    /*! Currently watched airplanes */
    TAirplanesMap   planes;
    /*! New airplanes for searching the collision. */
    TAirplanesStack planesNew;

    /*! Cancel state */
    volatile bool   mIsStopped;
};

#endif // AIRPLANE_THREAD_H
