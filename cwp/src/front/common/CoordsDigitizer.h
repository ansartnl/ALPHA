#ifndef _COORDS_DIGITIZER_H__
#define _COORDS_DIGITIZER_H__

#include "Coordinate.h"
#include <QGraphicsItem>


//! Points digitizer.
class CoordsDigitizer
        : public QGraphicsItem
{
public:
    typedef QList<CCoord>   TCoordList;

    //! Constructor
    CoordsDigitizer(QGraphicsItem *parent = 0);

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget);
    //@}

    void updatePosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);

    void addCoord(const CCoord &coord) { mPoints << coord; }
    const TCoordList& coords() const { return mPoints; }

    bool isEmpty() const { return mPoints.empty(); }

    //! Copy points to clipboard.
    void toClipboard() const;

    //! Get points from clipboard
    static TCoordList GetPointsFromClipboard(QWidget *parent);

    inline void setMultiplePick(bool multiplePick)
    {mMultiplePicking = multiplePick;}
    inline bool multiplePick() const
    {return mMultiplePicking;}

private:
    QPainterPath    mPath;
    TCoordList      mPoints;
    bool            mMultiplePicking;
};

#endif // _COORDS_DIGITIZER_H__
