#include "timescaleengine.h"
#include <qwt_interval.h>
#include <qwt_math.h>
#include <qwt_transform.h>
#include <qdatetime.h>
#include <qdebug.h>

#define DEBUG_ENGINE 0

static inline int qwtDivideInterval( double intervalSize, int numSteps, 
    const int limits[], size_t numLimits )
{
    const double v = intervalSize / numSteps;

    for ( uint i = 0; i < numLimits - 1; i++ )
    {
        if ( v <= limits[i] )
            return limits[i];
    }

    return limits[ numLimits - 1 ];
}

static inline int qwtDivideMonths(
    double intervalSize, int numSteps )
{
    static int limits[] = { 1, 2, 3, 4, 6, 12 };

    return qwtDivideInterval( intervalSize, numSteps,
        limits, sizeof( limits ) / sizeof( double ) );
}

static inline int qwtDivideWeeks(
    double intervalSize, int numSteps )
{
    static int limits[] = { 1, 2, 4, 8, 12, 26, 52 };

    return qwtDivideInterval( intervalSize, numSteps,
        limits, sizeof( limits ) / sizeof( double ) );
}

static inline int qwtDivideDays(
    double intervalSize, int numSteps )
{
    const double v = intervalSize / numSteps;
    if ( v <= 5.0 )
        return qCeil( v );

    return qCeil( v / 7 ) * 7;
}

static inline double qwtDivideHours(
    double intervalSize, int numSteps )
{
    static int limits[] = { 1, 2, 3, 4, 6, 12, 24 };

    return qwtDivideInterval( intervalSize, numSteps,
        limits, sizeof( limits ) / sizeof( double ) );
}

static inline double qwtDivide60(
    double intervalSize, int numSteps )
{
    static int limits[] = { 1, 2, 5, 10, 15, 20, 30, 60 };
    
    return qwtDivideInterval( intervalSize, numSteps,
        limits, sizeof( limits ) / sizeof( double ) );
}   

TimeScaleEngine::TimeScaleEngine():
    d_maxWeeks( -1 )
{
}

TimeScaleEngine::~TimeScaleEngine()
{
}

void TimeScaleEngine::setMaxWeeks( int weeks )
{
    d_maxWeeks = qMax( weeks, -1 );
}

int TimeScaleEngine::maxWeeks() const
{
    return d_maxWeeks;
}

TimeDate::IntervalType TimeScaleEngine::intervalType( double min, double max, int maxSteps ) const
{
    const QDateTime from = qwtToDateTime( min );
    const QDateTime to = qwtToDateTime( max );

    const QDate dm1 = qwtFloorDate( from, TimeDate::Month ).date();
    const QDate dm2 = qwtCeilDate( to, TimeDate::Month ).date();

    const int months = 12 * ( dm2.year() - dm1.year() ) + ( dm2.month() - dm1.month() );
    if ( months > maxSteps * 6 )
        return TimeDate::Year;

    const QDate dt1 = qwtFloorDate( from, TimeDate::Day ).date();
    const QDate dt2 = qwtCeilDate( to, TimeDate::Day ).date();

    const int days = dt1.daysTo( dt2 );
    const int weeks = qCeil( days / 7.0 );

    if ( d_maxWeeks >= 0 )
    {
        if ( weeks > d_maxWeeks )
        {
            if ( days > 2 * maxSteps * 7 )
                return TimeDate::Month;
        }
        else
        {
            if ( days > 2 * maxSteps )
                return TimeDate::Week;
        }
    }
    else
    {
        if ( days > 2 * maxSteps * 7 )
            return TimeDate::Month;

        if ( days > 2 * maxSteps )
            return TimeDate::Week;
    }

    const QDateTime dh1 = qwtFloorDate( from, TimeDate::Hour );
    const QDateTime dh2 = qwtCeilDate( to, TimeDate::Hour );

    const int hours = dh1.secsTo( dh2 ) / 3600;
    if ( hours > 2 * maxSteps )
        return TimeDate::Day;

    const int seconds = from.secsTo( to );

    if ( seconds > maxSteps * 3600 )
        return TimeDate::Hour;

    if ( seconds > maxSteps * 60 )
        return TimeDate::Minute;

    if ( seconds >= maxSteps )
        return TimeDate::Second;

    return TimeDate::Millisecond;
}


