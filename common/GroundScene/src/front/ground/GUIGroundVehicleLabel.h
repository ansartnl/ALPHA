#ifndef GUIGROUNDVEHICLELABEL_H
#define GUIGROUNDVEHICLELABEL_H

#include <QGraphicsWidget>

#include "back/ground/GroundMain.h"

#include "front/core/airplane/GUILabelText.h"
#include "front/core/airplane/GUIPixmapItem.h"

#ifndef GROUND_CWP
#include "front/core/Language.h"
#endif

class GUIGroundVehicle;
class CCoord;

class GUIGroundVehicleLabel :
        public QGraphicsWidget
#ifndef GROUND_CWP
        , public ILanguage
#endif
{
    Q_OBJECT

    static const char* BACKGROUND_STYLE;

public:
    explicit GUIGroundVehicleLabel(QGraphicsItem *parent = 0);
    
    virtual ~GUIGroundVehicleLabel();

    //! ILanguage
    void Translate();
    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget);
    //@}

    //! Set new position at point
    /*! \param point a QPointF */
    void UpdatePosition(const QPointF point);
    //! Set pointer CGUIAirplane
    /*! \param pPlane a pointer CGUIAirplane */
    void SetVehicle(GUIGroundVehicle* pVehicle);
    //! Set airplane coordinate
    /*! \param coord a pointer CCoord */
    void SetPoint(const CCoord* coord);
    //! Get user defined azimuth
    /*! \return a azimuth in degree */
    int  GetAzimuth2() const;
    //! Set user defined azimuth
    /*! \param degree a azimuth in degree */
    void SetAzimuth2(int degree);
    //! Update(redraw) label
    void UpdateLabel();
    //! Set auto collapse
    void SetAutoCollapse(bool bCollapse);

protected:
    //@{
    //! Inherited from QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual bool event(QEvent *event);

//    QVariant itemChange(GraphicsItemChange change,
//                        const QVariant &value);
    //@}

protected slots:
    //! Function is called after editing
    void OnEndEdit(const CGUILabelText* sender, const QStringList sValues);
    void OnUpdateLabel();
    void OnUpdateFont();

    void onMove();
    void onReverse();
    void onStop();

private:
    void initGrid();
    void loadTypeGrid(bool bFullShow);
    void updateGrid();
    void adjustGrid();
    void showGrid(bool bShow);
    QPointF    intersectRect(const QLineF line, const QRectF rect) const;
    void SetColor();
    double TransformM_SEC();
    double TransformFeet(bool bHundredMeters = false);
    void switchButtons(bool isMoving);

private:
    int                m_iPenWidth;
    QColor            m_color;
    QBrush            m_backgroundBrush;
    QPointF            mVehicleParentPoint;
    QRectF            m_clRect;
    bool            m_bMoving;
    bool            m_bCheck;
    bool            m_bAutoCollapse;
    bool            m_bFullShow;
    int                m_iAzimuth2;
    QTimer*            m_ActiveTimer;

    GUIGroundVehicle*    mVehicle;

    // formular items
    CGUIPixmapItem* mItemCheck;
    CGUILabelText*  mItemName;
    CGUILabelText*  mItemSpeed;
    CGUILabelText*  mItemRoute;

    QGraphicsWidget* mButtonFrame;
    QGraphicsProxyWidget* m_groundButtonMove;
    QGraphicsProxyWidget* m_groundButtonReverse;
    QGraphicsProxyWidget* m_groundButtonStop;

    QGraphicsItem*   m_parent;
#ifndef GROUND_CWP
    QSharedPointer<atd::CPoint>      mNextPointCoord;
#endif
};

#endif // GUIGROUNDVEHICLELABEL_H
