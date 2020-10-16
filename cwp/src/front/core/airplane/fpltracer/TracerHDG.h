#ifndef TRACER_HDG_H
#define TRACER_HDG_H

#include <QGraphicsObject>
#include "Coordinate.h"
#include "back/core/Airplane.h"

class CGUILineText;

//! Class for the graphical representation of HDG
class CTracerHDG : public QGraphicsObject
{
    Q_OBJECT
    CTracerHDG() {}

public:
    //! Constructor
    /*!
      \param pPlane a pointer to CAirplane
      \param parent a parent QGraphicsItem
    */
    explicit CTracerHDG(const QSharedPointer<CAirplane>& pPlane, QGraphicsItem* parent = 0);
    //! Destructor
    virtual ~CTracerHDG();

    void Update();
    void Update(CCoord coord);
    void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget);
    //@}
private:
    void UpdateLines();

private:
    CGUILineText*   m_pLine1;
    CGUILineText*   m_pLine2;
    QWeakPointer<CAirplane>   m_pAirplane;
};

#endif // TRACER_HDG_H
