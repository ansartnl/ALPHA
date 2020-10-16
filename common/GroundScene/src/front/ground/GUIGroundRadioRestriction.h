#ifndef GUIGROUNDRADIORESTRICTION_H
#define GUIGROUNDRADIORESTRICTION_H
    
#include <QGraphicsItem>
#include "back/ground/GroundRadioRestriction.h"
#include "front/core/GUIInterface.h"

//! Class for the graphical representation of radio restriction zones for ground view
class GUIGroundRadioRestriction
    : public GroundRadioRestriction,
    public QGraphicsItem,
    public IGUIItemHandler
{
public:
    //! Constructor
    GUIGroundRadioRestriction(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundRadioRestriction();

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

#endif // GUIGROUNDRADIORESTRICTION_H
