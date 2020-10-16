#ifndef GUIGROUNDCROSSRADS_H
#define GUIGROUNDCROSSRADS_H

#include <QGraphicsItem>
#include "back/ground/GroundCrossroads.h"
#include "front/core/GUIInterface.h"

class GUIGroundCrossroads
        : public GroundCrossroads,
        public QGraphicsItem,
        public IGUIItemHandler
{
public:
    //! Constructor
    GUIGroundCrossroads(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundCrossroads();

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
    QPainterPath    mRoundingPath;
};

#endif // GUIGROUNDCROSSRADS_H
