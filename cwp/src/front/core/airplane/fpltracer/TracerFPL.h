#ifndef TRACERFPL_H
#define TRACERFPL_H

#include <QGraphicsItem>
#include "front/core/GUIInterface.h"
#include "TracerPoint.h"
#include "front/core/GUILineText.h"
#include "TracerHandler.h"

class CView;
class CGUIAirplane;
class CGUIAirplaneMTCDPoint;

//! Class for graphic editing FPL
class CTracerFPL
    : public QGraphicsItem,
    public IGUIItemHandler,
    public CGUILineText::IParent,
    public ITracerHandler
{
    CTracerFPL();
public:
    //! Constructor
    /*!
      \param pPlane a pointer to CGUIAirplane
    */
    CTracerFPL(const QSharedPointer<CFPLFrag>& FPLFrag, const QSharedPointer<IScene>& pScene,
               const QColor &color = QColor(Qt::yellow));
    //! Destructor
    virtual ~CTracerFPL();

    //! Update tracer
    void UpdateTracerPlane(CGUIAirplane* pPlane);

    //! Set read only mode
    void SetReadOnly(bool bReadOnly = true);

    //! Set color
    void SetColor(const QColor &color);

    //! Set CGUIAirplane
    void SetCGUIAirplane(CGUIAirplane* pPlane);

public:
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
    //! Interface CGUILineText::IParent
    virtual void DoubleClickLineText(CGUILineText* pLineText, QPointF scenePos);
    //@{
    //! Interface ITracerHandler
    virtual void DeleteTracerPoint(CTracerPoint* pPoint);
    virtual bool IsCanAddPoint(const QString& sName, const CCoord& coord, QSharedPointer<CPoint>& pPoint);
    virtual bool IsCanDeletePoint(CTracerPoint* pPoint);
    virtual void UpdateTracer(CTracerPoint* pPoint = 0);
    virtual void RecalculateTracer(quint32 seconds = 0);
    virtual CCoord GetCoordAtPoint(const QPointF &pos) const;
    //@}

private:
    void CreateGUIFPL();
    void DeleleTracer();
    QStringList GetListMTCDText(int id1, int id2);

    bool getPlanePoint(const QSharedPointer<CPointFrag> &cop_point, const QString &cop_point_name, QString &point_name, QPointF &plane_point);

private:
    typedef QList<CTracerPoint*>    TTracerPoints;
    typedef QList<CGUILineText*>    TTracerLines;
    typedef QMap<CCoord, CGUIAirplaneMTCDPoint*> TMapMTCDPoints;

    TTracerPoints               m_listTracerPoints;
    TTracerLines                m_listTracerLines;
    TMapMTCDPoints              m_mapMTCDPoints;
    QSharedPointer<CFPLFrag>    m_FPLFrag;
    QWeakPointer<CView>         m_pView;
    CGUIAirplane*               m_pAirplane;
    CGUILineText*               m_pPlaneLine;

    QColor                      m_color;
    qint64                      m_qllXMin;
    qint64                      m_qllYMax;
    double                      m_dScale;
    CCoord                      m_CentralCoord;

    bool                        m_bPosChange;
    bool                        m_bReadOnly;
};

#endif // TRACERFPL_H
