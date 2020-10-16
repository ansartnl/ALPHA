/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_painter.h"
#include "qwt_math.h"
#include "qwt_clipper.h"
#include "qwt_color_map.h"
#include "qwt_scale_map.h"
#include <qwindowdefs.h>
#include <qwidget.h>
#include <qframe.h>
#include <qrect.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qpaintdevice.h>
#include <qpixmap.h>
#include <qstyle.h>
#include <qtextdocument.h>
#include <qabstracttextdocumentlayout.h>
#include <qstyleoption.h>
#include <qpaintengine.h>
#include <qapplication.h>
#include <qdesktopwidget.h>

bool QwtPainter::d_polylineSplitting = true;
bool QwtPainter::d_roundingAlignment = true;

static inline bool qwtIsClippingNeeded( 
    const QPainter *painter, QRectF &clipRect )
{
    bool doClipping = false;
    const QPaintEngine *pe = painter->paintEngine();
    if ( pe && pe->type() == QPaintEngine::SVG )
    {
        // The SVG paint engine ignores any clipping,

        if ( painter->hasClipping() )
        {
            doClipping = true;
            clipRect = painter->clipRegion().boundingRect();
        }
    }

    return doClipping;
}

template <class T>
static inline void qwtDrawPolyline( QPainter *painter,
    const T *points, int pointCount, bool polylineSplitting )
{
    bool doSplit = false;
    if ( polylineSplitting )
    {
        const QPaintEngine *pe = painter->paintEngine();
        if ( pe && pe->type() == QPaintEngine::Raster )
        {
            /*
                The raster paint engine seems to use some algo with O(n*n).
                ( Qt 4.3 is better than Qt 4.2, but remains unacceptable)
                To work around this problem, we have to split the polygon into
                smaller pieces.
             */
            doSplit = true;
        }
    }

    if ( doSplit )
    {
        const int splitSize = 20;
        for ( int i = 0; i < pointCount; i += splitSize )
        {
            const int n = qMin( splitSize + 1, pointCount - i );
            painter->drawPolyline( points + i, n );
        }
    }
    else
        painter->drawPolyline( points, pointCount );
}

static inline void qwtUnscaleFont( QPainter *painter )
{
    if ( painter->font().pixelSize() >= 0 )
        return;

    static QSize screenResolution;
    if ( !screenResolution.isValid() )
    {
        QDesktopWidget *desktop = QApplication::desktop();
        if ( desktop )
        {
            screenResolution.setWidth( desktop->logicalDpiX() );
            screenResolution.setHeight( desktop->logicalDpiY() );
        }
    }

    const QPaintDevice *pd = painter->device();
    if ( pd->logicalDpiX() != screenResolution.width() ||
        pd->logicalDpiY() != screenResolution.height() )
    {
        QFont pixelFont( painter->font(), QApplication::desktop() );
        pixelFont.setPixelSize( QFontInfo( pixelFont ).pixelSize() );

        painter->setFont( pixelFont );
    }
}

/*!
  Check is the application is running with the X11 graphics system
  that has some special capabilities that can be used for incremental
  painting to a widget.

  \return True, when the graphicssystem is X11
*/
bool QwtPainter::isX11GraphicsSystem()
{
    static int onX11 = -1;
    if ( onX11 < 0 )
    {
        QPixmap pm( 1, 1 );
        QPainter painter( &pm );

        onX11 = ( painter.paintEngine()->type() == QPaintEngine::X11 ) ? 1 : 0;
    }

    return onX11 == 1;
}

/*!
  Check if the painter is using a paint engine, that aligns
  coordinates to integers. Today these are all paint engines
  beside QPaintEngine::Pdf and QPaintEngine::SVG.

  If we have an integer based paint engine it is also
  checked if the painter has a transformation matrix,
  that rotates or scales.

  \param  painter Painter
  \return true, when the painter is aligning

  \sa setRoundingAlignment()
*/
bool QwtPainter::isAligning( QPainter *painter )
{
    if ( painter && painter->isActive() )
    {
        switch ( painter->paintEngine()->type() )
        {
            case QPaintEngine::Pdf:
            case QPaintEngine::SVG:
                return false;

            default:;
        }

        const QTransform tr = painter->transform();
        if ( tr.isRotating() || tr.isScaling() )
        {
            // we might have to check translations too
            return false;
        }
    }

    return true;
}

