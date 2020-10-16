#ifndef GUIGROUNDSCENE_H
#define GUIGROUNDSCENE_H

#include <QGraphicsView>
#include "src/front/core/GUILineText.h"
#include "back/ground/GroundScene.h"

#include "back/ground/GroundCoordsDigitizer.h"
#include "GUIGroundApron.h"
#include "GUIGroundBuilding.h"
#include "GUIGroundParking.h"
#include "GUIGroundPath.h"
#include "GUIGroundTrace.h"
#include "GUIGroundPoint.h"
#include "GUIGroundRunway.h"
#include "GUIGroundStart.h"
#include "GUIGroundTower.h"
#include "GUIGroundRadioRestriction.h"
#include "GUIGroundVehicle.h"
#include "GUIGroundWaypointMarker.h"
#include "front/core/GUIAirplane.h"
#include "GUIGroundVehicle.h"
#include "GUIGroundSign.h"
#include "GUIGroundCrossrads.h"
#include "back/ground/IGroundItemGUI.h"
#include "front/ground/GUIWaypointManager.h"

class QShortcut;

class GUIGroundScene
        : public QGraphicsView,
        public CGUILineText::IParent,
        public GroundScene
#ifndef GROUND_CWP
        , public ILanguage
#endif
{
    Q_OBJECT
public:

    typedef QHash<QString, QString> TShortcutMap;
    //static QString XML_PATH;

    //! Interface to CGUIScene
    class IParent
    {
    public:
        virtual ~IParent() {}
        virtual void ZoomEvent (int iCurScale) = 0;
        virtual void CoordinateEvent(const CCoord& Coordinate, const QString& sRoute) = 0;
    };

    static const int MAX_SCALE = 40;
    //! Constructor
    /*! \param pParent a pointer to IParent */
    GUIGroundScene(IParent* pParent);
    //! Destructor
    virtual ~GUIGroundScene();

    //! ILanguage
    void Translate();

    virtual void AddAirplane(const QSharedPointer<CAirplane> &pAirplane);
    virtual void AddPoint(const QSharedPointer<GroundPoint>& pPoint);
    virtual void AddTower(const QSharedPointer<GroundTower>& pTower);
    virtual void AddStart(const QSharedPointer<GroundStart>& pStart);
    virtual void AddRunway(const QSharedPointer<GroundRunway>& pRunway);
    virtual void AddApron(const QSharedPointer<GroundApron>& pApron);
    virtual void AddPath(const QSharedPointer<GroundPath>& pPath);
    virtual void AddTrace(const QSharedPointer<GroundTrace>& pTrace);
    virtual void AddParking(const QSharedPointer<GroundParking>& pParking);
    virtual void AddBuilding(const QSharedPointer<GroundBuilding>& pBuilding);
    virtual void AddSign(const QSharedPointer<GroundSign>& pSign);
    virtual void AddCrossroads(const QSharedPointer<GroundCrossroads>& pCrossroads);
    virtual void AddVehicle(const QSharedPointer<GroundVehicle>& pVehicle);
    virtual void AddRadioRestriction(const QSharedPointer<GroundRadioRestriction>& pRestriction);
    virtual void AddWaypointMarker(const QSharedPointer<GroundWaypointMarker>& pMarker);

    virtual void DelAirplane(QSharedPointer<CAirplane> pAirplane, bool bSend);
    virtual void DelVehicle(QSharedPointer<GroundVehicle> pVehicle, bool bSend);
    virtual void DelWaypointMarker(QSharedPointer<GroundWaypointMarker> pMarker);

    virtual QSharedPointer<GroundPoint> CreatePoint() const;
    virtual QSharedPointer<GroundTower> CreateTower() const;
    virtual QSharedPointer<GroundStart> CreateStart() const;
    virtual QSharedPointer<GroundRunway> CreateRunway() const;
    virtual QSharedPointer<GroundApron> CreateApron() const;
    virtual QSharedPointer<GroundPath> CreatePath() const;
    virtual QSharedPointer<GroundTrace> CreateTrace() const;
    virtual QSharedPointer<GroundParking> CreateParking() const;
    virtual QSharedPointer<GroundBuilding> CreateBuilding() const;
    virtual QSharedPointer<GroundSign> CreateSign() const;
    virtual QSharedPointer<GroundCrossroads> CreateCrossroads() const;
    virtual QSharedPointer<CAirplane> CreateAirplane() const;
    virtual QSharedPointer<GroundVehicle> CreateVehicle() const;
    virtual QSharedPointer<GroundRadioRestriction> CreateRadioRestriction() const;
    virtual QSharedPointer<GroundWaypointMarker> CreateWaypointMarker() const;

    void SetShortcuts(const TShortcutMap& shortcut);

    //! Zooming
    void SetZoom(int iValue);

    //! Drawing all graphics objects
    void Draw();

    //! Before draw
    void Prepare();

    //! Assign layouts
    void SetLayouts();    

    //@{
    //! FIR's characteristics
#ifndef GROUND_CWP
    SI GetSI() const { return m_SI; }
    void SetSI(qint16 SI);
#endif
    void SetGauge(bool bShow);
    void SetAutoCollapse(bool bCollapse);
    void SetEchelon(qint16 echelon);
    void SetPress(qint16 press);
    void SetDeflexion(int type, int value);
    void DisplayRadioRestrictions(bool show);
    //@}

    //! Calculate geographic coordinate on graphic point
    CCoord GetCoordAtPoint(QPointF pos) const;
    //! Set list of reminder

    //! Interface CGUILineText::IParent
    void DoubleClickLineText(CGUILineText* pLineText);
    void DoubleClickLineText(CGUILineText* pLineText, QPointF scenePos);


    //! Start coordinates picking process.
    void StartCoordsPicking();
    //! Stop coordinates picking process.
    void StopCoordsPicking();

    inline void setDeletingWaypoint(bool isDeleting)
    {m_deletingWaypoint = isDeleting;}

signals:
    void sendWaypoints(QList<CCoord> waypoints);

public slots:
    void displayRouteWaypoints(bool enabled, QVector<CCoord> coords);
    void onWaypointsRequested();

    void pointHoverEnter(int idx);
    void pointHoverLeave(int idx);
    void standHoverEnter(QString name);
    void standHoverLeave(QString name);

    //! Update airplanes
    void OnDrawAirplanes();
    //! Take custom point on click
    void OnSetPlaneNeedCoord(CAirplane* receiver);
    void OnSetVehicleNeedCoord(GroundVehicle* receiver);

#ifndef GROUND_CWP
    //! Clear selection
    void OnResetBearing();
#endif
    //! Highlight the airplane
    void OnSetActivePlane(CAirplane* pPlane);

    void OnSetFormularDefaultPos();

protected slots:
    void OnCalcCoordAtPoint(const QPointF &point, CCoord &coord);
    void OnPullScene(QPointF CenterPoint);

#ifdef GROUND_CWP
    void OnTimerRefreshListACC();
#endif

protected:
    //@{
    //! Inherited from QGraphicsView
    virtual void wheelEvent (QWheelEvent * event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    //@}

private:
    void Zoom(const QPointF& PointOld, int iCurScale);
    void setCursor(const QCursor& cursor);
    void unsetCursor();

    qreal        m_qWidth;
    qreal        m_qHeight;
    int            m_iScale;
    qint64        m_qXMin;
    qint64        m_qYMin;
    qint64        m_qXSize;
    qint64        m_qYSize;
    double        m_dScale;
    IParent*    m_pParent;
    bool        m_bPull;
    bool        m_Moved;
    QPointF        m_PullPoint;
    CGUIAirplane*    m_pPlaneNeedCoord;
    GUIGroundVehicle*    m_pVehicleNeedCoord;

#ifndef GROUND_CWP
    SI          m_SI;
#endif
    bool        m_bShowGauge;
    bool        m_bAutoCollapse;
    qint16        m_qEchelon;
    qint16        m_qPress;
    QPair<int,int>  m_Deflexion;
    QTimer        m_AirplaneTimer;

    CGUILineText*                                       m_pTempLine;
    QMap<CGUILineText*, QSharedPointer<CGUILineText> >  m_mapGauge;    
    QList<QSharedPointer<CGUILineText> > m_listRoute;

    typedef QSharedPointer<CoordsDigitizer>             TCoordsDigitizer;
    //! Coords picking
    TCoordsDigitizer    m_coordsDigitizer;
    GUIWaypointManager  m_WaypointManager;

    bool    m_bRefreshListACC;
    bool    m_VehicleRouteEditing;
    bool    m_deletingWaypoint;

    QShortcut* mRestoreLabelsShortcut;
};

#endif // GUIGROUNDSCENE_H
