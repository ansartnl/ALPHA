#include "plot.h"
#include "settings.h"
#include "timedate.h"
#include "timescaleengine.h"
#include "timescaledraw.h"
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>

Plot::Plot( QWidget *parent ):
    QwtPlot( parent )
{
    const int axis = QwtPlot::yLeft;

    TimeScaleDraw *scaleDraw = new TimeScaleDraw();
    setAxisScaleDraw( axis, scaleDraw );

    TimeScaleEngine *scaleEngine = new TimeScaleEngine();
    setAxisScaleEngine( axis, scaleEngine );

    QwtPlotPanner *panner = new QwtPlotPanner( canvas() );
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( canvas() );

    for ( int i = 0; i < QwtPlot::axisCnt; i++ )
    {
        const bool on = ( i == axis );
        panner->setAxisEnabled( i, on );
        magnifier->setAxisEnabled( i, on );
    }
}

void Plot::applySettings( const Settings &settings )
{
    const int axis = QwtPlot::yLeft;

    TimeScaleEngine *scaleEngine = 
        static_cast<TimeScaleEngine *>( axisScaleEngine( axis ) );

    scaleEngine->setMaxWeeks( settings.maxWeeks );
    setAxisMaxMinor( axis, settings.maxMinorSteps );
    setAxisMaxMajor( axis, settings.maxMajorSteps );
    setAxisScale( axis, qwtFromDateTime( settings.startDateTime ), 
        qwtFromDateTime( settings.endDateTime ) );

    replot();
}
