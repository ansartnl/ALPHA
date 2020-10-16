#ifndef GUIGROUNDBUILDING_H
#define GUIGROUNDBUILDING_H

#include <QGraphicsItem>
#include "back/ground/GroundBuilding.h"
#include "front/core/GUIInterface.h"

//! Class for the graphical representation of a generic building for ground view
class GUIGroundBuilding
    : public GroundBuilding,
    public QGraphicsItem,
    public IGUIItemHandler
{
public:
    //! Constructor
    GUIGroundBuilding(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundBuilding();

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

    //! Interface IGUIItemHandler
    virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
    virtual void SetSI(qint16 SI) {}
private:
    int             mPenWidth;
    QColor          mColor;
    QPainterPath    mPainterPath;
};

#endif // GUIGROUNDBUILDING_H
