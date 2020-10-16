#ifndef GUIGROUNDPATH_H
#define GUIGROUNDPATH_H

#include <QGraphicsItem>
#include "back/ground/GroundPath.h"
#include "front/core/GUIInterface.h"

//! Class for the graphical representation of a taxi path for ground view
class GUIGroundPath
    : public GroundPath,
    public QGraphicsItem,
    public IGUIItemHandler
{
public:
    //! Constructor
    GUIGroundPath(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundPath();

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
    QColor          mPathColor;
    QPainterPath    mPainterSurfacePath;
};

#endif // GUIGROUNDPATH_H
