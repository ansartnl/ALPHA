#include "GUIAzimuth.h"
#include <QtGui>
#include <QToolTip>
#include <math.h>

CGUIAzimuth::CGUIAzimuth(QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent, f), m_ReadOnly(true), m_iAzimuth1(0), m_iAzimuth2(0)
{
	setMinimumSize(200, 200);
	QFont font;
	font.setPointSize(5);
	setFont(font);
	//setMouseTracking(true);
}

CGUIAzimuth::~CGUIAzimuth()
{

}

void CGUIAzimuth::setAzimuth1(int degrees)
{
	m_iAzimuth1 = qBound(0, degrees, 360);
	m_iAzimuth1 %= 360;
	update();
}

void CGUIAzimuth::setAzimuth2(int degrees)
{
	m_iAzimuth2 = qBound(0, degrees, 360);
	m_iAzimuth2 %= 360;
	update();
}

int CGUIAzimuth::getAzimuth1() const
{
	return m_iAzimuth1;
}

int CGUIAzimuth::getAzimuth2() const
{
	return m_iAzimuth2;
}

void CGUIAzimuth::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
	if(m_ReadOnly) return;
	if((event->button() == Qt::LeftButton) && rect().contains(event->pos()))
	{
		QPointF point = event->pos() - rect().center();
		double theta = atan2(-point.x(), -point.y()) * 180.0 / M_PI;
		setAzimuth2(theta < 0 ? int(qAbs(theta)) : int(360-theta));
		emit changedAzimuth(getAzimuth2());
		update();
	}
}

bool CGUIAzimuth::event(QEvent *event)
{
	if (event->type() == QEvent::ToolTip)
	{
		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
		if(helpEvent && (rect().contains(helpEvent->pos())))
		{
			QPointF point = helpEvent->pos() - rect().center();
			double theta = atan2(-point.x(), -point.y()) * 180.0 / M_PI;
			int degree = theta < 0 ? int(qAbs(theta)) : int(360-theta);
			degree %= 360;
			//QToolTip::showText(helpEvent->globalPos(), QString::number(degree) + QString::fromUtf8("\u00B0"), this);
			QToolTip::showText(QCursor::pos(), QString::number(degree) + QString::fromUtf8("\u00B0"), this);
		}
	}
	return QWidget::event(event);
}

void CGUIAzimuth::mouseMoveEvent(QMouseEvent *event)
{
	//QToolTip::hideText();
	QWidget::mouseMoveEvent(event);
}

void CGUIAzimuth::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

	draw(&painter);
}

void CGUIAzimuth::draw(QPainter *painter)
{
	QPen thickPen(palette().foreground(), 1.5);
	QPen thinPen(palette().foreground(), 0.5);
	QColor niceBlue(150, 150, 200);

	painter->setPen(Qt::NoPen);

	QLinearGradient backGradient(0, 0, width(), height());
	backGradient.setColorAt(0.0, Qt::white);
	backGradient.setColorAt(1.0, niceBlue);

	painter->setBrush(backGradient);
	painter->drawRect(0, 0, width(), height());

	int side = qMin(width(), height());

	painter->setViewport((width() - side) / 2, (height() - side) / 2,
						side, side);
	painter->setWindow(-50, -50, 100, 100);

	painter->setPen(thinPen);

	QConicalGradient coneGradient(0, 0, -90.0);
	coneGradient.setColorAt(0.0, Qt::darkGray);
	coneGradient.setColorAt(0.2, niceBlue);
	coneGradient.setColorAt(0.5, Qt::white);
	coneGradient.setColorAt(1.0, Qt::darkGray);

	painter->setBrush(coneGradient);
	painter->drawEllipse(-46, -46, 92, 92);
/*
	QStringList sRhumbs;
	sRhumbs << tr("N") << tr("NE") << tr("E") << tr("SE") << tr("S") << tr("SW") << tr("W") << tr("NW");
	for (int i = 0; i < 32; ++i) {
		painter->save();
		painter->rotate(i*11.25);
		if (i % 4 == 0) {
			painter->setPen(thickPen);
			painter->drawLine(0, -41, 0, -44);
			painter->drawText(-15, -41, 30, 30,
							  Qt::AlignHCenter | Qt::AlignTop,
							  sRhumbs[i/4]);
		} else {
			painter->setPen(thinPen);
			painter->drawLine(0, -42, 0, -44);
		}
		painter->restore();
	}
*/
/*
	for (int i = 0; i < 72; ++i) {
		painter->save();
		painter->rotate(i*5);
		if (i % 6 == 0) {
			painter->setPen(thickPen);
			painter->drawLine(0, -41, 0, -44);
			if(i) painter->drawText(-15, -41, 30, 30,
							  Qt::AlignHCenter | Qt::AlignTop,
							  QString::number(i*5));
		} else {
			painter->setPen(thinPen);
			if(i % 2)
			{
				painter->drawLine(0, -42, 0, -44);
			}
			else
			{
				painter->drawPoint(0, -44);
			}
		}
		painter->restore();
	}
*/
	for (int i = 0; i < 36; ++i) {
		painter->save();
		painter->rotate(i*10);
		if (i % 3 == 0) {
			painter->setPen(thickPen);
			painter->drawLine(0, -41, 0, -44);
			if(i) painter->drawText(-15, -41, 30, 30,
							  Qt::AlignHCenter | Qt::AlignTop,
							  QString::number(i*10));
		} else {
			painter->setPen(thinPen);
			painter->drawLine(0, -42, 0, -44);
		}
		painter->restore();
	}

	painter->save();
	painter->setBrush(Qt::lightGray);
	painter->setPen(thinPen);
	painter->rotate(getAzimuth1());
	QPolygon polPoint;
	polPoint << QPoint(0,10) << QPoint(-3,3) << QPoint(-10,0) << QPoint(-3,-3) << QPoint(0,-32)
			<< QPoint(3,-3) << QPoint(10,0) << QPoint(3,3) << QPoint(0,10);
	painter->drawPolygon(polPoint);
	painter->drawLine(-3,-3, 3,3);
	painter->drawLine(3,-3, -3,3);
	painter->drawLine(0,0, 0,9);
	painter->drawLine(-9,0, 9,0);
	painter->restore();

	painter->save();
	QPen qPen(QColor(0, 0, 255, 100));
	qPen.setWidth(2);
	qPen.setCapStyle(Qt::RoundCap);
	painter->setPen(qPen);
	painter->rotate(getAzimuth2());
	painter->drawLine(0,0, 0,-26);
	painter->restore();
}
