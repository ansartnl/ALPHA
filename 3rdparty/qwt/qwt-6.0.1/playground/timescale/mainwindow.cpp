#include "plot.h"
#include "panel.h"
#include "mainwindow.h"
#include "timedate.h"
#include <qwt_scale_widget.h>
#include <qlayout.h>

MainWindow::MainWindow( QWidget *parent ):
    QMainWindow( parent )
{
    Settings settings;
    settings.startDateTime = QDateTime( QDate( 2012, 3, 10 ), QTime( 23, 59, 0 ) );
    settings.endDateTime = QDateTime( QDate( 2012, 3, 11 ) );
    settings.maxMajorSteps = 10;
    settings.maxMinorSteps = 8;
    settings.maxWeeks = -1;

    d_plot = new Plot();
    d_panel = new Panel();
    d_panel->setSettings( settings );

    QWidget *box = new QWidget( this );

    QHBoxLayout *layout = new QHBoxLayout( box );
    layout->addWidget( d_plot, 10 );
    layout->addWidget( d_panel );

    setCentralWidget( box );

    updatePlot();

    connect( d_panel, SIGNAL( edited() ), SLOT( updatePlot() ) );
    connect( d_plot->axisWidget( QwtPlot::yLeft ), 
        SIGNAL( scaleDivChanged() ), SLOT( updatePanel() ) );
}

void MainWindow::updatePlot()
{
    d_plot->blockSignals( true );
    d_plot->applySettings( d_panel->settings() );
    d_plot->blockSignals( false );
}

void MainWindow::updatePanel()
{
    const QwtScaleDiv scaleDiv = d_plot->axisScaleDiv( QwtPlot::yLeft );

    Settings settings = d_panel->settings();
    settings.startDateTime = qwtToDateTime( scaleDiv.lowerBound() );
    settings.endDateTime = qwtToDateTime( scaleDiv.upperBound() );

    d_panel->setSettings( settings );
}
