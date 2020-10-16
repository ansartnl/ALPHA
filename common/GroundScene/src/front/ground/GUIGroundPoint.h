#ifndef GUIGROUNDPOINT_H
#define GUIGROUNDPOINT_H

#include <QGraphicsItem>
#include "back/ground/GroundPoint.h"
#include "front/core/GUIInterface.h"

//! Class for the graphical representation of a taxi waypoint for ground view
class GUIGroundPoint
    : public QObject,
    public GroundPoint,
    public QGraphicsItem,
    public IGUIItemHandler
{        
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    //! Constructor
    GUIGroundPoint(QGraphicsItem * parent = 0);    
    //! Destructor
    virtual ~GUIGroundPoint();

    virtual void setType(GroundPoint::Type val);
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

signals:
    void hoverEnter(int idx);
    void hoverLeave(int idx);

protected:
    //@{
    //! Inherited from QGraphicsItem
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    //@}

    void setupColors();
private:
    int             mPenWidth;
    QColor          mBorderColor;
    QColor          mFillColor;
    QPainterPath    mPainterPath;
    QPainterPath    mHoverPath;
    bool            mIsHovered;
};

#endif // GUIGROUNDPOINT_H
