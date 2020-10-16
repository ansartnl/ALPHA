#ifndef GUICONFLICT_H
#define GUICONFLICT_H

#include <QGraphicsWidget>
#include "front/common/XAppSettings.h"

class CGUILabelText;
class CAirplane;

//! Class for showing airplane's reminders
class CGUIConflict : public QGraphicsWidget
{
Q_OBJECT
public:
    //! Constructor
    CGUIConflict(QGraphicsItem * parent = 0);
    //! Destructor
    ~CGUIConflict();

    //! Set conflicts
    /*! \param list a list of conflicts */
    void SetConflicts(QList<QPair<QString, QString> > list);
    //! Set widget width
    void UpdateWidth();
    //! Set pointer CAirplane, current airplane
    /*! \param pPlane a pointer CAirplane */
    void SetPlane(CAirplane* pPlane);

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
//    virtual void paint(
//            QPainter *painter,
//            const QStyleOptionGraphicsItem *option,
//            QWidget *widget);
    //@}

private:
    void adjustGrid();

private:
    int				m_iPenWidth;
    QColor			m_color;
    QBrush			m_backgroundBrush;
    QRectF			m_clRect;

    QList<QPair<QString, QString> >	m_Conflicts;
    int							m_iCurIndex;

    CAirplane*					m_pPlane;

    QList<CGUILabelText*> m_items;
};

#endif // GUICONFLICT_H
