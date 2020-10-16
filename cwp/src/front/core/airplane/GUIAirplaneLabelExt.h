#ifndef GUIAIRPLANELABELEXT_H
#define GUIAIRPLANELABELEXT_H

#include <QGraphicsWidget>

#include "front/common/XGridLabel.h"
#include "EchelonValues.h"
#include "back/common/FLVal.h"
#include "utils/Coordinate.h"
#include "front/common/XAppSettings.h"

#include "AirplaneData.h"

class QGraphicsView;
class QGraphicsGridLayout;
class CGUIAirplane;
class CGUILabelText;
class CGUIReminder;
class CGUIAirplaneMTCDPoint;
class QTimer;
class CSFPL;

//! Azimuth and distance data
/*class CAzimuthAndDistance
{
public:
    CAzimuthAndDistance();
    QString GetAzimuthText(CGUIAirplane* pAirplane);
    QString GetDistanceText(CGUIAirplane* pAirplane);
private:
    bool Init(CGUIAirplane* pAirplane);

    CCoord  m_coord;
    bool    m_bInit;
    bool    m_bFind;
};*/

//! Class for working with airplane's extended formular.
class CGUIAirplaneLabelExt : public QGraphicsWidget, public AppSettings
{
    Q_OBJECT

    static const int kInvalidAzimuth = 361;

public:
    //! Constructor
    CGUIAirplaneLabelExt(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~CGUIAirplaneLabelExt();

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget);
    //@}

    QRectF boundingRectOnView(QGraphicsView *view) const;

    bool move() const { return move_; }

    //! Set new border color.
    void SetColor(const QColor& color)
    { m_color = color; }

    //! Set new position at point
    /*! \param point a QPointF */
    void UpdatePosition(const QPointF point);
    void returnPosition(qreal dif_x, qreal dif_y);
    //! Set pointer CGUIAirplane
    /*! \param pPlane a pointer CGUIAirplane */
    void SetAirplane(CGUIAirplane* pPlane);
    //! Set airplane coordinate
    /*! \param coord a pointer CCoord */
    //void SetPoint(const CCoord* coord);
    //! Set highlight airplane or not
    /*! \param bActive a highlight status */
    void SetActivePlane(bool bActive);
    //! Get user defined azimuth
    /*! \return a azimuth in degree */
    int GetAzimuth2() const { return m_iAzimuth2; }
    //! Set user defined azimuth
    /*! \param degree a azimuth in degree */
    //void SetAzimuth2(int degree);
    //void ClearAzimuth2();
    //void setNextPoint(const QSharedPointer<CPoint> &next_point);
    //! Update grid
    void showGrid(bool bShow);
    //! Reload formular type
    void UpdateFormularType();
    bool isMoved();
    void SetMoved(bool value);
    //{@
    //! Interface CXAppSettings
    virtual void Translate() {}
    virtual void UpdateXFont();
    virtual void SI() {}
    //@}
    void SetColor();
    QPointF GetTopRight() {return mapToParent(m_clRect.topRight());}
    void setType(XGridLabel::LabelType label_type);

signals:
    void lockMovingView();
    void unlockMovingView();

    //! Hold current grid type
public:
    const XGridLabel &GetGridLabel() const { return m_GridLabel; }
    void SetGridLabel(const XGridLabel &V) { m_GridLabel = V; }
private:
    XGridLabel m_GridLabel;

    //! Flag for constructor
public:
    const bool &GetConstructorFlag() const { return m_ConstructorFlag; }
    void SetConstructorFlag(const bool &V) { m_ConstructorFlag = V; }
private:
    bool m_ConstructorFlag;

protected:
    //@{
    //! Inherited from QGraphicsItem
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    //@}

protected slots:
    //! Function is called after editing
    //void OnEndEdit(const CGUILabelText *sender, const QStringList sValues);
    //! Set highlight airplane
    void OnActivePlane();
    //! Assume
    void OnTimerAssume();
    void onRejected();

private slots:
    void adjustGrid();
    void onSpiTimeout();
    void onEmgTimeout();

private:
    void initGrid();
    void loadTypeGrid(bool bFullShowVal);
    void updateGrid();
    //QPointF	intersectRect(const QLineF &line, const QRectF &rect) const;
    //void SetColor();
    //void SetHDG2Text();
    double TransformM_SEC();
    double Transform_Nms_SI();
    double Transform_Knot_SI();
    double TransformFeet(bool bHundredMeters = false);
    //QString GetFLText(const FLVal& val, bool bHundredMeters = false, bool recalcToAirdrome = true) const;
    /*! Pressure on the airfield
        \param FL a Flight Level in feet
    */
    double PressOnAirdromeFromSee(double FL) const;
    //FLVal PressOnAirdromeFromSee(const FLVal &val) const;
    //double PressOnAirdromeToSee(double FL) const;
    //FLVal PressOnAirdromeToSee(const FLVal &val) const;
   // void WriteLog(QString sType, QString sValue);

