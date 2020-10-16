#ifndef GUIAIRPLANE_H
#define GUIAIRPLANE_H

#include <QScopedPointer>

#include <QGraphicsItem>

#include "GUIObject.h"
#include "core/Airplane.h"
#include "GUIInterface.h"
#include "airplane/GUIAirplanePoint.h"
#include "airplane/GUIAirplaneLabel.h"
#include "airplane/GUIAirplaneLabelExt.h"
#include "GUILineText.h"
#include "ACTDlg.h"
#include "front/core/airplane/fpltracer/TracerFPL.h"

class CTracerFPL;

//! Class for highlighting airplane
class CHighlightPlane
{
    CHighlightPlane();
public:
    enum Type { None = 0, CFL, TFL, AFL };
    CHighlightPlane(const Type type, const QVariant &value1, const QVariant &value2 = QVariant())
        : m_type(type), m_value1(value1), m_value2(value2)
    {}
    const Type GetType() const
    { return m_type; }
    const QVariant GetValue1() const
    { return m_value1; }
    const QVariant GetValue2() const
    { return m_value2; }
private:
    Type     m_type;
    QVariant m_value1;
    QVariant m_value2;
};

//! Main class for the graphical representation of aircraft
class CGUIAirplane: public CAirplane, public CGUIObject, public IGUIItemHandler
{
    Q_OBJECT

        Q_INTERFACES(QGraphicsItem)

        static const QPair<int, int> RVSMLayer;
    public:
        //! Constructor
        /*! \param parent a parent QGraphicsItem */
        CGUIAirplane(QGraphicsItem * parent = 0);
        //! Destructor
        virtual ~CGUIAirplane();

        //@{
        //! Inherited from QGraphicsItem
        virtual QRectF boundingRect() const;
        virtual QPainterPath shape() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        //@}

        //@{
        //! Interface IGUIItemHandler
        virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
        //@}

        //! Update data
        void UpdatePosition();

        //! Highlight this airplane
        void SetActivePlane(bool bSetActive);
        //! Set highlight
        void ActivatePlane(bool bActive);
        //@{
        //! Set highlighting plane
        void SetHighlight(const CHighlightPlane& val);
        void SendHighlight(const CHighlightPlane& val);
        //@]

        //@{
        //! Coordinate of airplane
        void GetPlaneNewCoord();
        void SetPlaneNewCoord(const CCoord& coord);
        CCoord GetCurGUIPlaneCoord();
        //@}

        //! Set new formular color.
        void SetFormularColor(const QColor& color);
        //! Set new color of airplane.
        virtual void SetColor(const QColor& color);

        //! Check airplane on altitude interval
        void CheckPlaneAlt();

        //! Reload formular type
        void UpdateFormularType();

        void UpdateFormularPos();

        virtual bool fixedFormular() const;
        virtual void setFixedFormular(bool fixed);
        virtual void setFormularPos(qreal angle, int length);
        virtual void formularBoundingRect(QPointF &point, QRectF &rect) const;
        void ShowFormularExt();

        virtual qreal angle() const { return m_Plane.angle(); }

    public slots:
        //! Show airplane's route
        void ShowPathPoints(bool bShow = true);

    public:
        //! Is show route
        bool isShowPathPoints() const;
        bool setSignShowRoute(bool sign);
        bool getSignShowRoute() const;
        void updateExtAfterSceneUpdated();
        int GetMachLevel() const
        {
            return 7500;
        }

        void SetActivationFlag(bool flag){}
        void SetDeflexion(const QPair<int,int> &def){}

        //@{
        //! Properties of airplane
        void SetBearing(bool bVal);
        void SetAutoCollapse(bool bCollapse);
        quint16 GetBearingAngle(CCoord BearingCoord);
        int GetAzimuth2() const;
        void SetAzimuth2(int degree);
        void ClearAzimuth2();
        void setNextPoint(const QSharedPointer<CPoint> &next_point);
        const CHighlightPlane GetHighlight() const
        { return m_highlightPlane; }
        //@}
        //! Change SFPL
        virtual void ChangeSFPL();
        //{@
        //! Last active plane
        void SetLastActive(const bool bActive);
        bool IsLastActive() const
        {
            return m_bLastActive;
        }
        //@}
        void SetTracer(QSharedPointer<CTracerFPL> pTracer)
        { m_pTracer = pTracer; }
        //! Tracer HDG
        void SetTracerHDG();
        void setNextPointSearchMode();
        void ClearLabelExtflag(){ LabelExtFlag = false; }
        qint64 Get_qllXMin();
        qint64 Get_qllYMax();
        double Get_dScale();
        CCoord Get_CentralCoord();
        QSharedPointer<CTracerFPL> Get_pTracer();

    signals:
        //! Get the coordinates from the scene
        void getCoord(CAirplane* receiver);
        //! Clear selection
        void resetBearing();
        //! Highlight this airplane
        void setActivePlane(CAirplane* pPlane);
        //! Change SFPL
        void changedSFPL(const QSharedPointer<CSFPL>&);
        //! Clear selection
        void resetLastActive();
        //! Some highlighting
        void setHighlight(const CHighlightPlane& val);
        //! Tracer HDG
        void setTracerHDG(CAirplane *receiver);
        void setNextPointSearchMode(CAirplane *airplane);

        void lockMovingView();
        void unlockMovingView();

    protected:
        virtual void afterDirectAssume();

    private slots:
        void OnChangeSFPL();

        void RecalculateAll(bool from_Zoom_change = false);

        void OnAfterDirectAssume();

    private:
        int m_iPenWidth;
        QColor m_color;
        QPointF m_CenterPoint;
        qint64 m_qllXMin;
        qint64 m_qllYMax;
        double m_dScale;
        CCoord m_CentralCoord;
        bool m_bLastActive;

        QPainterPath m_painterPath;
        QPainterPath m_painterPathEquipment;

        QVector<QPointF> m_arrPoints;

        CGUIAirplanePoint m_Plane;
        CGUIAirplaneLabel m_Label;
        CGUIAirplaneLabelExt m_LabelExt;

        QScopedPointer<CACTDlg>     m_pActDlg;

        CHighlightPlane             m_highlightPlane;

        QSharedPointer<CTracerFPL>  m_pTracer;

        static int m_zValue;

        bool  m_fixedFormular;
        qreal m_formularLinkAngle;
        int   m_formularLinkLength;
        bool LabelExtFlag;
        bool was_set_ext;
        bool m_from_zoom_change;
        bool m_bSignShowRoute;
};

#endif // GUIAIRPLANE_H
