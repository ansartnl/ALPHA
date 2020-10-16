#ifndef GUIGROUNDTOWER_H
#define GUIGROUNDTOWER_H

#include <QGraphicsItem>
#include "back/ground/GroundTower.h"
#include "front/core/GUIInterface.h"

//! Class for the graphical representation of a tower for ground view
class GUIGroundTower
    : public GroundTower,
    public QGraphicsItem,
    public IGUIItemHandler
{
public:
    //! Constructor
    GUIGroundTower(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundTower();

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
    int                mPenWidth;
    QColor            mColor;
    QPainterPath    mPainterPath;
};

#endif // GUIGROUNDTOWER_H