/*!
  Enable whether coordinates should be rounded, before they are painted
  to a paint engine that floors to integer values. For other paint engines
  this ( Pdf, SVG ), this flag has no effect.
  QwtPainter stores this flag only, the rounding itsself is done in 
  the painting code ( f.e the plot items ).

  The default setting is true. 

  \sa roundingAlignment(), isAligning()
*/
void QwtPainter::setRoundingAlignment( bool enable )
{
    d_roundingAlignment = enable;
}

/*!
  \brief En/Disable line splitting for the raster paint engine

  The raster paint engine paints polylines of many points
  much faster when they are splitted in smaller chunks.

  \sa polylineSplitting()
*/
void QwtPainter::setPolylineSplitting( bool enable )
{
    d_polylineSplitting = enable;
}

//! Wrapper for QPainter::drawPath()
void QwtPainter::drawPath( QPainter *painter, const QPainterPath &path )
{
    painter->drawPath( path );
}

//! Wrapper for QPainter::drawRect()
void QwtPainter::drawRect( QPainter *painter, double x, double y, double w, double h )
{
    drawRect( painter, QRectF( x, y, w, h ) );
}

//! Wrapper for QPainter::drawRect()
void QwtPainter::drawRect( QPainter *painter, const QRectF &rect )
{
    const QRectF r = rect;

    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    if ( deviceClipping )
    {
        if ( !clipRect.intersects( r ) )
            return;

        if ( !clipRect.contains( r ) )
        {
            fillRect( painter, r & clipRect, painter->brush() );

            painter->save();
            painter->setBrush( Qt::NoBrush );
            drawPolyline( painter, QPolygonF( r ) );
            painter->restore();

            return;
        }
    }

    painter->drawRect( r );
}

//! Wrapper for QPainter::fillRect()
void QwtPainter::fillRect( QPainter *painter,
    const QRectF &rect, const QBrush &brush )
{
    if ( !rect.isValid() )
        return;

    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    /*
      Performance of Qt4 is horrible for non trivial brushs. Without
      clipping expect minutes or hours for repainting large rects
      (might result from zooming)
    */

    if ( deviceClipping )
        clipRect &= painter->window();
    else
        clipRect = painter->window();

    if ( painter->hasClipping() )
        clipRect &= painter->clipRegion().boundingRect();

    QRectF r = rect;
    if ( deviceClipping )
        r = r.intersect( clipRect );

    if ( r.isValid() )
        painter->fillRect( r, brush );
}

//! Wrapper for QPainter::drawPie()
void QwtPainter::drawPie( QPainter *painter, const QRectF &rect,
    int a, int alen )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );
    if ( deviceClipping && !clipRect.contains( rect ) )
        return;

    painter->drawPie( rect, a, alen );
}

//! Wrapper for QPainter::drawEllipse()
void QwtPainter::drawEllipse( QPainter *painter, const QRectF &rect )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    if ( deviceClipping && !clipRect.contains( rect ) )
        return;

    painter->drawEllipse( rect );
}

//! Wrapper for QPainter::drawText()
void QwtPainter::drawText( QPainter *painter, double x, double y,
        const QString &text )
{
    drawText( painter, QPointF( x, y ), text );
}

//! Wrapper for QPainter::drawText()
void QwtPainter::drawText( QPainter *painter, const QPointF &pos,
        const QString &text )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    if ( deviceClipping && !clipRect.contains( pos ) )
        return;


    painter->save();
    qwtUnscaleFont( painter );
    painter->drawText( pos, text );
    painter->restore();
}

//! Wrapper for QPainter::drawText()
void QwtPainter::drawText( QPainter *painter,
    double x, double y, double w, double h,
    int flags, const QString &text )
{
    drawText( painter, QRectF( x, y, w, h ), flags, text );
}

//! Wrapper for QPainter::drawText()
void QwtPainter::drawText( QPainter *painter, const QRectF &rect,
        int flags, const QString &text )
{
    painter->save();
    qwtUnscaleFont( painter );
    painter->drawText( rect, flags, text );
    painter->restore();
}

#ifndef QT_NO_RICHTEXT