void TimeScaleEngine::autoScale( int maxNumSteps,
    double &x1, double &x2, double &stepSize ) const
{
    QwtLinearScaleEngine::autoScale( maxNumSteps, x1, x2, stepSize );
}

QwtScaleDiv TimeScaleEngine::divideScale( double x1, double x2,
    int maxMajSteps, int maxMinSteps, double stepSize ) const
{
    if ( maxMajSteps < 1 )
        maxMajSteps = 1;

    double min = qMin( x1, x2 );
    double max = qMax( x1, x2 );

    if ( !qwtToDateTime( min ).isValid() )
    {
        qDebug() << "Invalid: " << min << qwtToDateTime( min );
        return QwtScaleDiv();
    }

    if ( !qwtToDateTime( max ).isValid() )
    {
        qDebug() << "Invalid: " << max << qwtToDateTime( max );
        return QwtScaleDiv();
    }

#if DEBUG_ENGINE >= 1
    qDebug() << "Divide: " << min << max << qwtToDateTime( min ) << qwtToDateTime( max ) << maxMajSteps;
#endif
    QwtScaleDiv scaleDiv;

    switch ( intervalType( min, max, maxMajSteps ) )
    {
        case TimeDate::Year:
        {
            scaleDiv = divideToYears( min, max, maxMajSteps, maxMinSteps, stepSize );
            break;
        }
        case TimeDate::Month:
        {
            scaleDiv = divideToMonths( min, max, maxMajSteps, maxMinSteps, stepSize );
            break;
        }
        case TimeDate::Week:
        {
            scaleDiv = divideToWeeks( min, max, maxMajSteps, maxMinSteps, stepSize );
            break;
        }
        case TimeDate::Day:
        {
            scaleDiv = divideToDays( min, max, maxMajSteps, maxMinSteps, stepSize );
            break;
        }
        case TimeDate::Hour:
        {
            scaleDiv = divideToHours( min, max, maxMajSteps, maxMinSteps, stepSize );
            break;
        }
        case TimeDate::Minute:
        {
            scaleDiv = divideToMinutes( min, max, maxMajSteps, maxMinSteps, stepSize );
            break;
        }
        case TimeDate::Second:
        {
            scaleDiv = divideToSeconds( min, max, maxMajSteps, maxMinSteps, stepSize );
            break;
        }
        case TimeDate::Millisecond:
        {
            scaleDiv = QwtLinearScaleEngine::divideScale( x1, x2,
                maxMajSteps, maxMinSteps, stepSize );

#if DEBUG_ENGINE >= 2
            qDebug() << "Millisecond: " << max - min;
            const QList<double> ticks = scaleDiv.ticks( QwtScaleDiv::MajorTick);
            for ( int i = 0; i < ticks.size(); i++ )
                qDebug() << i << qRound64( ticks[i] );
#endif

            break;
        }
    }

#if DEBUG_ENGINE >= 2
    qDebug() << scaleDiv;
#endif

    return scaleDiv;
}


