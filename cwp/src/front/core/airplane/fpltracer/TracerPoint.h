#ifndef TRACERPOINT_H
#define TRACERPOINT_H

#include <QGraphicsWidget>
#include "Coordinate.h"
#include "TracerHandler.h"
#include "back/core/RouteFragmentation.h"
#include <QMessageBox>

//! Class for the graphical representation of FPL's point
class CTracerPoint : public QGraphicsWidget
{
    Q_OBJECT
    CTracerPoint() {}

public:
    enum PointType { Custom = 0, New, Normal, FPL, FPL_SSA };

    //! Constructor
    /*!
      \param pTracer a pointer to CTracerFPL
      \param coord a coordinate of point
      \param type a type of point
      \param parent a parent QGraphicsItem
    */
    CTracerPoint(ITracerHandler* pTracer, const QString& sName, const CCoord& coord, PointType type, QGraphicsItem* parent = 0);
    //! Destructor
    virtual ~CTracerPoint();

    //! Get type of point
    PointType GetType() const { return m_Type; }
    //! Set type of point
    void SetType(PointType type);
    //! Get read only
    bool GetReadOnly() const { return m_bReadOnly; }
    //! Set read only
    void SetReadOnly(bool bReadOnly);
    //! Get point data
    /*!
      \param sName a name of point
      \param coord a coordianate of point
    */
    void GetPointParam(QString& sName, CCoord& coord) const;
    //! Get point coord
    CCoord GetPointCoord() const
    { return m_Coord; }
    //! Set point position
    void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);

    //! Set point color
    void SetColor(const QColor &color) { m_color = color; }

    //@{
    //! FPL frag
    void SetFPLProc(CFPLFrag::TProcFragPtr pProc)
    { m_pFragProc = pProc; }
    CFPLFrag::TProcFragPtr GetFPLProc()
    { return m_pFragProc; }
    void SetFPLPoint(CProcFrag::TPointFragPtr pPoint)
    { m_pFragPoint = pPoint; }
    CProcFrag::TPointFragPtr GetFPLPoint()
    { return m_pFragPoint; }
    //@}

public:
    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget);
    void setVisible(bool visible);
    //@}

protected:
    //@{
    //! Inherited from QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    //@}

private:
    ITracerHandler* m_pTracer;
    int             m_iPenWidth;
    QColor          m_color;
    qint64          m_qllXMin;
    qint64          m_qllYMax;
    double          m_dScale;
    CCoord          m_CentralCoord;
    QPainterPath    m_painterPath;

    QString         m_sPointName;
    CCoord          m_Coord;
    PointType       m_Type;

    bool            m_bMoving;
    bool            m_bMousePress;
    bool            m_bReadOnly;

    CFPLFrag::TProcFragPtr      m_pFragProc;
    CProcFrag::TPointFragPtr    m_pFragPoint;

    QScopedPointer<QMessageBox> m_pMessBox;
};

Q_DECLARE_METATYPE(CTracerPoint*)

#endif // TRACERPOINT_H
