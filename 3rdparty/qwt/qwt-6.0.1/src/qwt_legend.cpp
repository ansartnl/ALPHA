/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_legend.h"
#include "qwt_legend_label.h"
#include "qwt_dyngrid_layout.h"
#include "qwt_math.h"
#include "qwt_plot_item.h"
#include <qapplication.h>
#include <qscrollbar.h>
#include <qscrollarea.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qstyleoption.h>

static void qwtRenderBackground( QPainter *painter,
    const QRectF &rect, const QWidget *widget )
{
    if ( widget->testAttribute( Qt::WA_StyledBackground ) )
    {
        QStyleOption opt;
        opt.initFrom( widget );
        opt.rect = rect.toAlignedRect();

        widget->style()->drawPrimitive(
            QStyle::PE_Widget, &opt, painter, widget);
    }
    else
    {
        const QBrush brush =
            widget->palette().brush( widget->backgroundRole() );

        painter->fillRect( rect, brush );
    }
}

class QwtLegendMap: public QMap<const QwtPlotItem *, QList<QWidget *> >
{
public:
    void removeWidget( const QWidget * );
    QList<QWidget *> legendWidgets( const QwtPlotItem * ) const;
    const QwtPlotItem *plotItem( const QWidget * ) const;
    QwtPlotItem *plotItem( const QWidget * );
};

void QwtLegendMap::removeWidget( const QWidget *widget )
{
    while ( QwtPlotItem *item = plotItem( widget ) )
    {
        QList<QWidget *> widgets = legendWidgets( item );
        widgets.removeAll( const_cast< QWidget *>( widget ) );
        insert( item, widgets );
    }
}

QwtPlotItem *QwtLegendMap::plotItem( const QWidget *widget )
{
    if ( widget == NULL )
        return NULL;

    for ( Iterator it = begin(); it != end(); ++it )
    {
        const QList< QWidget *> &widgets = it.value();
        for ( int i = 0; i < widgets.size(); i++ )
        {
            if ( widgets[i] == widget )
                return const_cast<QwtPlotItem *>( it.key() );
        }
    }

    return NULL;
}

const QwtPlotItem *QwtLegendMap::plotItem( const QWidget *widget ) const
{
    QwtLegendMap *that = const_cast<QwtLegendMap *>( this );
    return that->plotItem( widget );
}

QList<QWidget *> QwtLegendMap::legendWidgets( const QwtPlotItem *item ) const
{
    const ConstIterator it = find( item );
    if ( it != constEnd() )
        return it.value();

    return QList<QWidget *>();
}

class QwtLegend::PrivateData
{
public:
    PrivateData():
        itemMode( QwtLegendData::ReadOnly ),
        view( NULL )
    {
    }

    QwtLegendData::Mode itemMode;
    QwtLegendMap itemMap;

    class LegendView;
    LegendView *view;
};

class QwtLegend::PrivateData::LegendView: public QScrollArea
{
public:
    LegendView( QWidget *parent ):
        QScrollArea( parent )
    {
        contentsWidget = new QWidget( this );
        contentsWidget->setObjectName( "QwtLegendViewContents" );

        setWidget( contentsWidget );
        setWidgetResizable( false );

        viewport()->setObjectName( "QwtLegendViewport" );

        // QScrollArea::setWidget internally sets autoFillBackground to true
        // But we don't want a background.
        contentsWidget->setAutoFillBackground( false );
        viewport()->setAutoFillBackground( false );
    }

    virtual bool event( QEvent *event )
    {
        if ( event->type() == QEvent::PolishRequest )
        {
            setFocusPolicy( Qt::NoFocus );
        }

        if ( event->type() == QEvent::Resize )
        {
            // adjust the size to en/disable the scrollbars
            // before QScrollArea adjusts the viewport size

            const QRect cr = contentsRect();

            int w = cr.width();
            int h = contentsWidget->heightForWidth( cr.width() );
            if ( h > w )
            {
                w -= verticalScrollBar()->sizeHint().width();
                h = contentsWidget->heightForWidth( w );
            }

            contentsWidget->resize( w, h );
        }

        return QScrollArea::event( event );
    }

    virtual bool viewportEvent( QEvent *event )
    {
        bool ok = QScrollArea::viewportEvent( event );

        if ( event->type() == QEvent::Resize )
        {
            layoutContents();
        }
        return ok;
    }

    QSize viewportSize( int w, int h ) const
    {
        const int sbHeight = horizontalScrollBar()->sizeHint().height();
        const int sbWidth = verticalScrollBar()->sizeHint().width();

        const int cw = contentsRect().width();
        const int ch = contentsRect().height();

        int vw = cw;
        int vh = ch;

        if ( w > vw )
            vh -= sbHeight;

        if ( h > vh )
        {
            vw -= sbWidth;
            if ( w > vw && vh == ch )
                vh -= sbHeight;
        }
        return QSize( vw, vh );
    }

