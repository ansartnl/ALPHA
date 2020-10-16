#ifndef GUIGROUNDTRACE_H
#define GUIGROUNDTRACE_H

#include <QGraphicsItem>
#include "back/ground/GroundTrace.h"
#include "front/core/GUIInterface.h"

//! Class for the graphical representation of a taxi path for ground view
class GUIGroundTrace
    : public GroundTrace,
    public QGraphicsItem,
    public IGUIItemHandler
{
public:
    //! Constructor
    GUIGroundTrace(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundTrace();

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

#endif // GUIGROUNDTRACE_H