/*!
  Draw a text document into a rectangle

  \param painter Painter
  \param rect Traget rectangle
  \param flags Alignments/Text flags, see QPainter::drawText()
  \param text Text document
*/
void QwtPainter::drawSimpleRichText( QPainter *painter, const QRectF &rect,
    int flags, const QTextDocument &text )
{
    QTextDocument *txt = text.clone();

    painter->save();

    painter->setFont( txt->defaultFont() );
    qwtUnscaleFont( painter );

    txt->setDefaultFont( painter->font() );
    txt->setPageSize( QSizeF( rect.width(), QWIDGETSIZE_MAX ) );

    QAbstractTextDocumentLayout* layout = txt->documentLayout();

    const double height = layout->documentSize().height();
    double y = rect.y();
    if ( flags & Qt::AlignBottom )
        y += ( rect.height() - height );
    else if ( flags & Qt::AlignVCenter )
        y += ( rect.height() - height ) / 2;

    QAbstractTextDocumentLayout::PaintContext context;
    context.palette.setColor( QPalette::Text, painter->pen().color() );

    painter->translate( rect.x(), y );
    layout->draw( painter, context );

    painter->restore();
    delete txt;
}

#endif // !QT_NO_RICHTEXT


//! Wrapper for QPainter::drawLine()
void QwtPainter::drawLine( QPainter *painter,
    const QPointF &p1, const QPointF &p2 )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    if ( deviceClipping &&
        !( clipRect.contains( p1 ) && clipRect.contains( p2 ) ) )
    {
        QPolygonF polygon;
        polygon += p1;
        polygon += p2;
        drawPolyline( painter, polygon );
        return;
    }

    painter->drawLine( p1, p2 );
}

//! Wrapper for QPainter::drawPolygon()
void QwtPainter::drawPolygon( QPainter *painter, const QPolygonF &polygon )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    QPolygonF cpa = polygon;
    if ( deviceClipping )
        cpa = QwtClipper::clipPolygonF( clipRect, polygon );

    painter->drawPolygon( cpa );
}

//! Wrapper for QPainter::drawPolyline()
void QwtPainter::drawPolyline( QPainter *painter, const QPolygonF &polygon )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    QPolygonF cpa = polygon;
    if ( deviceClipping )
        cpa = QwtClipper::clipPolygonF( clipRect, cpa );

    qwtDrawPolyline<QPointF>( painter,
        cpa.constData(), cpa.size(), d_polylineSplitting );
}

//! Wrapper for QPainter::drawPolyline()
void QwtPainter::drawPolyline( QPainter *painter,
    const QPointF *points, int pointCount )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    if ( deviceClipping )
    {
        QPolygonF polygon( pointCount );
        qMemCopy( polygon.data(), points, pointCount * sizeof( QPointF ) );

        polygon = QwtClipper::clipPolygonF( clipRect, polygon );
        qwtDrawPolyline<QPointF>( painter,
            polygon.constData(), polygon.size(), d_polylineSplitting );
    }
    else
    {
        qwtDrawPolyline<QPointF>( painter, points, pointCount, d_polylineSplitting );
    }
}

//! Wrapper for QPainter::drawPolygon()
void QwtPainter::drawPolygon( QPainter *painter, const QPolygon &polygon )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    QPolygon cpa = polygon;
    if ( deviceClipping )
        cpa = QwtClipper::clipPolygon( clipRect, polygon );

    painter->drawPolygon( cpa );
}

//! Wrapper for QPainter::drawPolyline()
void QwtPainter::drawPolyline( QPainter *painter, const QPolygon &polygon )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    QPolygon cpa = polygon;
    if ( deviceClipping )
        cpa = QwtClipper::clipPolygon( clipRect, cpa );

    qwtDrawPolyline<QPoint>( painter,
        cpa.constData(), cpa.size(), d_polylineSplitting );
}

//! Wrapper for QPainter::drawPolyline()
void QwtPainter::drawPolyline( QPainter *painter,
    const QPoint *points, int pointCount )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    if ( deviceClipping )
    {
        QPolygon polygon( pointCount );
        qMemCopy( polygon.data(), points, pointCount * sizeof( QPoint ) );

        polygon = QwtClipper::clipPolygon( clipRect, polygon );
        qwtDrawPolyline<QPoint>( painter,
            polygon.constData(), polygon.size(), d_polylineSplitting );
    }
    else
        qwtDrawPolyline<QPoint>( painter, points, pointCount, d_polylineSplitting );
}

