#ifndef GUIGROUNDSIGN_H
#define GUIGROUNDSIGN_H

#include <QGraphicsItem>
#include "back/ground/GroundSign.h"
#include "front/core/GUIInterface.h"

//! Class for the graphical representation of a taxi sign for ground view
class GUIGroundSign
    : public GroundSign,
    public QGraphicsItem,
    public IGUIItemHandler
{
public:
    //! Constructor
    GUIGroundSign(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~GUIGroundSign();

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

protected:

    QString parseLabel() const;

private:
    int                mPenWidth;
    QColor            mColor;
    QPainterPath    mPainterPath;
    QPainterPath    mHoverPath;

    QString         mParsedLabel;
};

#endif // GUIGROUNDSIGN_H
