#ifndef _TIME_DATE_H
#define _TIME_DATE_H

#include <qglobal.h>
#include <qdatetime.h>

#define DATE_MIN Q_INT64_C(-210866716800000)

namespace TimeDate
{
    enum IntervalType
    {
        Millisecond,
        Second,
        Minute,
        Hour,
        Day,
        Week,
        Month,
        Year
    };
};

extern QDateTime qwtToDateTime( double value );
extern double qwtFromDateTime( const QDateTime & );

extern QDateTime qwtCeilDate( const QDateTime &, 
    TimeDate::IntervalType );

extern QDateTime qwtFloorDate( const QDateTime &, 
    TimeDate::IntervalType );

#endif