//! Wrapper for QPainter::drawPoint()
void QwtPainter::drawPoint( QPainter *painter, const QPointF &pos )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    if ( deviceClipping && !clipRect.contains( pos ) )
        return;

    painter->drawPoint( pos );
}

//! Wrapper for QPainter::drawPoint()
void QwtPainter::drawPoint( QPainter *painter, const QPoint &pos )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    if ( deviceClipping )
    {
        const int minX = qCeil( clipRect.left() );
        const int maxX = qFloor( clipRect.right() );
        const int minY = qCeil( clipRect.top() );
        const int maxY = qFloor( clipRect.bottom() );

        if ( pos.x() < minX || pos.x() > maxX 
            || pos.y() < minY || pos.y() > maxY )
        {
            return;
        }
    }

    painter->drawPoint( pos );
}

//! Wrapper for QPainter::drawPoints()
void QwtPainter::drawPoints( QPainter *painter, 
    const QPoint *points, int pointCount )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    if ( deviceClipping )
    {
        const int minX = qCeil( clipRect.left() );
        const int maxX = qFloor( clipRect.right() );
        const int minY = qCeil( clipRect.top() );
        const int maxY = qFloor( clipRect.bottom() );

        const QRect r( minX, minY, maxX - minX, maxY - minY );

        QPolygon clippedPolygon( pointCount );
        QPoint *clippedData = clippedPolygon.data();

        int numClippedPoints = 0;
        for ( int i = 0; i < pointCount; i++ )
        {
            if ( r.contains( points[i] ) )
                clippedData[ numClippedPoints++ ] = points[i];
        }
        painter->drawPoints( clippedData, numClippedPoints );
    }
    else
    {
        painter->drawPoints( points, pointCount );
    }
}

//! Wrapper for QPainter::drawPoints()
void QwtPainter::drawPoints( QPainter *painter, 
    const QPointF *points, int pointCount )
{
    QRectF clipRect;
    const bool deviceClipping = qwtIsClippingNeeded( painter, clipRect );

    if ( deviceClipping )
    {
        QPolygonF clippedPolygon( pointCount );
        QPointF *clippedData = clippedPolygon.data();

        int numClippedPoints = 0;
        for ( int i = 0; i < pointCount; i++ )
        {
            if ( clipRect.contains( points[i] ) )
                clippedData[ numClippedPoints++ ] = points[i];
        }
        painter->drawPoints( clippedData, numClippedPoints );
    }
    else
    {
        painter->drawPoints( points, pointCount );
    }
}

//! Wrapper for QPainter::drawImage()
void QwtPainter::drawImage( QPainter *painter,
    const QRectF &rect, const QImage &image )
{
    const QRect alignedRect = rect.toAlignedRect();

    if ( alignedRect != rect )
    {
        const QRectF clipRect = rect.adjusted( 0.0, 0.0, -1.0, -1.0 );

        painter->save();
        painter->setClipRect( clipRect, Qt::IntersectClip );
        painter->drawImage( alignedRect, image );
        painter->restore();
    }
    else
    {
        painter->drawImage( alignedRect, image );
    }
}

//! Wrapper for QPainter::drawPixmap()
void QwtPainter::drawPixmap( QPainter *painter,
    const QRectF &rect, const QPixmap &pixmap )
{
    const QRect alignedRect = rect.toAlignedRect();

    if ( alignedRect != rect )
    {
        const QRectF clipRect = rect.adjusted( 0.0, 0.0, -1.0, -1.0 );

        painter->save();
        painter->setClipRect( clipRect, Qt::IntersectClip );
        painter->drawPixmap( alignedRect, pixmap );
        painter->restore();
    }
    else
    {
        painter->drawPixmap( alignedRect, pixmap );
    }
}

//! Draw a focus rectangle on a widget using its style.
void QwtPainter::drawFocusRect( QPainter *painter, QWidget *widget )
{
    drawFocusRect( painter, widget, widget->rect() );
}

