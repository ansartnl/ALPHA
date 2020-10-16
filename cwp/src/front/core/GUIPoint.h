#ifndef GUIPOINT_H
#define GUIPOINT_H

#include "GUIObject.h"
#include "import/Point.h"
#include "GUIInterface.h"

//! Class for the graphical representation of FIR's point
class CGUIPoint
	: public CPoint,
	public CGUIObject,
	public IGUIItemHandler
{
    class CPointPixmap
    {
    public:
        CPointPixmap();
        ~CPointPixmap();
        const QPixmap& GetPixmap(CPoint::Type type);
    private:
        typedef QMap<CPoint::Type, QPixmap> TPointMap;
        TPointMap   m_map;
    };

public:
	//! Constructor
	CGUIPoint();

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
	QPointF			m_CenterPoint;
	QRectF			m_Zone;
	int				m_iPenWidth;
    static CPointPixmap m_Pixmaps;
};

#endif // GUIPOINT_H