QwtScaleDiv TimeScaleEngine::divideToSeconds( double min, double max,
    int maxMajSteps, int maxMinSteps, double stepSize ) const
{
    Q_UNUSED( maxMinSteps );

    // round min/max to seconds
    QDateTime from = qwtToDateTime( min );
    from = qwtFloorDate( from, TimeDate::Second );

    QDateTime to = qwtToDateTime( max );
    to = qwtCeilDate( to, TimeDate::Second );

    // calculate the step size
    stepSize = qAbs( stepSize );
    if ( stepSize == 0.0 )
    {
        const double seconds = from.secsTo( to );
        stepSize = qwtDivide60( seconds, maxMajSteps );

#if DEBUG_ENGINE >= 2
        qDebug() << "Seconds: " << seconds << " -> " << stepSize;
#endif
    }

    // calculate the major ticks
    QList<double> majorTicks;

    for ( QDateTime dt = from; dt <= to; 
        dt = dt.addMSecs( qRound64( stepSize * 1000 ) ) )
    {
        const double value = qwtFromDateTime( dt );
        if ( value >= min && value <= max )
            majorTicks += value;
    }

    // calculate the minor/medium ticks
    QList<double> minorTicks;
    QList<double> mediumTicks;

    if ( maxMinSteps > 1 )
    {
        const double minStepSize = divideInterval( stepSize, maxMinSteps );

        const int numSteps = stepSize / minStepSize;

        for ( QDateTime dt = from; dt <= to; 
            dt = dt.addMSecs( qRound64( stepSize * 1000 ) ) )
        {
            const double tick0 = qwtFromDateTime( dt );

            for ( int i = 1; i < numSteps; i++ )
            {
                const double value = tick0 + i * minStepSize * 1000;
                if ( value >= min && value <= max )
                {
                    if ( numSteps % 2 == 0 && i == numSteps / 2 )
                        mediumTicks += value;
                    else
                        minorTicks += value;
                }
            }
        }
    }

    QwtScaleDiv scaleDiv;
    scaleDiv.setInterval( min, max );
    scaleDiv.setTicks( QwtScaleDiv::MajorTick, majorTicks );
    scaleDiv.setTicks( QwtScaleDiv::MediumTick, mediumTicks );
    scaleDiv.setTicks( QwtScaleDiv::MinorTick, minorTicks );

    return scaleDiv;
}

QwtScaleDiv TimeScaleEngine::divideToMinutes( double min, double max,
    int maxMajSteps, int maxMinSteps, double stepSize ) const
{
    Q_UNUSED( maxMinSteps );

    QDateTime from = qwtToDateTime( min );
    from = qwtFloorDate( from, TimeDate::Minute );

    QDateTime to = qwtToDateTime( max );
    to = qwtCeilDate( to, TimeDate::Minute );

    stepSize = qAbs( stepSize );
    if ( stepSize == 0.0 )
    {
        const double minutes = from.secsTo( to ) / 60;
        stepSize = qwtDivide60( minutes, maxMajSteps );
#if DEBUG_ENGINE >= 2
        qDebug() << "Minutes: " << minutes << " -> " << stepSize;
#endif
    }

    QList<double> majorTicks;
    while ( from <= to )
    {
        const double value = qwtFromDateTime( from );
        if ( value >= min && value <= max )
            majorTicks += value;

        from = from.addSecs( qRound( 60 * stepSize ) );
    }

    // calculate the minor/medium ticks
    QList<double> minorTicks;
    QList<double> mediumTicks;

    QwtScaleDiv scaleDiv;
    scaleDiv.setInterval( min, max );
    scaleDiv.setTicks( QwtScaleDiv::MajorTick, majorTicks );
    scaleDiv.setTicks( QwtScaleDiv::MediumTick, mediumTicks );
    scaleDiv.setTicks( QwtScaleDiv::MinorTick, minorTicks );

    return scaleDiv;
}

QwtScaleDiv TimeScaleEngine::divideToHours( double min, double max,
    int maxMajSteps, int maxMinSteps, double stepSize ) const
{
    Q_UNUSED( maxMinSteps );

    QDateTime from = qwtToDateTime( min );
    from = qwtFloorDate( from, TimeDate::Hour );

    QDateTime to = qwtToDateTime( max );
    to = qwtCeilDate( to, TimeDate::Hour );

    stepSize = qAbs( stepSize );
    if ( stepSize == 0.0 )
    {
        const double hours = from.secsTo( to ) / 3600;
        stepSize = qwtDivideHours( hours, maxMajSteps );

#if DEBUG_ENGINE >= 2
        qDebug() << "Hours: " << hours << " -> " << stepSize;
#endif
    }

    QList<double> majorTicks;
    while ( from <= to )
    {
        const double value = qwtFromDateTime( from );
        if ( value >= min && value <= max )
            majorTicks += value;

        from = from.addSecs( 3600 * stepSize );
    }

    // calculate the minor/medium ticks
    QList<double> minorTicks;
    QList<double> mediumTicks;

    QwtScaleDiv scaleDiv;
    scaleDiv.setInterval( min, max );
    scaleDiv.setTicks( QwtScaleDiv::MajorTick, majorTicks );

    return scaleDiv;
}