//! Draw a focus rectangle on a widget using its style.
void QwtPainter::drawFocusRect( QPainter *painter, QWidget *widget,
    const QRect &rect )
{
    QStyleOptionFocusRect opt;
    opt.init( widget );
    opt.rect = rect;
    opt.state |= QStyle::State_HasFocus;

    widget->style()->drawPrimitive( QStyle::PE_FrameFocusRect,
        &opt, painter, widget );
}

/*!
  Draw a frame with rounded borders

  \param painter Painter
  \param rect Frame rectangle
  \param xRadius x-radius of the ellipses defining the corners
  \param yRadius y-radius of the ellipses defining the corners
  \param palette QPalette::WindowText is used for plain borders
                 QPalette::Dark and QPalette::Light for raised
                 or sunken borders
  \param lineWidth Line width
  \param frameStyle bitwise OR´ed value of QFrame::Shape and QFrame::Shadow
*/

void QwtPainter::drawRoundedFrame( QPainter *painter, 
    const QRectF &rect, double xRadius, double yRadius, 
    const QPalette &palette, int lineWidth, int frameStyle )
{
    painter->save();
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->setBrush( Qt::NoBrush );

    double lw2 = lineWidth * 0.5;
    QRectF r = rect.adjusted( lw2, lw2, -lw2, -lw2 );

    QPainterPath path;
    path.addRoundedRect( r, xRadius, yRadius );

    enum Style
    {
        Plain,
        Sunken,
        Raised
    };

    Style style = Plain;
    if ( (frameStyle & QFrame::Sunken) == QFrame::Sunken )
        style = Sunken;
    else if ( (frameStyle & QFrame::Raised) == QFrame::Raised )
        style = Raised;

    if ( style != Plain && path.elementCount() == 17 )
    {
        // move + 4 * ( cubicTo + lineTo )
        QPainterPath pathList[8];
        
        for ( int i = 0; i < 4; i++ )
        {
            const int j = i * 4 + 1;
            
            pathList[ 2 * i ].moveTo(
                path.elementAt(j - 1).x, path.elementAt( j - 1 ).y
            );  
            
            pathList[ 2 * i ].cubicTo(
                path.elementAt(j + 0).x, path.elementAt(j + 0).y,
                path.elementAt(j + 1).x, path.elementAt(j + 1).y,
                path.elementAt(j + 2).x, path.elementAt(j + 2).y );
                
            pathList[ 2 * i + 1 ].moveTo(
                path.elementAt(j + 2).x, path.elementAt(j + 2).y
            );  
            pathList[ 2 * i + 1 ].lineTo(
                path.elementAt(j + 3).x, path.elementAt(j + 3).y
            );  
        }   

        QColor c1( palette.color( QPalette::Dark ) );
        QColor c2( palette.color( QPalette::Light ) );

        if ( style == Raised )
            qSwap( c1, c2 );

        for ( int i = 0; i < 4; i++ )
        {
            QRectF r = pathList[2 * i].controlPointRect();

            QPen arcPen;
            arcPen.setWidth( lineWidth );

            QPen linePen;
            linePen.setWidth( lineWidth );

            switch( i )
            {
                case 0:
                {
                    arcPen.setColor( c1 );
                    linePen.setColor( c1 );
                    break;
                }
                case 1:
                {
                    QLinearGradient gradient;
                    gradient.setStart( r.topLeft() );
                    gradient.setFinalStop( r.bottomRight() );
                    gradient.setColorAt( 0.0, c1 );
                    gradient.setColorAt( 1.0, c2 );

                    arcPen.setBrush( gradient );
                    linePen.setColor( c2 );
                    break;
                }
                case 2:
                {
                    arcPen.setColor( c2 );
                    linePen.setColor( c2 );
                    break;
                }
                case 3:
                {
                    QLinearGradient gradient;

                    gradient.setStart( r.bottomRight() );
                    gradient.setFinalStop( r.topLeft() );
                    gradient.setColorAt( 0.0, c2 );
                    gradient.setColorAt( 1.0, c1 );

                    arcPen.setBrush( gradient );
                    linePen.setColor( c1 );
                    break;
                }
            }


            painter->setPen( arcPen );
            painter->drawPath( pathList[ 2 * i] );

            painter->setPen( linePen );
            painter->drawPath( pathList[ 2 * i + 1] );
        }
    }
    else
    {
        QPen pen( palette.color( QPalette::WindowText ), lineWidth );
        painter->setPen( pen );
        painter->drawPath( path );
    }

    painter->restore();
}

