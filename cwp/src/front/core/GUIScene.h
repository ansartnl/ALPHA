#ifndef GUIZONE_H
#define GUIZONE_H

#include "Scene.h"
#include "GUIBound.h"

#include <QGraphicsItemGroup>

class XReminder;

//! Class for the graphical representation of FIR
class CGUIScene:
        public QGraphicsObject,
        public IGUIItemHandler,
        public Scene
{
    Q_OBJECT
public:
    typedef QList<QGraphicsItem*> TListItem;
    //! Constructor
    /*! \param pParent a pointer to IParent */
    CGUIScene();
    //! Destructor
    virtual ~CGUIScene();

    virtual bool AddPoint(const QSharedPointer<CPoint>& pPoint);
    virtual bool AddAirport(const QSharedPointer<CAirport>& pAirport);
    virtual bool AddRoute(const QSharedPointer<CRoute>& pRoute);
    virtual void AddRestriction(const QSharedPointer<CRestriction>& pRestriction);
    virtual void AddUserRestriction(const QSharedPointer<CRestrictionUser>& pRestriction);
    virtual void AddHolding(const QSharedPointer<CHolding>& pHolding);
    virtual void AddSector(const QSharedPointer<Scene>& pSector);
    virtual void AddNote(const QSharedPointer<MapNote>& pNote);
    virtual void AddUserPics(const QSharedPointer<CUserPics>& pUserPics);
    virtual void AddSvgItem(const QSharedPointer<SvgItem>& pSvgItem);

    virtual QSharedPointer<CRestrictionUser> DelUserRestriction(quint32 id);
    virtual QSharedPointer<CUserPics> DelUserPics(quint32 id);
    virtual QSharedPointer<SvgItem> DelSvgItem(quint32 id);

    virtual QSharedPointer<CPoint> CreatePoint(CPoint::Type type) const;
    virtual QSharedPointer<CAirport> CreateAirport() const;
    virtual QSharedPointer<CRunway> CreateRunway() const;
    virtual QSharedPointer<CRoute> CreateRoute() const;
    virtual QSharedPointer<CRestriction> CreateRestriction() const;
    virtual QSharedPointer<CRestrictionUser> CreateUserRestriction() const;
    virtual QSharedPointer<CHolding> CreateHolding() const;
    virtual QSharedPointer<CSSA> CreateSSA() const;
    virtual QSharedPointer<Scene> CreateSector() const;
    virtual QSharedPointer<MapNote> CreateNote() const;
    virtual QSharedPointer<CUserPics> CreateUserPics() const;
    virtual QSharedPointer<SvgItem> CreateSvgItem(const QString& path) const;

    void prepare(qint64& iXMin, qint64& iYMin, qint64& iXMax, qint64& iYMax);

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //@}

    //@{
    //! Interface IGUIItemHandler
    virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
    virtual void SetSI(qint16 SI)
    {
    }
    //@}

    //! Get all items
    const TListItem& GetItemList() const
    {
        return m_ItemList;
    }

    //! Assign layouts
    void SetLayouts();

    void SetSignActivePoint();

    void SetMapColor(const QColor& color);
    void SetFIRColor(const QColor& color);
    void SetBorderColor(const QColor& color);
    void SetSectorColor(const QColor& color);
    void SetApproachColor(const QColor& color);
    void SetBlastoffColor(const QColor& color);
    void SetRouteColor(const QColor& color);
    void SetHoldingColor(const QColor& color);
    void SetCorridorColor(const QColor& color);

public slots:

    void OnNoteDelete(quint32 id);
    void OnNoteEdit(quint32 id);

private:
    int m_iScale;
    qint64 m_qXMin;
    qint64 m_qYMin;
    qint64 m_qXSize;
    qint64 m_qYSize;

    QPainterPath m_Path;
    int m_iPenWidth;
    TListItem m_ItemList;

    QColor penColor;
    QColor brushColor;
};

#endif // GUIZONE_H
