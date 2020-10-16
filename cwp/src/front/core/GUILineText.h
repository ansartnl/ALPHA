#ifndef GUILINETEXT_H
#define GUILINETEXT_H

#include <QGraphicsItem>
#include <QFont>
#include "GUIInterface.h"
#include "Coordinate.h"
#include "front/common/XAppSettings.h"

class CAirplane;

//! Class implements graphics line with text
class CGUILineText :
    public QGraphicsItem
    , public IGUIItemHandler
    , public AppSettings
{
public:
    //! Interface to CGUILineText
    class IParent
    {
    public:
        virtual ~IParent() {};
        virtual void DoubleClickLineText(CGUILineText* line_text, QPointF scene_pos) = 0;
    };

public:
    enum AutoText	{None = 0, Distance, DistanceWithAngle, DistanceWithAngleAndTime, DistanceWithMileAndAngle, DistanceWithMileAngleTime };
    enum ShowPoint	{NonePoints = 0, LeftPoint = 1, RightPoint = 2, AllPoints = 3};

    //! Constructor
    /*!
        \param pParent a pointer to IParent
        \param parent a parent QGraphicsItem
    */
    explicit CGUILineText(IParent *pParent = 0, QGraphicsItem *parent = 0);
    //! Destructor
    virtual ~CGUILineText();

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

    //@{
    //! Interface IGUIItemHandler
    virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
    //@}

    //{@
    //! Interface CXAppSettings
    virtual void Translate() {}
    virtual void UpdateXFont();
    virtual void SI();
    //@}

    void SetColor(const QColor &color)	{ m_color = color; }
    void SetPenWidth(const int iVal)	{ m_iPenWidth = iVal; }
    void SetFont(const QFont &font)		{ m_font = font; }
    void SetAutoText(CGUILineText::AutoText type)	{ m_autoText = type; }
        void SetTurnTimes(quint32 forward, quint32 side, quint32 backward)
        { m_timeToForwardSector = forward; m_timeToSideSector = side; m_timeToBackwardSector = backward; }

    void setPositionAndPoints(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord, const CCoord& p1, const CCoord& p2, ShowPoint showPoint = AllPoints);
    void SetPoints(const CCoord& p1, const CCoord& p2, ShowPoint showPoint = AllPoints);
    void SetPoints(const CCoord& p1, const QSharedPointer<CAirplane>& p2, ShowPoint showPoint = AllPoints);
    void SetPoints(const QSharedPointer<CAirplane>& p1, const QSharedPointer<CAirplane>& p2, ShowPoint showPoint = AllPoints);
    void SetText(const QString &sText, Qt::Alignment alignment = Qt::AlignHCenter);
    QString GetText() {return m_sText;}

    void SetPoint1(const CCoord& p1, ShowPoint showPoint = AllPoints);
    void SetPoint1(const QSharedPointer<CAirplane>& p1, ShowPoint showPoint = AllPoints);
    void SetPoint2(const CCoord& p2, ShowPoint showPoint = AllPoints);
    void SetPoint2(const QSharedPointer<CAirplane>& p2, ShowPoint showPoint = AllPoints);

    void SetUseTextShape(bool bUse)
    { m_bUseTextShape = bUse; }

    void Update();

    QLineF GetLine() const;

    CCoord GetCoordPoint1() const;
    CCoord GetCoordPoint2() const;

    QSharedPointer<CAirplane> GetPlane1() const;
    QSharedPointer<CAirplane> GetPlane2() const;

    //@{
    //! user-defined properties
    void SetProperty(const QString &sName, const QVariant &val)
    { m_mapProperty.insert(sName, val); }
    QVariant GetProperty(const QString &sName) const
    { return m_mapProperty[sName]; }
    //@}

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void Recalculate();
    QPointF GetPoint(const CCoord& coord) const;
    void AddAutoText(QString& sText, QString &sTimeText);

    void getPoint(const CCoord &coord, bool is_plane, const QWeakPointer<CAirplane> &plane, QPointF &point);

private:
    int			m_iPenWidth;
    QColor		m_color;
    QFont		m_font;
    qint64		m_qllXMin;
    qint64		m_qllYMax;
    double		m_dScale;
    CCoord		m_CentralCoord;
    IParent*	m_pParent;

    QPointF						m_point1;
    QPointF						m_point2;
    QPointF						m_point1Orig;
    QPointF						m_point2Orig;
    ShowPoint					m_showPoints;
    QPainterPath				m_pathLine;
    QPainterPath				m_pathText;
    QTransform					m_transform;
    QTransform					m_textTransform;

    CCoord						m_Coord1;
    CCoord						m_Coord2;
    QWeakPointer<CAirplane>		m_pPlane1;
    QWeakPointer<CAirplane>		m_pPlane2;
    bool						m_bIsPlane1;
    bool						m_bIsPlane2;

    QString			m_sText, m_sTimeText;
    Qt::Alignment	m_alignment;
    AutoText		m_autoText;
        /** Average time in secs to reach the forward, side or backward sectors relative
            to the plane direction */
        quint32         m_timeToForwardSector, m_timeToSideSector, m_timeToBackwardSector;

    bool            m_bUseTextShape;

    QMap<QString,QVariant>      m_mapProperty;
    int                         m_Measure;
};

Q_DECLARE_METATYPE(CGUILineText*)

#endif // GUILINETEXT_H
