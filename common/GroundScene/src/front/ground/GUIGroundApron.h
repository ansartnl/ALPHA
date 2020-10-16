#ifndef GUIGROUNDAPRON_H
#define GUIGROUNDAPRON_H

#include <QGraphicsItem>
#include "back/ground/GroundApron.h"
#include "front/core/GUIInterface.h"

//! Class for the graphical representation of apron for ground view
class GUIGroundApron
    : public GroundApron,
    public QGraphicsItem,
    public IGUIItemHandler
{
public:
    //! Constructor
    GUIGroundApron(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundApron();

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

#endif // GUIGROUNDAPRON_H
