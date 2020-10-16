#ifndef GUIWAYPOINTMANAGER_H
#define GUIWAYPOINTMANAGER_H

#include <QGraphicsItem>
#include "back/ground/WaypointManager.h"
#include "front/core/GUIInterface.h"

class GUIWaypointManager
        :
        public QObject,
        public WaypointManager,
        public QGraphicsItem,
        public IGUIItemHandler
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit GUIWaypointManager(QGraphicsItem *parent = 0);
    explicit GUIWaypointManager(GUIGroundScene* scene = 0);
    
    //! Destructor
    virtual ~GUIWaypointManager();

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

    void recalculate();
    //! Interface IGUIItemHandler
    virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
    virtual void SetSI(qint16 SI) {}

public slots:
    void onWaypointMoved(int SN, QPointF where);
    void onWaypointDeleted(int SN);

private:
    QPainterPath    mPath;
    QVector<CCoord> mPathCoords;
};

#endif // GUIWAYPOINTMANAGER_H
