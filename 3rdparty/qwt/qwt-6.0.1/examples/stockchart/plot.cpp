#include "plot.h"
#include "legend.h"
#include "griditem.h"
#include "quotefactory.h"
#include <qwt_legend.h>
#include <qwt_plot_tradingcurve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_legend_label.h>

Plot::Plot( QWidget *parent ):
    QwtPlot( parent )
{
    setTitle( "Financial Chart 2010" );

    setAxisTitle( QwtPlot::xBottom, "2010" );
    setAxisTitle( QwtPlot::yLeft, QString( "Price [EUR]" ) );

    setAxisScale( QwtPlot::xBottom, 0.0, 365.0 );

#if 0
    QwtLegend *legend = new QwtLegend;
    legend->setDefaultItemMode( QwtLegendData::Checkable );
    insertLegend( legend, QwtPlot::RightLegend );
#else
    Legend *legend = new Legend;
    insertLegend( legend, QwtPlot::RightLegend );
#endif

    populate();

    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    QwtPlotZoomer* zoomer = new QwtPlotZoomer( canvas() );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
        Qt::RightButton );

    QwtPlotPanner *panner = new QwtPlotPanner( canvas() );
    panner->setMouseButton( Qt::MidButton );

    connect( legend, SIGNAL( checked( QwtPlotItem *, bool, int ) ),
        SLOT( showItem( QwtPlotItem *, bool ) ) );
}

void Plot::populate()
{
    GridItem *gridItem = new GridItem();
#if 0
    gridItem->setOrientations( Qt::Horizontal );
#endif
    gridItem->attach( this );

    const Qt::GlobalColor colors[] =
    {
        Qt::red,
        Qt::blue,
        Qt::darkCyan,
        Qt::darkMagenta,
        Qt::darkYellow
    };

    const int numColors = sizeof( colors ) / sizeof( colors[0] );

    for ( int i = 0; i < QuoteFactory::NumStocks; i++ )
    {
        QuoteFactory::Stock stock = static_cast<QuoteFactory::Stock>( i );

        QwtPlotTradingCurve *curve = new QwtPlotTradingCurve();
        curve->setTitle( QuoteFactory::title( stock ) );
        curve->setOrientation( Qt::Vertical );
        curve->setSamples( QuoteFactory::samples2010( stock ) );

        const Qt::GlobalColor color = colors[ i % numColors ];

        curve->setSymbolPen( QPen( color ) );
        curve->setSymbolBrush( QwtPlotTradingCurve::Decreasing, color );
        curve->setSymbolBrush( QwtPlotTradingCurve::Increasing, Qt::white );
        curve->attach( this );

        showItem( curve, true );
    }

    for ( int i = 0; i < 4; i++ )
    {
        QwtPlotMarker *marker = new QwtPlotMarker();
        marker->setTitle( QString( "Event %1" ).arg( i + 1 ) );
        marker->setLineStyle( QwtPlotMarker::VLine );
        marker->setItemAttribute( QwtPlotItem::Legend, true );

        QPen pen;
        pen.setStyle( Qt::DashLine );
        pen.setColor( colors[ i % numColors ] );

        marker->setLinePen( pen );
        marker->setValue( 77.0 * ( i + 1 ), 0.0 );
        marker->setVisible( false );

        marker->attach( this );
    }
}

void Plot::setMode( int style )
{
    QwtPlotTradingCurve::SymbolStyle symbolStyle =
        static_cast<QwtPlotTradingCurve::SymbolStyle>( style );

    QwtPlotItemList curves = itemList( QwtPlotItem::Rtti_PlotTradingCurve );
    for ( int i = 0; i < curves.size(); i++ )
    {
        QwtPlotTradingCurve *curve =
            static_cast<QwtPlotTradingCurve *>( curves[i] );
        curve->setSymbolStyle( symbolStyle );
    }

    replot();
}

void Plot::showItem( QwtPlotItem *item, bool on )
{
    item->setVisible( on );
    replot();
}

void Plot::exportPlot()
{
    QwtPlotRenderer renderer;
    renderer.setDiscardFlag( QwtPlotRenderer::DiscardBackground, false );
    renderer.exportTo( this, "stockchart.pdf" );
}