    void layoutContents()
    {
        const QwtDynGridLayout *tl = qobject_cast<QwtDynGridLayout *>(
            contentsWidget->layout() );
        if ( tl == NULL )
            return;

        const QSize visibleSize = viewport()->contentsRect().size();

        const int minW = int( tl->maxItemWidth() ) + 2 * tl->margin();

        int w = qMax( visibleSize.width(), minW );
        int h = qMax( tl->heightForWidth( w ), visibleSize.height() );

        const int vpWidth = viewportSize( w, h ).width();
        if ( w > vpWidth )
        {
            w = qMax( vpWidth, minW );
            h = qMax( tl->heightForWidth( w ), visibleSize.height() );
        }

        contentsWidget->resize( w, h );
    }

    QWidget *contentsWidget;
};

/*!
  Constructor

  \param parent Parent widget
*/
QwtLegend::QwtLegend( QWidget *parent ):
    QwtAbstractLegend( parent )
{
    setFrameStyle( NoFrame );

    d_data = new QwtLegend::PrivateData;

    d_data->view = new QwtLegend::PrivateData::LegendView( this );
    d_data->view->setObjectName( "QwtLegendView" );
    d_data->view->setFrameStyle( NoFrame );

    QwtDynGridLayout *gridLayout = new QwtDynGridLayout(
        d_data->view->contentsWidget );
    gridLayout->setAlignment( Qt::AlignHCenter | Qt::AlignTop );

    d_data->view->contentsWidget->installEventFilter( this );

    QVBoxLayout *layout = new QVBoxLayout( this );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( d_data->view );
}

//! Destructor
QwtLegend::~QwtLegend()
{
    delete d_data;
}

void QwtLegend::setMaxColumns( uint numColums )
{
    QwtDynGridLayout *tl = qobject_cast<QwtDynGridLayout *>(
        d_data->view->contentsWidget->layout() );
    if ( tl )
        tl->setMaxCols( numColums );
}

uint QwtLegend::maxColumns() const
{
    uint maxCols = 0;

    const QwtDynGridLayout *tl = qobject_cast<const QwtDynGridLayout *>(
        d_data->view->contentsWidget->layout() );
    if ( tl )
        maxCols = tl->maxCols();

    return maxCols;
}

/*!
  \brief Set the default mode for legend labels

  Legend labels will be constructed according to the
  attributes in a QwtLegendData object. When it doesn't
  contain a value for the QwtLegendData::ModeRole the
  label will be initialized with the default mode of the legend.

  \param mode Default item mode

  \sa itemMode(), QwtLegendData::value(), QwtPlotItem::legendData()
  \note Changing the mode doesn't have any effect on existing labels.
 */
void QwtLegend::setDefaultItemMode( QwtLegendData::Mode mode )
{
    d_data->itemMode = mode;
}

/*!
  \return Default item mode
  \sa setDefaultItemMode()
*/
QwtLegendData::Mode QwtLegend::defaultItemMode() const
{
    return d_data->itemMode;
}

/*!
  The contents widget is the only child of the viewport of 
  the internal QScrollArea  and the parent widget of all legend items.

  \return Container widget of the legend items
*/
QWidget *QwtLegend::contentsWidget()
{
    return d_data->view->contentsWidget;
}

/*!
  \return Horizontal scrollbar
  \sa verticalScrollBar()
*/
QScrollBar *QwtLegend::horizontalScrollBar() const
{
    return d_data->view->horizontalScrollBar();
}

/*!
  \return Vertical scrollbar
  \sa horizontalScrollBar()
*/
QScrollBar *QwtLegend::verticalScrollBar() const
{
    return d_data->view->verticalScrollBar();
}

/*!
  The contents widget is the only child of the viewport of 
  the internal QScrollArea  and the parent widget of all legend items.

  \return Container widget of the legend items

*/
const QWidget *QwtLegend::contentsWidget() const
{
    return d_data->view->contentsWidget;
}

/*!
  \brief Update the entries for a plot item

  \param plotItem Plot items
  \param data List of legend entry attributes of plot item
 */
void QwtLegend::updateLegend( const QwtPlotItem *plotItem, 
    const QList<QwtLegendData> &data )
{
    QList<QWidget *> widgetList = legendWidgets( plotItem );

    if ( widgetList.size() != data.size() )
    {
        QLayout *contentsLayout = d_data->view->contentsWidget->layout();

        while ( widgetList.size() > data.size() )
        {
            QWidget *w = widgetList.takeLast();

            contentsLayout->removeWidget( w );

            // updates might be triggered by signals from the legend widget
            // itself. So we better don't delete it here.

            w->hide();
            w->deleteLater();
        }

        for ( int i = widgetList.size(); i < data.size(); i++ )
        {
            QWidget *widget = createWidget( data[i] );

            if ( contentsLayout )
                contentsLayout->addWidget( widget );

            widgetList += widget;
        }

        if ( widgetList.isEmpty() )
        {
            d_data->itemMap.remove( plotItem );
        }
        else
        {
            d_data->itemMap.insert( plotItem, widgetList );
        }

        updateTabOrder();
    }
    
    for ( int i = 0; i < data.size(); i++ )
        updateWidget( widgetList[i], data[i] );
}

