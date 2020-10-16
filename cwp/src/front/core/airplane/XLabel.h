#ifndef XLABEL_H
#define XLABEL_H

#include <QGraphicsSimpleTextItem>
#include "front/common/XGridLabel.h"

//! Class for the graphical representation of an element of the formular
class CXLabelItem : public QObject, public QGraphicsSimpleTextItem
{
Q_OBJECT

public:
	class ILabelHandler
	{
	public:
		//! Destructor
		virtual ~ILabelHandler(){};

		//! Set item position
		/*!
		  \param pItem a pointer to CXLabelItem
		  \param scenePos a position point
		*/
		virtual void SetPos(CXLabelItem* pItem, QPointF scenePos) = 0;
	};

	//! Constructor
	CXLabelItem(ILabelHandler* pLabel, XGridItem::LabelItemType type, QGraphicsItem* parent = 0);
	//! Destructor
	virtual ~CXLabelItem();

	void Translate();

	//@{
	//! Inherited from QGraphicsSimpleTextItem
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	//@}

protected:
	//@{
	//! Inherited from QGraphicsSimpleTextItem
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	QVariant itemChange(GraphicsItemChange change,
						const QVariant &value);
	//@}

	
public:
    const XGridItem::LabelItemType &GetType() const { return m_Type; }
    void SetType(const XGridItem::LabelItemType &V) { m_Type = V; }
private:
    XGridItem::LabelItemType m_Type;
	
public:
    const QPointF &GetDefaultPoint() const { return m_DefaultPoint; }
    void SetDefaultPoint(const QPointF &V) { m_DefaultPoint = V; }
private:
    QPointF m_DefaultPoint;
	
public:
    const QColor &GetForeground() const { return m_Foreground; }
    void SetForeground(const QColor &V);// { m_Foreground = V; }
private:
    QColor m_Foreground;
	
public:
    const QColor &GetBackground() const { return m_Background; }
    void SetBackground(const QColor &V) { m_Background = V; }
private:
    QColor m_Background;
	
public:
    const quint8 &GetTransparent() const { return m_Transparent; }
    void SetTransparent(const quint8 &V) { m_Transparent = V; }
private:
    quint8 m_Transparent;

private:
	ILabelHandler*	m_pLabel;
	bool			m_bMoving;
};

#endif // XLABEL_H
