#ifndef GUIGROUNDSTART_H
#define GUIGROUNDSTART_H

#include <QGraphicsItem>
#include "back/ground/GroundStart.h"
#include "front/core/GUIInterface.h"

//! Class for the graphical representation of a starting point for ground view
class GUIGroundStart
    : public GroundStart,
    public QGraphicsItem,
    public IGUIItemHandler
{
public:
    //! Constructor
    GUIGroundStart(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundStart();

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

#endif // GUIGROUNDSTART_H