/*!
  \brief Create a widget to be inserted into the legend

  The default implementation returns a QwtLegendLabel.

  \param data Attributes of the legend entry
  
  \note updateWidget() will called soon after createWidget()
        with the same attributes.
 */
QWidget *QwtLegend::createWidget( const QwtLegendData &data ) const
{
    Q_UNUSED( data );

    QwtLegendLabel *label = new QwtLegendLabel();
    label->setItemMode( defaultItemMode() );

    connect( label, SIGNAL( clicked() ), SLOT( itemClicked() ) );
    connect( label, SIGNAL( checked( bool ) ), SLOT( itemChecked( bool ) ) );

    return label;
}

/*!
  \brief Update the widget 

  \param widget Usually a QwtLegendLabel
  \param data Attributes to be displayed

  \sa createWidget()
  \note When widget is no QwtLegendLabel updateWidget() does nothing.
 */
void QwtLegend::updateWidget( QWidget *widget, const QwtLegendData &data )
{
    QwtLegendLabel *label = qobject_cast<QwtLegendLabel *>( widget );
    if ( label )
    {
        label->setData( data );
        if ( !data.value( QwtLegendData::ModeRole ).isValid() )
        {
            // use the default mode, when there is no specific
            // hint from the legend data

            label->setItemMode( defaultItemMode() );
        }
    }
}

void QwtLegend::updateTabOrder()
{
    QLayout *contentsLayout = d_data->view->contentsWidget->layout();
    if ( contentsLayout )
    {
        // set tab focus chain

        QWidget *w = NULL;

        for ( int i = 0; i < contentsLayout->count(); i++ )
        {
            QLayoutItem *item = contentsLayout->itemAt( i );
            if ( w && item->widget() )
                QWidget::setTabOrder( w, item->widget() );

            w = item->widget();
        }
    }
}

//! Return a size hint.
QSize QwtLegend::sizeHint() const
{
    QSize hint = d_data->view->contentsWidget->sizeHint();
    hint += QSize( 2 * frameWidth(), 2 * frameWidth() );

    return hint;
}

/*!
  \return The preferred height, for the width w.
  \param width Width
*/
int QwtLegend::heightForWidth( int width ) const
{
    width -= 2 * frameWidth();

    int h = d_data->view->contentsWidget->heightForWidth( width );
    if ( h >= 0 )
        h += 2 * frameWidth();

    return h;
}


/*!
  Handle QEvent::ChildRemoved andQEvent::LayoutRequest events 
  for the contentsWidget().

  \param object Object to be filtered
  \param event Event
*/
bool QwtLegend::eventFilter( QObject *object, QEvent *event )
{
    if ( object == d_data->view->contentsWidget )
    {
        switch ( event->type() )
        {
            case QEvent::ChildRemoved:
            {
                const QChildEvent *ce = 
                    static_cast<const QChildEvent *>(event);
                if ( ce->child()->isWidgetType() )
                {
                    QWidget *w = static_cast< QWidget * >( ce->child() );
                    d_data->itemMap.removeWidget( w );
                }
                break;
            }
            case QEvent::LayoutRequest:
            {
                d_data->view->layoutContents();

                if ( parentWidget() && parentWidget()->layout() == NULL )
                {
                    /*
                       We want the parent widget ( usually QwtPlot ) to recalculate
                       its layout, when the contentsWidget has changed. But
                       because of the scroll view we have to forward the LayoutRequest
                       event manually.

                       We don't use updateGeometry() because it doesn't post LayoutRequest
                       events when the legend is hidden. But we want the
                       parent widget notified, so it can show/hide the legend
                       depending on its items.
                     */
                    QApplication::postEvent( parentWidget(),
                        new QEvent( QEvent::LayoutRequest ) );
                }                
                break;
            }
            default:
                break;
        }
    }

    return QwtAbstractLegend::eventFilter( object, event );
}

/*!
  Called internally when the legend has been clicked on.
  Emits a clicked() signal.
*/
void QwtLegend::itemClicked()
{
    QWidget *w = qobject_cast<QWidget *>( sender() );
    if ( w )
    {
        QwtPlotItem *plotItem = d_data->itemMap.plotItem( w );
        if ( plotItem )
        {
            const QList<QWidget *> widgetList =
                d_data->itemMap.legendWidgets( plotItem );

            const int index = widgetList.indexOf( w );
            if ( index >= 0 )
                Q_EMIT clicked( plotItem, index );
        }
    }
}

