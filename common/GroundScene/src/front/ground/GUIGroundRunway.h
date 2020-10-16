#ifndef GUIGROUNDRUNWAY_H
#define GUIGROUNDRUNWAY_H

#include <QGraphicsItem>
#include "back/ground/GroundRunway.h"
#include "front/core/GUIInterface.h"

#include <QFont>
#include <QStaticText>

//! Class for the graphical representation of a runway for ground view
class GUIGroundRunway
    : public GroundRunway,
    public QGraphicsItem,
    public IGUIItemHandler
{
public:
    //! Constructor
    GUIGroundRunway(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundRunway();

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

    QPainterPath    mMarkingPath;
};

#endif // GUIGROUNDRUNWAY_H
