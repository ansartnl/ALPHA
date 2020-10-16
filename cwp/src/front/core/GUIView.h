#ifndef CGUIVIEW_H
#define CGUIVIEW_H

#include "core/View.h"

#include <QHash>
#include <QGraphicsView>
#include <QTimer>
#include <QSet>
#include <QUuid>

#include "GUILineText.h"
#include "GUIBound.h"
#include "CoordsDigitizer.h"
#include "GUIRcGrid.h"
#include <back/core/airplane/AirplaneMTCD.h>
#include "back/core/airplane/BearingPoint.h"

class QSignalMapper;
class XReminder;
class CHighlightPlane;
class CTracerHDG;
class OLDIMessage;
template <typename T> class QFutureWatcher;

#ifdef CWP_AIRPLANE_THREAD
    class AirplaneThread;
#endif

//! Class graphical implementation of view
class CGUIView: public QGraphicsView, public CGUILineText::IParent, public CView
{
    Q_OBJECT

    enum mode {
        NONE_MODE = 0,
        TRACER_HDG,
        NEXT_POINT_SEARCH,
        MAP_NOTES
    };

    public:
        //! Interface to CGUIScene
        class IParent
        {
            public:
                virtual ~IParent() {}
                virtual void ZoomEvent(int iCurScale) = 0;
                virtual void CoordinateEvent(const CCoord& Coordinate, const QString& sRoute) = 0;
        };

        static const int MAX_SCALE = 50;
        //! Constructor
        /*! \param pParent a pointer to IParent */
        CGUIView(IParent* pParent);
        //! Destructor
        virtual ~CGUIView();

        //@{
        //! Inherited from CView
        virtual void AddScene(const QString& sKey, const TScenePtr& pscene);
        virtual void AddAirplane(const QSharedPointer<CAirplane>& pAirplane);
        virtual void AddMeteo(const QSharedPointer<core::CMeteo>& pMeteo);
        virtual void DelMeteo(uint uiZoneID);
        virtual void DelAirplane(QSharedPointer<CAirplane> pAirplane, bool bSend = true);
        virtual QSharedPointer<core::CMeteo> CreateMeteo() const;
        virtual QSharedPointer<CAirplane> CreateAirplane() const;
        //@}

        //! Zooming
        void SetZoom(int iValue);

        void SetFormularColor(const QColor& color);
        void SetGridColor(const QColor& color);

        //! Drawing all graphics objects
        void Draw();
        //! Redraw scene ( only paint)
        void Redraw();
        //! Redraw scene
        void RedrawAll();

        //! Before draw
        void prepare();

        //! Assign layouts
        void SetLayouts();

        //! Check airplanes on altitude interval
        void CheckPlanesAlt();


        //@{
        //! FIR's characteristics
        void SetGauge(bool bShow);
        void SetAutoCollapse(bool bCollapse);
        void SetEchelon(qint16 echelon);
        void SetPress(qint16 press);
        qint16 GetEchelon() const;
        qint16 GetPress() const;
        //@}

        //! Calculate geographic coordinate on graphic point
        CCoord GetCoordAtPoint(const QPointF &pos) const;
        //! Convert from global to scene coordinate.
        QPointF CoordToScene(const CCoord &coord);
        //! Convert from global to screen coordinate.
        QPoint CoordToScreen(const CCoord &coord);
        //! Convert distance from global to scene.
        qreal DistanceToScene(qreal distance);
        //! Retrieve current coordinate system.
        void GetCoordSystem(qint64 &qllXMin, qint64 &qllYMax, double &dScale, CCoord& centralCoord);

        //! Set list of reminder
        void SetReminder(XReminder* pReminder)
        {
            m_pReminder = pReminder;
        }

        //! Interface CGUILineText::IParent
        void DoubleClickLineText(CGUILineText *line_text, QPointF scene_pos);

        //@{
        //! Interface
        virtual void OnUpdateReminder(const QSharedPointer<CAirplane>& pPlane);
        virtual void SetSTCA(AirplaneCollision& ac);
        //@}

        //! Recieve OLDI message
        void RecieveOLDIMessage(QSharedPointer<OLDIMessage>& pMes);

        //! Start coordinates picking process.
        void startCoordsPicking(bool multipleCoords = true);
        //! Stop coordinates picking process.
        void stopCoordsPicking();
        //! Bearing information
        /*!
         \param AirplaneState bearing information
         */
        virtual void BearingData(CBearingData& BearingData);
        //! Set RcGrid center point
        void SetRcGridCenter(const RcGridValues& values);
        //! Load list DF(direction finders)
        void LoadDF();
        QList<CBearingPoint>& GetListDF();
        void setBearingId(const TBearingIdSet &id);

        void UpdateFormularType();

        void AddTempLine(const CCoord& coord1, const CCoord& coord2);

        void UpdateGaugeColor();

        bool IsDrawing() const {return m_isDrawing;}
    signals:
        void singleCoordPicked();
        void pickCoord(QUuid id);
        void lastBearing(const QString &lastBearing);

