#ifndef GUIMETEO_H
#define GUIMETEO_H

#include <QGraphicsItem>
#include "../GUIInterface.h"
#include "back/core/meteo/Meteo.h"

//! Class for displaying meteo zones
class CGUIMeteo
	: public core::CMeteo,
	public QGraphicsItem,
	public IGUIItemHandler
{
public:
	//! Constructor
	CGUIMeteo(QGraphicsItem * parent = 0);
	//! Destructor
	virtual ~CGUIMeteo();

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

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

private:
	int				m_iPenWidth;
	QColor			m_color;
	QPainterPath	m_painterPath;
};

#endif // GUIMETEO_H
