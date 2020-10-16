#ifndef GUIUSERPICS_H
#define GUIUSERPICS_H

#include "GUIInterface.h"
#include "UserPics.h"
#include "GUIObject.h"

//! Class for the graphical representation of user pics
class CGUIUserPics
    : public CUserPics,
    public CGUIObject,
    public IGUIItemHandler
{
public:
    //! Constructor
    CGUIUserPics(QGraphicsItem * parent = 0);
    //! Destructor
    virtual ~CGUIUserPics();
    void setSelectedPoint(int row, bool selected);
    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

    //@{
    //! Interface IGUIItemHandler
    virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
    virtual void SetSI(qint16 SI) {}
    //@}

    //! Inherited from ItemHandler
    virtual void MarkItemActive(bool bActive);

private:
    int             m_iPenWidth;
    QPainterPath    m_painterPath;
    bool            m_bPrimitiveArc;
    bool            m_draw_selected;
    int             m_index_selected_point;
    double          m_last_scale;
    qint64          m_last_qllXMin;
    qint64          m_last_qllYMax;
    QPointF         m_selected_point;
};

#endif // GUIUSERPICS_H
