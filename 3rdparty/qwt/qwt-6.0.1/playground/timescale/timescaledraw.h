#ifndef _TIME_SCALE_DRAW_H_
#define _TIME_SCALE_DRAW_H_ 1

#include "timedate.h"
#include <qwt_scale_draw.h>

class TimeScaleDraw: public QwtScaleDraw
{
public:
    TimeScaleDraw();
    virtual ~TimeScaleDraw();

    virtual QwtText label( double ) const;

protected:
    virtual QString format( const QwtScaleDiv & ) const;
    virtual QString format( TimeDate::IntervalType ) const;
};

#endif
