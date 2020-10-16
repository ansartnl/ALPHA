#ifndef GUIGROUNDWAYPOINTMARKER_H
#define GUIGROUNDWAYPOINTMARKER_H

#include <QGraphicsItem>
#include "back/ground/GroundWaypointMarker.h"
#include "front/core/GUIInterface.h"

class GUIGroundWaypointMarker
        : public QObject,
        public GroundWaypointMarker,
        public QGraphicsItem,
        public IGUIItemHandler
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit GUIGroundWaypointMarker(QObject *parent = 0);
    //! Destructor
    virtual ~GUIGroundWaypointMarker();

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
    void isMoved(int SN, QPointF where);
    void isDeleted(int SN);

protected:
    //@{
    //! Inherited from QGraphicsItem
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //@}
private:
    int             mPenWidth;
    QColor          mColor;
    QPainterPath    mPainterPath;
    QPainterPath    mHoverPath;
    QPointF         mDragStartPos;
};

#endif // GUIGROUNDWAYPOINTMARKER_H
