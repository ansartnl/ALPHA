#ifndef GUIAIRPLANELABEL_H
#define GUIAIRPLANELABEL_H

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
class CGUIConflict;
class CGUILabelText;
class CGUIReminder;
class CGUIAirplaneMTCDPoint;
class QTimer;
class CSFPL;
class QGraphicsLinearLayout;

//! Azimuth and distance data
class CAzimuthAndDistance
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
};

//! Class for working with airplane's characteristics
class CGUIAirplaneLabel : public QGraphicsWidget, public AppSettings
{
    Q_OBJECT

    static const int kInvalidAzimuth = 361;

public:
    //! Constructor
    CGUIAirplaneLabel(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~CGUIAirplaneLabel();

    QGraphicsLinearLayout *grid;
    QGraphicsLinearLayout *linear0;
    QGraphicsLinearLayout *linear1;
    QGraphicsLinearLayout *linear1_1;
    QGraphicsLinearLayout *linear2;
    QGraphicsLinearLayout *linear3;
    QGraphicsLinearLayout *linear4;
    QGraphicsLinearLayout *linear5;

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget);
    //@}

    bool move() const { return move_; }
    bool twitch() const { return twitch_; }

    //! Set new border color.
    void SetColor(const QColor& color)
    { m_color = color; }

    //! Set new position at point
    /*! \param point a QPointF */
    void UpdatePosition(const QPointF point);
    //void returnPosition(const QPointF delta);
    void returnPosition(qreal dif_x, qreal dif_y);
    //! Set pointer CGUIAirplane
    /*! \param pPlane a pointer CGUIAirplane */
    void SetAirplane(CGUIAirplane* pPlane);
    //! Set airplane coordinate
    /*! \param coord a pointer CCoord */
    void SetPoint(const CCoord* coord);
    //! Set highlight airplane or not
    /*! \param bActive a highlight status */
    void SetActivePlane(bool bActive);
    //! Get user defined azimuth
    /*! \return a azimuth in degree */
    int GetAzimuth2() const { return m_iAzimuth2; }
    //! Set user defined azimuth
    /*! \param degree a azimuth in degree */
    void SetAzimuth2(int degree);
    void ClearAzimuth2();
    void setNextPoint(const QSharedPointer<CPoint> &next_point);
    //! Update grid
    void showGrid(bool bShow);
    //! Reload formular type
    void UpdateFormularType();

    //{@
    //! Interface CXAppSettings
    virtual void Translate() {}
    virtual void UpdateXFont();
    virtual void SI();
    //@}
    void SetColor();
    QPointF GetTopRight() {return mapToParent(m_clRect.topRight());}
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
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    //@}

protected slots:
    //! Function is called after editing
    void OnEndEdit(const CGUILabelText *sender, const QStringList sValues);
    //! Set highlight airplane
    void OnActivePlane();
    //! Assume
    void OnTimerAssume();
    void onDialogOpened();
    void onDialogClosed();
	void afterSleep();

private slots:
    void adjustGrid();
    void onSpiTimeout();
    void onEmgTimeout();

private:
    void initGrid();
    void loadTypeGrid(bool bFullShowVal);
    void updateGrid();
    QPointF	intersectRect(const QLineF &line, const QRectF &rect) const;
    //void SetColor();
    void SetHDG2Text();
    double TransformM_SEC();
    double Transform_Nms_SI();
    double Transform_Knot_SI();
    double TransformFeet(bool bHundredMeters = false);
    QString GetFLText(const FLVal& val, bool bHundredMeters = false, bool recalcToAirdrome = true) const;
    /*! Pressure on the airfield
        \param FL a Flight Level in feet
    */
    double PressOnAirdromeFromSee(double FL) const;
    FLVal PressOnAirdromeFromSee(const FLVal &val) const;
    double PressOnAirdromeToSee(double FL) const;
    FLVal PressOnAirdromeToSee(const FLVal &val) const;
    void WriteLog(QString sType, QString sValue);

    XGridLabel& GetGridLabel() { return m_GridLabel; }