QwtScaleDiv TimeScaleEngine::divideToDays( double min, double max,
    int maxMajSteps, int maxMinSteps, double stepSize ) const
{
    Q_UNUSED( maxMinSteps );

    QDateTime from = qwtToDateTime( min );
    from = qwtFloorDate( from, TimeDate::Day );

    QDateTime to = qwtToDateTime( max );
    to = qwtCeilDate( to, TimeDate::Day );

    stepSize = qAbs( stepSize );
    if ( stepSize == 0.0 )
    {
        const double days = from.daysTo( to );
        stepSize = qwtDivideDays( days, maxMajSteps );

#if DEBUG_ENGINE >= 2
        qDebug() << "Days: " << days << " -> " << stepSize;
#endif
    }

    QList<double> majorTicks;
    while ( from <= to )
    {
        const double value = qwtFromDateTime( from );
        if ( value >= min && value <= max )
            majorTicks += value;

        from = from.addSecs( qRound( 24 * 3600 * stepSize ) );
    }

    // calculate the minor/medium ticks
    QList<double> minorTicks;
    QList<double> mediumTicks;

    QwtScaleDiv scaleDiv;
    scaleDiv.setInterval( min, max );
    scaleDiv.setTicks( QwtScaleDiv::MajorTick, majorTicks );
    scaleDiv.setTicks( QwtScaleDiv::MediumTick, mediumTicks );
    scaleDiv.setTicks( QwtScaleDiv::MinorTick, minorTicks );

    return scaleDiv;
}

QwtScaleDiv TimeScaleEngine::divideToWeeks( double min, double max,
    int maxMajSteps, int maxMinSteps, double stepSize ) const
{
    Q_UNUSED( maxMinSteps );

    QDateTime from = qwtToDateTime( min );
    from = qwtFloorDate( from, TimeDate::Week );

    QDateTime to = qwtToDateTime( max );
    to = qwtCeilDate( to, TimeDate::Week );

    stepSize = qAbs( stepSize );
    if ( stepSize == 0.0 )
    {
        const int weeks = qFloor( from.daysTo( to ) / 7 );
        stepSize = qwtDivideWeeks( weeks, maxMajSteps );

#if DEBUG_ENGINE >= 2
        qDebug() << "Weeks: " << weeks << " -> " << stepSize;
#endif
    }

    const int stepSizeInDays = qCeil( stepSize * 7 );

    QList<double> majorTicks;
    for ( QDateTime dt = from; dt <= to; dt = dt.addDays( stepSizeInDays ) )
    {
        const double value = qwtFromDateTime( dt );
        if ( value >= min && value <= max )
            majorTicks += value;
    }

    QList<double> minorTicks;
    QList<double> mediumTicks;

    if ( maxMinSteps >= stepSize * 7 )
    {
        // ticks for each day

        for ( QDateTime dt = from; dt <= to; dt = dt.addDays( stepSizeInDays ) )
        {
            for ( int i = 1; i < stepSize * 7; i++ )
            {
                const double value = qwtFromDateTime( dt.addDays( i ) );
                if ( value >= min && value <= max )
                {
                    if ( ( i % 7 ) == 0 )
                        mediumTicks += value;
                    else
                        minorTicks += value;
                }
            }
        }
    }
    else if ( maxMinSteps > 1 )
    {
        // ticks for each week

        int minStepSize = qCeil( divideInterval( stepSize, maxMinSteps ) );
        int numSteps = qRound( minStepSize / stepSize );

        for ( QDateTime dt = from; dt <= to; dt = dt.addDays( stepSizeInDays ) )
        {
            for ( int i = 1; i < numSteps; i++ )
            {
                const double value = qwtFromDateTime( dt.addDays( i * numSteps ) );
                if ( value >= min && value <= max )
                    minorTicks += value;
            }
        }
    }

    QwtScaleDiv scaleDiv;
    scaleDiv.setInterval( min, max );
    scaleDiv.setTicks( QwtScaleDiv::MajorTick, majorTicks );
    scaleDiv.setTicks( QwtScaleDiv::MediumTick, mediumTicks );
    scaleDiv.setTicks( QwtScaleDiv::MinorTick, minorTicks );

    return scaleDiv;
}