    public slots:
        //! Update view
        void OnDraw();
        //! Update airplanes
        void OnDrawAirplanes();
#ifdef CWP_AIRPLANE_THREAD
        void OnUpdateAirplane(quint16 id);
        void OnSetAirplane(quint16 id);
#endif
        //! Take custom point on click
        void OnSetPlaneNeedCoord(CAirplane* receiver);
        //! Clear selection
        void OnResetBearing();
        //! Highlight the airplane
        void OnSetActivePlane(CAirplane* pPlane);
        //! Change airplane's SFPL
        void OnShangedSFPL(const QSharedPointer<CSFPL>& pSFPL);
        //! Clear selection
        void OnResetLastActive();
        //! Highlight airplanes
        void OnHighlightPlanes(const CHighlightPlane& val);
        //! Plane tracer HDG
        void OnTracerHDG(CAirplane* receiver);
        //! ACID change is required.
        void OnChangeACID(quint16 idAirplane);
        //! Refresh meteo
        void OnMeteoRefresh();

        void onSetNextPointSearchMode(CAirplane *receiver);
        void onSetMapNotesMode(bool checked);

        void onHighlightCenter();

        void onGetPickCoord(bool bMulti, QUuid id);

        void onUserPicsChanged();
        void userPicsSelectChanged(int, int, bool);
        void userPicsSelectionDrop();
        void onUserPicsVisabilityChanged();

    protected:
        //@{
        //! Inherited from QGraphicsView
        virtual void wheelEvent(QWheelEvent * event);
        virtual void mouseMoveEvent(QMouseEvent *event);
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
        virtual void mouseDoubleClickEvent(QMouseEvent *event);
        virtual void keyPressEvent(QKeyEvent *event);
        virtual void keyReleaseEvent(QKeyEvent *event);
        //@}
        void timerEvent(QTimerEvent *event);

    private slots:
        void OnTimerRefreshListACC();
        //! Slot for connection with QInputDialog for changing value of ACID.
        /*! Need this function to limit number of symbols to 7.
            \sa OnChangeACID(..)
        */
        void on_acid_textValueSelected(const QString & text);

        void OnPullScene(QPointF CenterPoint);
        void DelBearing(int id);
        void OnMTCD();
        void OnMTCDfutwatchResult(int index);
        void OnMTCDfutwatchFinish();

        void onLockMoving();
        void onUnlockMoving();

        void onDrawCircle();

    private:
        void Zoom(const QPointF& PointOld, int iCurScale);
        void UpdateFormularsAfterUpdate();
        void setCursor(const QCursor& cursor);
        void unsetCursor();
        void RemoveSTCA(quint16 idPlane);
        void pickUserRestriction(const QPoint &pos);
        void RequestAllFPL();

        static mtcd::TListMTCDPtr CalcMTCD(const QPair<QSharedPointer<mtcd::MTCD>, QSharedPointer<QLinkedList<QSharedPointer<mtcd::MTCD> > > >& planeMTCD);
        void debugOnMTCDfutwatchResult(mtcd::TListMTCDPtr pList);
        void debugOnMTCDfutwatchFinish();

        void createVectorMeasurement(const QPoint &pos);

        void setCoordDigitizer(bool multipleCoords = true);
        void resetModeData();
        void setNextPoint(const CCoord &coords);

        qreal m_qWidth;
        qreal m_qHeight;
        int m_iScale;
        qint64 m_qXMin;
        qint64 m_qYMin;
        qint64 m_qXSize;
        qint64 m_qYSize;
        QTimer m_AirplaneTimer;
        IParent* m_pParent;
        bool m_bPull;
        QPointF m_PullPoint;
        CAirplane* m_pPlaneNeedCoord;
        CAirplane *mode_airplane_;
        mode mode_;
        bool m_bShowGauge;
        qint16 m_qEchelon;
        qint16 m_qPress;
        int     m_last_selected_pics;
        int     m_last_selected_pic_row;
        int     m_last_selected_show;
        bool    m_bRefreshListACC;

        XReminder*  m_pReminder;

        QHash<CAirplane*, QSharedPointer<CTracerHDG> > airplane_to_tracer_;

        CGUILineText*                                       m_pTempLine;
        QMap<CGUILineText*, QSharedPointer<CGUILineText> >  m_mapGauge;

        typedef QMap<QPair<int,int>, QSharedPointer<CGUILineText> > TMapSTCA;
        TMapSTCA    m_mapSTCA;

        typedef QSharedPointer<CoordsDigitizer>     TCoordsDigitizer;
        //! Coords picking
        TCoordsDigitizer        m_coordsDigitizer;
        //! Bearing
        QHash<quint32, CGUILineText*> m_pBearingLines;
        QHash<quint32, QTimer*>       m_pBearingTimers;
        QHash<quint32, qint64>        m_bearingTimestamps;
        QList<CBearingPoint>          m_listDF;
        QSignalMapper*                m_pBearingSignalMapper;
        TBearingIdSet                 m_bearingId;

        CGUIRcGrid*         m_rcGrid;

        QFutureWatcher<mtcd::TListMTCDPtr>* m_MTCDfutwatch;
        QMap<int, mtcd::TListMTCDPtr>       m_MTCDmapPlane;
        QLinkedList<QPair<QSharedPointer<mtcd::MTCD>, QSharedPointer<QLinkedList<QSharedPointer<mtcd::MTCD> > > > > m_listPair;
        static volatile bool                m_MTCDforceStop;

        bool m_isLocked;
        bool m_isDrawing;

#ifdef CWP_AIRPLANE_THREAD
        AirplaneThread      *m_airplaneThread;
#endif

        // Highlighting center
        static const int kStartRadius = 256;
        static const int kDivisor = 2;
        int m_radius;
        QGraphicsEllipseItem *m_circle;

        QUuid mPickCoordID;
};

extern CGUIView     *gActiveView;

#endif // CGUIVIEW_H
