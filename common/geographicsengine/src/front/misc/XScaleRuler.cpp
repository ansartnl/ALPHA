#include "XScaleRuler.h"
#include <QPen>
#include <QGraphicsScene>

const QString g_sKmText = ("km");
const QString g_sNMText = ("NM");

void CXScaleRuler::paint(QPainter &painter, const QRect& rect)
{
    if ( !m_isVisible )
        return;

    QPen qPenOld = painter.pen();

    QPen qPen(m_color);
    painter.setPen(qPen);

    const QString textUnits = m_units == Kilometers ? g_sKmText : g_sNMText;

    QFontMetrics fm = painter.fontMetrics();

    const int textUnitsWidth = fm.width(textUnits);
    const int textUnitsHeight = fm.height();
    const int bottomShift = 5;
    const int rightShift = 5;
    const int heightRuler = 4;
    const int heightTick = heightRuler+2;

    int x = rect.right() - rightShift - textUnitsWidth,
        y = rect.bottom() - bottomShift - heightRuler;

    // Draw interval rects.
    for ( int ic = 0; ic < m_intervalNumber; ic++ )
    {
        x -= m_intervalLength;
        painter.drawRect(x, y, m_intervalLength, heightRuler);
    }

    x = rect.right() - rightShift - textUnitsWidth,
    y = rect.bottom() - bottomShift;

    // Draw interval ticks.
    for ( int ic = 0; ic <= m_intervalNumber; ic++ )
    {
        painter.drawLine(x, y, x, y-heightTick);
        x -= m_intervalLength;
    }

    // Draw the most left interval with dash pen.
    {
        QPen qPenDash(m_color);
        qPenDash.setStyle(Qt::DotLine);
        qPenDash.setWidth(4);
        qPenDash.setCapStyle(Qt::FlatCap);
        painter.setPen(qPenDash);

        x = rect.right() - rightShift - textUnitsWidth - m_intervalNumber*m_intervalLength;
        y = rect.bottom() - bottomShift - heightRuler/2;

        painter.drawLine(x, y, x+m_intervalLength, y);

        // Restore pen
        painter.setPen(qPen);
    }

    // Draw text labels ...

    QString textLabel = ("0");
    qreal intervalDist = 0.0;

    x = rect.right() - rightShift - textUnitsWidth - (m_intervalNumber-1)*m_intervalLength;
    y = rect.bottom() - bottomShift - heightTick - 2;

    QRect rectLabel;
    // Draw from "0" .. "n-1" to the right
    for ( int ic = 1; ic < m_intervalNumber; ic++ )
    {
        QRect rectLabel = fm.boundingRect(textLabel);

        QPoint centerPoint = rectLabel.center();
        rectLabel.translate(x-centerPoint.x(), y-centerPoint.y()-rectLabel.height()/2);
        rectLabel.adjust(-2, 0, 2, 0);

        painter.drawText(rectLabel, Qt::AlignCenter, textLabel);

        x += m_intervalLength;

        intervalDist += m_intervalDist;
        textLabel = formatLabel(intervalDist);
    }

    x = rect.right() - rightShift - textUnitsWidth;
    y = rect.bottom() - bottomShift - heightTick - 2;

    // Draw "n" label on the right
    {
        QRect rectLabel = fm.boundingRect(textLabel);

        QPoint centerPoint = rectLabel.center();
        rectLabel.translate(x-centerPoint.x()-rectLabel.width()/2,
                            y-centerPoint.y()-rectLabel.height()/2);
        rectLabel.adjust(-6, 0, -2, 0);

        painter.drawText(rectLabel, Qt::AlignCenter, textLabel);
    }

    x = rect.right() - rightShift - textUnitsWidth - m_intervalNumber*m_intervalLength;
    y = rect.bottom() - bottomShift - heightTick - 2;

    // Draw left label.
    {
        textLabel = formatLabel(m_intervalDist);

        QRect rectLabel = fm.boundingRect(textLabel);

        QPoint centerPoint = rectLabel.center();
        rectLabel.translate(x-centerPoint.x(), y-centerPoint.y()-rectLabel.height()/2);
        rectLabel.adjust(-2, 0, 2, 0);

        painter.drawText(rectLabel, Qt::AlignCenter, textLabel);
    }

    // Draw units label.

    x = rect.right() - rightShift - textUnitsWidth;
    painter.drawText(QRect(x, y-textUnitsHeight, textUnitsWidth, textUnitsHeight),
                     Qt::AlignVCenter | Qt::AlignLeft, textUnits);

    painter.setPen(qPenOld);
}

QString CXScaleRuler::formatLabel(qreal value) const
{
    if ( value < 1.0 && value != 0.0 )
        return QString::number(value, 'f', 2);

    return QString::number((int)value);
}
