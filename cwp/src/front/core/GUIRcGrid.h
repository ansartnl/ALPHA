#ifndef GUIRCGRID_H
#define GUIRCGRID_H

#include <QGraphicsItem>
#include "GUIInterface.h"
#include "RcGridValues.h"

//! Class for the graphical representation of radial circular grid
class CGUIRcGrid
        : public QGraphicsItem,
        public IGUIItemHandler
{
public:
    //! Constructor
    CGUIRcGrid();

    //! Set grid values
    void setRcGridValues(const RcGridValues& values);

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

    //@{
    //! Interface IGUIItemHandler
    virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
    virtual void SetSI(qint16 SI) {}
    //@}

    void SetColor(const QColor& color);
    QList<qreal> stripeRadiusList() const;
    QList<qreal> stripeSizeList() const;
    QList<QColor> stripeColorList() const;
    QList<qreal> sectorAngleList() const;
    QList<qreal> sectorSizeList() const;
    QList<QColor> sectorColorList() const;

    bool appendSector(qreal angle, qreal size, const QColor & color);
    bool appendStripe(qreal radius, qreal size, const QColor & color);

    static void SaveRcGridValues(const QString& sFir, const RcGridValues& values);
    static RcGridValues LoadRcGridValues(const QString& sFir);

private:
    static void SaveCenterCoord(const QString& sFir, const CCoord& coord);
    static CCoord LoadCenterCoord(const QString& sFir);

private:
    QList<qreal> m_stripeRadiusList;
    QList<qreal> m_stripeSizeList;
    QList<QColor> m_stripeColorList;
    QList<qreal> m_sectorAngleList;
    QList<qreal> m_sectorSizeList;
    QList<QColor> m_sectorColorList;

    QRectF m_boundingRect;
    qreal m_scale;

    RcGridValues m_values;

    QColor m_Color;
};

#endif // GUIADGRID_H
