#include "timescaledraw.h"
#include <qdatetime.h>

static TimeDate::IntervalType qwtIntervalType( const QList<QDateTime> &dateTimes )
{
    for ( int type = TimeDate::Second; type <= TimeDate::Year; type++ )
    {
        if ( type == TimeDate::Week )
            type++;

        const TimeDate::IntervalType intervalType = 
            static_cast<TimeDate::IntervalType>( type );

        for ( int i = 0; i < dateTimes.size(); i++ )
        {
            if ( qwtFloorDate( dateTimes[i], intervalType ) != dateTimes[i] )
            {
                return static_cast<TimeDate::IntervalType>( type - 1 );
            }
        }
    }

    return TimeDate::Year;
}

TimeScaleDraw::TimeScaleDraw()
{
}

TimeScaleDraw::~TimeScaleDraw()
{
}

QwtText TimeScaleDraw::label( double value ) const
{
    static QDateTime time;

    time.setMSecsSinceEpoch( qRound64( value ) );

    // the format string should be cached !!!
    return time.toString( format( scaleDiv() ) );
}

QString TimeScaleDraw::format( const QwtScaleDiv &scaleDiv ) const
{
    const QList<double> ticks = scaleDiv.ticks( QwtScaleDiv::MajorTick );

    QList<QDateTime> dates;
    for ( int i = 0; i < ticks.size(); i++ )
        dates += qwtToDateTime( ticks[i] );

    return format( qwtIntervalType( dates ) );
}

QString TimeScaleDraw::format( TimeDate::IntervalType intervalType ) const
{
    QString format;

    switch( intervalType )
    {
        case TimeDate::Year:
        {
            format = "yyyy";
            break;
        }
        case TimeDate::Month:
        {
            format = "MMM yyyy";
            break;
        }
        case TimeDate::Week:
        case TimeDate::Day:
        {
            format = "ddd dd MMM yyyy";
            break;
        }
        case TimeDate::Hour:
        case TimeDate::Minute:
        {
            format = "hh:mm\nddd dd MMM yyyy";
            break;
        }
        case TimeDate::Second:
        {
            format = "hh:mm:ss\nddd dd MMM yyyy";
            break;
        }
        case TimeDate::Millisecond:
        default:
        {
            format = "hh:mm:ss:zzz\nddd dd MMM yyyy";
        }
    }

    return format;
}