    XGridLabel& GetGridLabel() { return m_GridLabel; }

    void setLabelColors(XGridLabel::LabelType label_type);
    void fillLabelFields(QGraphicsGridLayout *grid, XGridLabel::LabelType label_type);
    void processFlightLevels(XGridLabel::LabelType label_type);

    void processAcid(const QSharedPointer<CSFPL> &sfpl);
    //void processCfl(CAirplaneData::PlaneType plane_type, const QSharedPointer<CSFPL> &sfpl);

    bool isAssumed() const;
    //bool isSpecSignalExist() const;

    CGUILabelText *fieldByPos(const QPointF &pos) const;
    void updateHdgSpdVrcCflFieldsColor();

private:
    int				m_iPenWidth;
    QColor			m_color;
    QColor			m_colorActive;
    QColor			m_colorForeground;
    QColor			m_colorBackground;
    int				m_Transparent;
    QPointF			m_PlainParentPoint;
    QRectF			m_clRect;
    bool			m_bFullShow;

    bool			m_bMoving;
    bool			m_bActive;
    bool			m_bPress;
    bool			m_bPressGsp;
    bool			m_bPressW;
    bool            m_bMachLevel;
    int				m_iAzimuth2;
    QMap<int,int>	m_mapRoutePoints;
    QMap<int,int>	m_mapHoldingZones;
    QTimer*			m_ActiveTimer;

    CGUIAirplane*	m_pAirplane;

    CGUILabelText*	m_itemCODE;
    CGUILabelText*	m_itemACID;
    CGUILabelText*  m_itemType;

    CGUILabelText*  m_itemAfl;
    CGUILabelText*  m_itemCfl;
    CGUILabelText*  m_itemTfl;

    CGUILabelText*  m_itemSpace;
    CGUILabelText*	m_itemAdep;
    CGUILabelText*	m_itemAdes;
    CGUILabelText*	m_itemAlter;

    CGUILabelText*  m_itemPoint;
    CGUILabelText*	m_itemEq;
    CGUILabelText*	m_itemLit;

    CGUILabelText*  m_itemRoute;
    QString         RouteMiddle;
    CGUILabelText*	m_itemStar;

    CGUILabelText*	m_itemIask;
    CGUILabelText*	m_itemIasm;
    CGUILabelText*	m_itemGsp;

    CGUILabelText*	m_itemHdg;
    CGUILabelText*	m_itemTas;
    CGUILabelText*	m_itemVrc;

    CGUILabelText*	m_itemBps;
    CGUILabelText*	m_itemSfl;
    CGUILabelText*	m_itemAcas;




    //CGUILabelText*	item_spd_;
    CGUILabelText*	item_gsp_;
   // CGUILabelText*	item_w_;

    CGUILabelText *item_cs_debug_;
    CGUILabelText *item_ns_debug_;

    CGUILabelText *item_spec_signal_1;
    CGUILabelText *item_spec_signal_2;
    CGUILabelText *item_spec_signal_3;

    //Ground formular items
    CGUILabelText*  m_groundItemStatus;
    CGUILabelText*  m_groundItemACID;
    CGUILabelText*  m_groundItemSQUAWK;
    CGUILabelText*  m_groundItemSpeed;
    CGUILabelText*  m_groundItemSpeedLabel;




    QPointF drag_point_;
    CAirplaneData::PlaneType plane_type_;

    QTimer *spi_timer_;
    QTimer *emg_timer_;
    bool move_;
};

#endif // GUIAIRPLANELABELEXT_H
