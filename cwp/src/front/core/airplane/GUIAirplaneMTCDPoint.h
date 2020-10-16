#ifndef GUIAIRPLANEMTCDPOINT_H
#define GUIAIRPLANEMTCDPOINT_H

#include "front/core/GUIObject.h"
#include "Coordinate.h"

//! Class for the graphical representation of MTCD point
class CGUIAirplaneMTCDPoint: public CGUIObject
{
public:
    //! Constructor
    CGUIAirplaneMTCDPoint(QGraphicsItem* parent = 0);

    void SetCoord(const CCoord& coord);
    void SetPos(const QPointF& pos);
    void SetListMTCDText(const QStringList& sList, bool bAppend);

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    //@}

    //! Recalculate position
    void UpdatePosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);

private:
    CCoord      m_Coord;
    QStringList m_sTextMTCD;
};

#endif // GUIAIRPLANEMTCDPOINT_H
