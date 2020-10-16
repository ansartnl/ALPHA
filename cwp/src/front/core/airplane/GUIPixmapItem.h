#ifndef GUIPIXMAPITEM_H
#define GUIPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsLayoutItem>

//! Class extends QGraphicsPixmapItem
class CGUIPixmapItem : public QGraphicsPixmapItem, public QGraphicsLayoutItem
{
public:
	//! Constructor
	CGUIPixmapItem(QGraphicsItem * parent = 0);
	//! Destructor
	virtual ~CGUIPixmapItem();

	//! Is item visible or not
	
public:
    const bool &GetVisible() const { return m_Visible; }
    void SetVisible(const bool &V) { m_Visible = V; }
private:
    bool m_Visible;

public:
	//@{
	//! Inherited from QGraphicsLayoutItem
	void setGeometry(const QRectF &geom);
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
	void updateGeometry();
	//@}
};

#endif // GUIPIXMAPITEM_H
