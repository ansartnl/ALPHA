#ifndef GUIRUNWAY_H
#define GUIRUNWAY_H

#include "GUIObject.h"
#include "GUIInterface.h"
#include "Runway.h"

//! Class for the graphical representation of runway
class CGUIRunway
    : public CRunway,
    public CGUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
    CGUIRunway(QGraphicsItem * parent = 0);
	//! Destructor
    virtual ~CGUIRunway();

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
	virtual void SetSI(qint16 SI) {}
	//@}

	//! Inherited from ItemHandler
	virtual void MarkItemActive(bool bActive);

private:
	int				m_iPenWidth;
	QPainterPath	m_painterPath;
    QRectF          m_textRect;
};

#endif // GUIRUNWAY_H