    void setLabelColors(XGridLabel::LabelType label_type);
    void fillLabelFields(QGraphicsLinearLayout *grid, XGridLabel::LabelType label_type);
    void processFlightLevels(XGridLabel::LabelType label_type);

    void processAcid(const QSharedPointer<CSFPL> &sfpl);
    void processCfl(CAirplaneData::PlaneType plane_type, const QSharedPointer<CSFPL> &sfpl);

    bool isAssumed() const;
    bool isSpecSignalExist() const;

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
    //bool			m_bShowReminder;
    bool			m_bMoving;
    bool			m_bActive;
    bool			m_bPress;
    bool			m_bPressGsp;
    bool			m_bPressW;
    bool            m_bMachLevel;
    bool            m_bGSPorIAS;
    bool            m_bNextPointIAS;
    int				m_iAzimuth2;
    QMap<int,int>	m_mapRoutePoints;
    QMap<int,int>	m_mapHoldingZones;
    QTimer*			m_ActiveTimer;

    CGUIAirplane*	m_pAirplane;

    CGUILabelText*	m_itemCODE;
    CGUILabelText*	m_itemACID;
    CGUILabelText*	m_itemLX;
    CGUILabelText*	m_itemCS;
    CGUILabelText*	m_itemAFL;
    CGUILabelText*	m_itemTEND;
    CGUILabelText*	m_itemCFL;
    CGUILabelText*	m_itemTFL;
    CGUILabelText*	m_itemVRC;
    CGUILabelText*	m_itemHDG;
    CGUILabelText*	m_itemHDG2;
    CGUILabelText*	m_itemROLL;
    CGUILabelText*	m_itemNextPoint;
    CGUILabelText*	m_itemSPD1;
    CGUILabelText*	m_itemSPD2;
    CGUILabelText*	m_itemV;
    CGUILabelText*	m_itemUVD;
    CGUILabelText*	m_itemPEL;
    CGUILabelText*	m_itemRFL;
    CGUILabelText*	m_itemAzimuth;
    CGUILabelText*	m_itemDistance;
    CGUILabelText*	m_itemQNH;

    CGUILabelText*	item_spd_;
    CGUILabelText*	item_gsp_;
    CGUILabelText*	item_w_;

    CGUILabelText *item_cs_debug_;
    CGUILabelText *item_ns_debug_;

    CGUILabelText *item_spec_signal_;
    CGUILabelText *item_assume_;
    CGUILabelText *item_transfer_;
    CGUILabelText *item_act_out_;
    CGUILabelText *m_itemL;
    CGUILabelText *m_itemM;
    CGUILabelText *m_itemLM;
    CGUILabelText *m_Mhdg;
    CGUILabelText *m_Runway;
    CGUILabelText *m_itemType;

    //Ground formular items
    CGUILabelText*  m_groundItemStatus;
    CGUILabelText*  m_groundItemACID;
    CGUILabelText*  m_groundItemSQUAWK;
    CGUILabelText*  m_groundItemSpeed;
    CGUILabelText*  m_groundItemSpeedLabel;
//    CGUILabelText*  m_groundItemMove;
//    CGUILabelText*  m_groundItemHold;
//    CGUILabelText*  m_groundItemStop;
//    CGUILabelText*  m_groundItemRoute;

    //CGUIReminder*	m_pReminder;
    CGUIConflict* m_pConflict;

    CGUIAirplaneMTCDPoint*  m_pMTCDpoint;

    CAzimuthAndDistance m_AzimuthAndDistance;

    static EchelonValues    m_echelonValues;

    QPointF drag_point_;
    CAirplaneData::PlaneType plane_type_;
    bool spec_signal_;
    QTimer *spi_timer_;
    QTimer *emg_timer_;
    bool move_;
    bool twitch_;

    enum NextPointType
    {
        NextPoint, Mhdg, PathPoints
    };

    NextPointType np_type;
    QString NextPointText;

    static QSet<CGUIAirplaneLabel*> liveObjects;
};

#endif // GUIAIRPLANELABEL_H
