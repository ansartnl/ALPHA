#ifndef GUIGROUNDVEHICLEPOINT_H
#define GUIGROUNDVEHICLEPOINT_H

#include <QGraphicsItem>
#include "back/ground/GroundCoordinate.h"
//#include "common/Coordinate.h"

class GUIGroundVehicle;

class GUIGroundVehiclePoint : public QGraphicsItem
{
public:
    GUIGroundVehiclePoint(QGraphicsItem * parent = 0);
    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget);
    //@}

    //! Set new position at point
    /*!
      \param point a QPointF
      \param coord a CCoord airplane coordinate
    */
    void SetPosition(QPointF point, CCoord coord);
    //! Recalculate position
    void UpdatePosition(qint64 qllXMin, qint64 qllYMax, double dScale);

    void SetVehicle(GUIGroundVehicle* pVehicle);

    inline const CCoord& coord() const
    {return m_Coord;}

protected:
    //! Inherited from QGraphicsTextItem
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    int                m_iPenWidth;
    QColor            m_color;
    QPainterPath    m_painterPath;
    //QPixmap            m_image;
    QPointF            m_CenterPoint;
    CCoord            m_Coord;

    GUIGroundVehicle*    m_pVehicle;
};

#endif // GUIGROUNDVEHICLEPOINT_H