/*!
  Draw a color bar into a rectangle

  \param painter Painter
  \param colorMap Color map
  \param interval Value range
  \param scaleMap Scale map
  \param orientation Orientation
  \param rect Traget rectangle
*/
void QwtPainter::drawColorBar( QPainter *painter,
        const QwtColorMap &colorMap, const QwtInterval &interval,
        const QwtScaleMap &scaleMap, Qt::Orientation orientation,
        const QRectF &rect )
{
    QVector<QRgb> colorTable;
    if ( colorMap.format() == QwtColorMap::Indexed )
        colorTable = colorMap.colorTable( interval );

    QColor c;

    const QRect devRect = rect.toAlignedRect();

    /*
      We paint to a pixmap first to have something scalable for printing
      ( f.e. in a Pdf document )
     */

    QPixmap pixmap( devRect.size() );
    QPainter pmPainter( &pixmap );
    pmPainter.translate( -devRect.x(), -devRect.y() );

    if ( orientation == Qt::Horizontal )
    {
        QwtScaleMap sMap = scaleMap;
        sMap.setPaintInterval( rect.left(), rect.right() );

        for ( int x = devRect.left(); x <= devRect.right(); x++ )
        {
            const double value = sMap.invTransform( x );

            if ( colorMap.format() == QwtColorMap::RGB )
                c.setRgb( colorMap.rgb( interval, value ) );
            else
                c = colorTable[colorMap.colorIndex( interval, value )];

            pmPainter.setPen( c );
            pmPainter.drawLine( x, devRect.top(), x, devRect.bottom() );
        }
    }
    else // Vertical
    {
        QwtScaleMap sMap = scaleMap;
        sMap.setPaintInterval( rect.bottom(), rect.top() );

        for ( int y = devRect.top(); y <= devRect.bottom(); y++ )
        {
            const double value = sMap.invTransform( y );

            if ( colorMap.format() == QwtColorMap::RGB )
                c.setRgb( colorMap.rgb( interval, value ) );
            else
                c = colorTable[colorMap.colorIndex( interval, value )];

            pmPainter.setPen( c );
            pmPainter.drawLine( devRect.left(), y, devRect.right(), y );
        }
    }
    pmPainter.end();

    drawPixmap( painter, rect, pixmap );
}

#if QT_VERSION >= 0x050000

static inline void fillRect(QPainter *painter, const QRect &rect, const QBrush &brush)
{
    if ( brush.style() == Qt::TexturePattern ) 
    {
        painter->setClipRect( rect );
        painter->drawTiledPixmap(rect, brush.texture(), rect.topLeft());
    } 
    else if (brush.gradient()
        && brush.gradient()->coordinateMode() == QGradient::ObjectBoundingMode) 
    {
        painter->save();
        painter->setClipRect( rect );
        painter->fillRect(0, 0, painter->device()->width(), 
            painter->device()->height(), brush);
        painter->restore();
    } 
    else 
    {
        painter->fillRect(rect, brush);
    }
}

#endif

void QwtPainter::fillPixmap( const QWidget *widget, 
    QPixmap &pixmap, const QPoint &offset )
{
#if QT_VERSION >= 0x050000

    // Qwt 5.0.0 Alpha doesn't offer a replacement for QPixmap::fill
#if 1
    const QRect rect( offset, pixmap.size() );

    QPainter painter( &pixmap );
    painter.translate( -offset );

    const QBrush autoFillBrush = 
        widget->palette().brush( widget->backgroundRole() );

    if ( !( widget->autoFillBackground() && autoFillBrush.isOpaque() ) ) 
    {
        const QBrush bg = widget->palette().brush( QPalette::Window );
        fillRect( &painter, rect, bg);
    }

    if ( widget->autoFillBackground() )
        fillRect( &painter, rect, autoFillBrush);

    if ( widget->testAttribute(Qt::WA_StyledBackground) ) 
    {
        painter.setClipRegion( rect );

        QStyleOption opt;
        opt.initFrom( widget );
        widget->style()->drawPrimitive( QStyle::PE_Widget, 
            &opt, &painter, widget );
    }
#endif

#else
    pixmap.fill( widget, offset );
#endif
}

