#include "GUIPixmapItem.h"
#include <QGraphicsGridLayout>
#include "src/front/core/GUIInterface.h"

CGUIPixmapItem::CGUIPixmapItem(QGraphicsItem *parent/* = 0*/)
	 : QGraphicsPixmapItem(parent), QGraphicsLayoutItem(),
	 m_Visible(true)
 {
	 setGraphicsItem(this);
     setZValue(gui::Zlabelitem);
	 //setFlag(QGraphicsItem::ItemIsSelectable, true);
	 setAcceptedMouseButtons(Qt::NoButton);
	 setOpacity(1);
}

CGUIPixmapItem::~CGUIPixmapItem()
{

}

void CGUIPixmapItem::setGeometry(const QRectF &geom)
{
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	QPointF point(geom.topLeft());
	QGraphicsLayoutItem* pParentLayout = parentLayoutItem();
	if(pParentLayout)
	{
		QGraphicsGridLayout* pGrid = dynamic_cast<QGraphicsGridLayout*>(pParentLayout);
		if(pGrid)
		{
			Qt::Alignment alignment = pGrid->alignment(this);

			if(alignment & Qt::AlignLeft)
			{
			}
			if(alignment & Qt::AlignRight)
			{
				point = QPointF(geom.topRight());
				point.rx() -= boundingRect().width();
			}
			if(alignment & Qt::AlignHCenter)
			{
				point.rx() += geom.width()/2 - boundingRect().width()/2;
			}
		}
	}
	setPos(point);
}

QSizeF CGUIPixmapItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
	switch (which)
	{
		case Qt::MinimumSize:
		case Qt::PreferredSize:
			return boundingRect().size();
		case Qt::MaximumSize:
			return QSize(1000,1000);
		default:
			break;
	}
	return constraint;
 }

void CGUIPixmapItem::updateGeometry()
{
	prepareGeometryChange();
	QGraphicsLayoutItem::updateGeometry();
}