QwtScaleDiv TimeScaleEngine::divideToMonths( double min, double max,
    int maxMajSteps, int maxMinSteps, double stepSize ) const
{
    Q_UNUSED( maxMinSteps );

    if ( min > max )
        qSwap( min, max );

    QDateTime from = qwtToDateTime( min );
    from = qwtFloorDate( from, TimeDate::Month );

    QDateTime to = qwtToDateTime( max );
    to = qwtCeilDate( to, TimeDate::Month );

    stepSize = qAbs( stepSize );
    if ( stepSize == 0.0 )
    {
        const double months = 12 * ( to.date().year() - from.date().year() ) 
            + to.date().month() - from.date().month();

        stepSize = qwtDivideMonths( months, maxMajSteps );

#if DEBUG_ENGINE >= 2
        qDebug() << "Months: " << months << " -> " << stepSize;
#endif
    }

    stepSize = qMax( stepSize, 1.0 );

    QList<double> majorTicks;
    while ( from <= to )
    {
        const double value = qwtFromDateTime( from );
        if ( value >= min && value <= max )
            majorTicks += value;

        from = from.addMonths( stepSize );
    }

    // calculate the minor/medium ticks
    QList<double> minorTicks;
    QList<double> mediumTicks;

    QwtScaleDiv scaleDiv;
    scaleDiv.setInterval( min, max );
    scaleDiv.setTicks( QwtScaleDiv::MajorTick, majorTicks );
    scaleDiv.setTicks( QwtScaleDiv::MediumTick, mediumTicks );
    scaleDiv.setTicks( QwtScaleDiv::MinorTick, minorTicks );

    return scaleDiv;
}

QwtScaleDiv TimeScaleEngine::divideToYears( double min, double max,
    int maxMajSteps, int maxMinSteps, double stepSize ) const
{
    Q_UNUSED( maxMinSteps );

    if ( min > max )
        qSwap( min, max );

    QDateTime from = qwtToDateTime( min );
    from = qwtFloorDate( from, TimeDate::Year );

    QDateTime to = qwtToDateTime( max );
    to = qwtCeilDate( to, TimeDate::Year );

    stepSize = qAbs( stepSize );
    if ( stepSize == 0.0 )
    {
        const int years = to.date().year() - from.date().year();
        stepSize = divideInterval( years, maxMajSteps );

#if DEBUG_ENGINE >= 2
        qDebug() << "Years: " << from.date() << to.date() << years << " -> " << stepSize;
#endif
    }

    QList<double> majorTicks;
    while ( from <= to )
    {
        const double value = qwtFromDateTime( from );
        if ( value >= min && value <= max )
            majorTicks += value;

        from = from.addMonths( qRound( stepSize * 12 ) );
    }   

    // calculate the minor/medium ticks
    QList<double> minorTicks;
    QList<double> mediumTicks;

    QwtScaleDiv scaleDiv;
    scaleDiv.setInterval( min, max );
    scaleDiv.setTicks( QwtScaleDiv::MajorTick, majorTicks );
    scaleDiv.setTicks( QwtScaleDiv::MediumTick, mediumTicks );
    scaleDiv.setTicks( QwtScaleDiv::MinorTick, minorTicks );

    return scaleDiv;
}

QwtTransform *TimeScaleEngine::transformation() const
{
    return new QwtNullTransform();
}

