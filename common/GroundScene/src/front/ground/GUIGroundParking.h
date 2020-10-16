#ifndef GUIGROUNDPARKING_H
#define GUIGROUNDPARKING_H

#include <QGraphicsItem>
#include "back/ground/GroundParking.h"
#include "front/core/GUIInterface.h"

//! Class for the graphical representation of a parking for ground view
class GUIGroundParking
    : public QObject,
    public GroundParking,
    public QGraphicsItem,
    public IGUIItemHandler
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    //! Constructor
    GUIGroundParking(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundParking();

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
    void hoverEnter(QString name);
    void hoverLeave(QString name);

protected:
    //@{
    //! Inherited from QGraphicsItem
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    //@}

private:
    int             mPenWidth;
    QColor          mColor;
    QPainterPath    mPainterPath;
    QPainterPath    mHoverPath;
    int             mFontSize;
};

#endif // GUIGROUNDPARKING_H
