#include "timedate.h"
#include <math.h>
#include <qdebug.h>

static const Qt::DayOfWeek s_dayOfWeek0 = Qt::Monday;
static const int s_julianDay0 = QDate(1970, 1, 1).toJulianDay();

static inline double qwtToJulianDay( int year, int month, int day )
{
    // code from QDate but using doubles to avoid overflows
    // for large values

    const int m1 = ( month - 14 ) / 12;
    const int m2 = ( 367 * ( month - 2 - 12 * m1 ) ) / 12;
    const double y1 = ::floor( ( 4900.0 + year + m1 ) / 100 );

    return ::floor( ( 1461.0 * ( year + 4800 + m1 ) ) / 4 ) + m2
            - ::floor( ( 3 * y1 ) / 4 ) + day - 32075;
}

static inline QDate qwtToDate( int year, int month, int day )
{
    if ( year > 100000 )
    {
        const double jd = qwtToJulianDay( year, month, day );

        if ( jd > INT_MAX )
        {
            qDebug() << "Floor: overrun";
            return QDate();
        }

        return QDate::fromJulianDay( static_cast<int>( jd ) );
    }
    else
    {
        return QDate( year, month, day );
    }
}

QDateTime qwtToDateTime( double value )
{
#if 0
    qint64 msecs = qRound64( value );
    if ( msecs < DATE_MIN )
        return QDateTime();

    const int msecsPerDay = 86400000;

    int days = msecs / msecsPerDay;
    msecs %= msecsPerDay;
    if ( msecs < 0) 
    {
        --days;
        msecs += msecsPerDay;
    }

    if ( double( s_julianDay0 ) + days > double( INT_MAX ) )
    {
        qDebug() << "Overflow";
        //return QDateTime();
    }

    const QDate d = QDate::fromJulianDay( s_julianDay0 + days );
    const QTime t = QTime().addMSecs( msecs );

    QDateTime dt( d, t, Qt::LocalTime );
#if 1
    if ( !dt.isValid() )
    {
        qDebug() << "Not valid: " << value << dt.date().isValid() << dt.time().isValid() << dt.date().toJulianDay();
    }
#endif
#else
    QDateTime dt;
    dt.setMSecsSinceEpoch( qRound64( value ) );
#endif
    return dt;
}

double qwtFromDateTime( const QDateTime &dateTime )
{
#if 0
    const QDateTime dt = dateTime.toUTC();

    const qint64 jd = dt.date().toJulianDay();
    int msecs = QTime( 0, 0, 0 ).msecsTo( dt.time() );

    qint64 days = jd - s_julianDay0;

    const int msecsPerDay = 86400000;
    return ( days * msecsPerDay ) + msecs;
#else
    return dateTime.toMSecsSinceEpoch();
#endif
}

QDateTime qwtCeilDate( const QDateTime &dateTime, 
    TimeDate::IntervalType type )
{
    QDateTime dt;

    switch ( type )
    {
        case TimeDate::Millisecond:
        {
            dt = dateTime;
            break;
        }
        case TimeDate::Second:
        {
            dt.setDate( dateTime.date() );

            const QTime t = dateTime.time();
            dt.setTime( QTime( t.hour(), t.minute(), t.second(), 0 ) );

            if ( dt < dateTime )
                dt.addSecs( 1 );

            break;
        }
        case TimeDate::Minute:
        {
            dt.setDate( dateTime.date() );

            const QTime t = dateTime.time();
            dt.setTime( QTime( t.hour(), t.minute(), 0, 0 ) );

            if ( dt < dateTime )
                dt.addSecs( 60 );

            break;
        }
        case TimeDate::Hour:
        {
            dt.setDate( dateTime.date() );
            
            const QTime t = dateTime.time();
            dt.setTime( QTime( t.hour(), 0, 0, 0 ) );

            if ( dt < dateTime )
                dt.addSecs( 3600 );

            break;
        }
        case TimeDate::Day:
        {
            dt = QDateTime( dateTime.date() );
            if ( dt < dateTime )
                dt = dt.addDays( 1 );

            break;
        }
        case TimeDate::Week:
        {
            dt = QDateTime( dateTime.date() );
            if ( dt < dateTime )
                dt = dt.addDays( 1 );

            int days = s_dayOfWeek0 - dt.date().dayOfWeek();
            if ( days < 0 )
                days += 7;

            dt = dt.addDays( days );

            break;
        }
        case TimeDate::Month:
        {
            dt = QDateTime( qwtToDate( dateTime.date().year(), dateTime.date().month(), 1 ) );
            if ( dt < dateTime )
                dt.addMonths( 1 );

            break;
        }
        case TimeDate::Year:
        {
            const QDate d = dateTime.date();

            int year = d.year();
            if ( d.month() > 1 || d.day() > 1 || !dateTime.time().isNull() )
                year++;

            if ( year == 0 )
                year++; // ther is no year 0

            dt = QDateTime( qwtToDate( year, 1, 1 ) );
            break;
        }
    }

    return dt;
}

QDateTime qwtFloorDate( const QDateTime &dateTime, TimeDate::IntervalType type )
{
    QDateTime dt;

    switch ( type )
    {
        case TimeDate::Millisecond:
        {
            dt = dateTime;
            break;
        }
        case TimeDate::Second:
        {
            dt.setDate( dateTime.date() );

            const QTime t = dateTime.time();
            dt.setTime( QTime( t.hour(), t.minute(), t.second(), 0 ) );

            break;
        }
        case TimeDate::Minute:
        {
            dt.setDate( dateTime.date() );

            const QTime t = dateTime.time();
            dt.setTime( QTime( t.hour(), t.minute(), 0, 0 ) );

            break;
        }
        case TimeDate::Hour:
        {
            dt.setDate( dateTime.date() );

            const QTime t = dateTime.time();
            dt.setTime( QTime( t.hour(), 0, 0, 0 ) );
            break;
        }
        case TimeDate::Day:
        {
            dt = QDateTime( dateTime.date() );
            break;
        }
        case TimeDate::Week:
        {
            dt = QDateTime( dateTime.date() );

            int days = dt.date().dayOfWeek() - s_dayOfWeek0;
            if ( days < 0 )
                days += 7;

            dt = dt.addDays( -days );

            break;
        }
        case TimeDate::Month:
        {
            dt = QDateTime( qwtToDate( dateTime.date().year(), dateTime.date().month(), 1 ) );
            break;
        }
        case TimeDate::Year:
        {
            dt = QDateTime( qwtToDate( dateTime.date().year(), 1, 1 ) );
            break;
        }
    }

    return dt;
}
