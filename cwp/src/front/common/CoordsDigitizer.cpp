#include "CoordsDigitizer.h"
#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QApplication>
#include <QtCore/QTextStream>
#include <QtGui/QClipboard>
#include <QMessageBox>
#include <QtCore/QMimeData>

#include "src/front/core/GUIInterface.h"

CoordsDigitizer::CoordsDigitizer(QGraphicsItem *parent)
    : QGraphicsItem(parent), mMultiplePicking(true)
{
    setZValue(gui::Zdigitizer);
}

QRectF CoordsDigitizer::boundingRect() const
{
    return mPath.boundingRect();
}

QPainterPath CoordsDigitizer::shape() const
{
    return mPath;
}

void CoordsDigitizer::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
    painter->save();
    QPen qPen( Qt::green );
    painter->setPen(qPen);
    painter->drawPath(mPath);
    painter->restore();
}

void CoordsDigitizer::updatePosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    prepareGeometryChange();

    mPath = QPainterPath();

    foreach ( const CCoord &coord, mPoints )
    {
        QPointF point = QPointF((coord.GetX() - qllXMin) * dScale,
                                (qllYMax - coord.GetY()) * dScale);

        qreal shift = 2;
        // Draw cross
        mPath.moveTo(point.x()-shift, point.y()+shift);
        mPath.lineTo(point.x()+shift, point.y()-shift);
        mPath.moveTo(point.x()+shift, point.y()+shift);
        mPath.lineTo(point.x()-shift, point.y()-shift);
    }

    update();
}

void CoordsDigitizer::toClipboard() const
{
    QClipboard *clipboard = QApplication::clipboard();

    QString textClp;
    QTextStream stream(&textClp);
    foreach ( const CCoord &coord, mPoints )
    {
        stream << QString("%1\t%2").arg(coord.GetLatitude().GetAngle(), 0, 'f', 8)
                  .arg(coord.GetLongitude().GetAngle(), 0, 'f', 8) << endl;
    }

    clipboard->setText(textClp);
}

CoordsDigitizer::TCoordList CoordsDigitizer::GetPointsFromClipboard(QWidget *parent)
{
    TCoordList coords;

    QClipboard *clipboard = QApplication::clipboard();
    if ( !clipboard->mimeData() || !clipboard->mimeData()->hasText() )
    {
        QMessageBox::information(parent, QObject::tr("Paste from Clipboard"),
                                 QObject::tr("Unknown data in the clipboard"));
        return coords;
    }

    QString textClp = clipboard->text();
    QTextStream stream(&textClp);
    while ( !stream.atEnd() )
    {
        QString lat, lon;
        stream >> lat >> lon;
        if ( QTextStream::Ok != stream.status() )
            continue;

        coords << CCoord(lon.toDouble(), lat.toDouble());
    }

    return coords;
}
