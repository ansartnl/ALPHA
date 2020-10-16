#ifndef GUIGROUNDVEHICLE_H
#define GUIGROUNDVEHICLE_H

#include <QGraphicsItem>
#include <QStack>
#include "front/core/GUIInterface.h"
#include "back/ground/GroundVehicle.h"

#include "front/core/GUILineText.h"

#include "GUIGroundVehiclePoint.h"
#include "GUIGroundVehicleLabel.h"

#include "back/ground/IGroundItemGUI.h"

struct RoutePoints{
    int SN;
    int increment;
    QVector<CCoord> points;
};

//! Main class for graphical representation of ground vehicle
class GUIGroundVehicle
        : public GroundVehicle,
        public QGraphicsItem,
        public IGUIItemHandler,
        public IGroundItemGUI
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:

    typedef QList<QSharedPointer<GroundPoint> > TRoutePointList;

    GUIGroundVehicle(QGraphicsItem * parent = 0);
    virtual ~GUIGroundVehicle();

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget);
    //@}
    void Translate();
    //@{
    //! Interface IGUIItemHandler
    virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
    virtual void SetSI(qint16 SI);
    //@}

    //! Update data
    void UpdatePosition();

    //! Show airplane's route
    void ShowPathPoints(bool bShow = true);
    //! Is show route
    bool isShowPathPoints() const;

    //! Gets position on a scene
    QPointF getVehiclePosition();

    void GetVehicleNewCoord();

    void SetVehicleNewCoord(const CCoord &coord);

    void setPathPoints(const TRoutePointList& coords)
    {
        mPathCoords.clear();
        mPathCoords.append(coords);
    }

    inline const TRoutePointList& pathPoints() const
    {return mPathCoords;}

    void clearPathPoints()
    {mPathCoords.clear();}

    const CCoord& coordinate() const
    {return GetCoord();}

    bool isAtEnd();

    void setRoutePoints(const QVector<CCoord>& coords)
    {
        mRoutePoints.points.clear();
        mRoutePoints.points += coords;
    }

    void setRouteParameters(int sn, int increment)
    {
        mRoutePoints.SN = sn;
        mRoutePoints.increment = increment;
    }

    inline const QVector<CCoord>& planPoints() const
    {return mRoutePoints.points;}

    //! set formular default position
    void SetFormularDefaultPos();
    //! Set auto collapse
    void SetAutoCollapse(bool bCollapse);

private:
    void RecalculateAll();

signals:
    //! Get the coordinates from the scene
    void getCoord(GroundVehicle* receiver);
    void calcCoordAtPoint(const QPointF &point, CCoord &coord);

private:
    int            m_iPenWidth;
    QColor        m_color;
    QPointF        m_CenterPoint;
    qint64        m_qllXMin;
    qint64        m_qllYMax;
    double        m_dScale;
    bool            m_bMoving;
    CCoord        m_CentralCoord;

    QPainterPath        m_painterPath;
    QPainterPath        m_painterPathPoints;

    QStack<CCoord>        m_listCoords;
    QList<QSharedPointer<CGUILineText> > m_listRoute;
    GUIGroundVehiclePoint    m_Vehicle;
    GUIGroundVehicleLabel    m_Label;

    TRoutePointList mPathCoords;

    QVector<CCoord>     mPlanPoints;
    RoutePoints         mRoutePoints;

};

#endif // GUIGROUNDVEHICLE_H