/*!
  Called internally when the legend has been checked
  Emits a checked() signal.
*/
void QwtLegend::itemChecked( bool on )
{
    QWidget *w = qobject_cast<QWidget *>( sender() );
    if ( w )
    {
        QwtPlotItem *plotItem = d_data->itemMap.plotItem( w );
        if ( plotItem )
        {
            const QList<QWidget *> widgetList =
                d_data->itemMap.legendWidgets( plotItem );

            const int index = widgetList.indexOf( w );
            if ( index >= 0 )
                Q_EMIT checked( plotItem, on, index );
        }
    }
}

/*!
  Render the legend into a given rectangle.

  \param painter Painter
  \param rect Bounding rectangle
  \param fillBackground When true, fill rect with the widget background 

  \sa renderLegend() is used by QwtPlotRenderer - not by QwtLegend itself
*/
void QwtLegend::renderLegend( QPainter *painter, 
    const QRectF &rect, bool fillBackground ) const
{
    if ( d_data->itemMap.isEmpty() )
        return;

    if ( fillBackground )
    {
        if ( autoFillBackground() ||
            testAttribute( Qt::WA_StyledBackground ) )
        {
            qwtRenderBackground( painter, rect, this );
        }
    }

    const QwtDynGridLayout *legendLayout = 
        qobject_cast<QwtDynGridLayout *>( contentsWidget()->layout() );
    if ( legendLayout == NULL )
        return;

    uint numCols = legendLayout->columnsForWidth( qFloor( rect.width() ) );
    QList<QRect> itemRects =
        legendLayout->layoutItems( rect.toRect(), numCols );

    int index = 0;

    for ( int i = 0; i < legendLayout->count(); i++ )
    {
        QLayoutItem *item = legendLayout->itemAt( i );
        QWidget *w = item->widget();
        if ( w )
        {
            painter->save();

            painter->setClipRect( itemRects[index] );
            renderItem( painter, w, itemRects[index], fillBackground );

            index++;
            painter->restore();
        }
    }
}

/*!
  Render a legend entry into a given rectangle.

  \param painter Painter
  \param widget Widget representing a legend entry
  \param rect Bounding rectangle
  \param fillBackground When true, fill rect with the widget background 

  \note When widget is not derived from QwtLegendLabel renderItem
        does nothing beside the background
*/
void QwtLegend::renderItem( QPainter *painter, 
    const QWidget *widget, const QRectF &rect, bool fillBackground ) const
{
    if ( fillBackground )
    {
        if ( widget->autoFillBackground() ||
            widget->testAttribute( Qt::WA_StyledBackground ) )
        {
            qwtRenderBackground( painter, rect, widget );
        }
    }

    const QwtLegendLabel *label = qobject_cast<const QwtLegendLabel *>( widget );
    if ( label )
    {
        // icon

        const QwtGraphic &icon = label->data().icon();
        const QSizeF sz = icon.defaultSize();

        const QRectF iconRect( rect.x() + label->margin(),
            rect.center().y() - 0.5 * sz.height(), 
            sz.width(), sz.height() );

        icon.render( painter, iconRect, Qt::KeepAspectRatio );

        // title

        QRectF titleRect = rect;
        titleRect.setX( iconRect.right() + 2 * label->spacing() );

        painter->setFont( label->font() );
        label->data().title().draw( painter, titleRect );
    }
}

/*!
  \return List of widgets associated to a plot item
  \sa legendWidget()
 */
QList<QWidget *> QwtLegend::legendWidgets( const QwtPlotItem *item ) const
{
    return d_data->itemMap.legendWidgets( item );
}

/*!
  \return First widget in the list of widgets associated to a plot item
  \note Almost all types of plot items have only one widget
*/
QWidget *QwtLegend::legendWidget( const QwtPlotItem *item ) const
{
    const QList<QWidget *> list = d_data->itemMap.legendWidgets( item );
    if ( list.isEmpty() )
        return NULL;

    return list[0];
}

//! \return True, when no plot item is inserted
bool QwtLegend::isEmpty() const
{
    return d_data->itemMap.isEmpty();
}

/*!
    Return the extent, that is needed for the scroll bars

    \param orientation Orientation ( 
    \return The width of the vertical scroll bar for Qt::Horizontal and v.v.
 */
int QwtLegend::scrollExtent( Qt::Orientation orientation ) const
{
    int extent = 0;

    if ( orientation == Qt::Horizontal )
        extent = verticalScrollBar()->sizeHint().width();
    else
        extent = horizontalScrollBar()->sizeHint().height();

    return extent;
}

