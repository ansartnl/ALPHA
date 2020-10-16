#include "GUIMapNote.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <math.h>

#include <QStaticText>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include <QSettings>

#include "back/utils/ProfileSettings.h"

GUIMapNote::GUIMapNote(QGraphicsItem *parent)
    :QGraphicsItem(parent),
      mTextColor(Qt::black), mPenWidth(1)
{
    setZValue(21);

    mDropDownMenu = new QMenu();
    mRemoveAction = new QAction(tr("Remove"), this);
    connect(mRemoveAction, SIGNAL(triggered()), this, SLOT(OnRemove()));
    mEditAction = new QAction(tr("Edit"), this);
    connect(mEditAction, SIGNAL(triggered()), this, SLOT(OnEdit()));

    mDropDownMenu->addAction(mRemoveAction);
    mDropDownMenu->addAction(mEditAction);

    connect(this, SIGNAL(destroyed()), mDropDownMenu, SLOT(deleteLater()));
    connect(this, SIGNAL(destroyed()), mRemoveAction, SLOT(deleteLater()));
}

QRectF GUIMapNote::boundingRect() const
{
    return mTextZone;
}

QPainterPath GUIMapNote::shape() const
{
    QPainterPath shape;
    shape.addRect(mTextZone);
    return shape;
}

void GUIMapNote::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPen qPen(mTextColor);
    qPen.setWidth(mPenWidth);
    painter->setPen(qPen);
    painter->drawText(mTextZone, Qt::TextWordWrap, text());
    painter->restore();
}

void GUIMapNote::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    mTextZone = QRectF();
    QPointF point(
        (coord().GetX() - qllXMin) * dScale,
        (qllYMax - coord().GetY()) * dScale);
    point = mapFromScene(point);

    QFontMetricsF metrics(scene()->font());

    QStringList lines = text().split("\n");
    foreach(const QString& line, lines)
    {
        if(metrics.boundingRect(line).width() > mTextZone.width())
            mTextZone = QRectF(metrics.boundingRect(line));
    }
    mTextZone.setHeight(mTextZone.height() * lines.size());
    mTextZone.adjust(-1, -1, 1, 1);
    mTextZone.moveTo(point);

    mTextColor = QColor(Qt::black);
    QColor colorTemp = UserProfile::CSettings::Instance().GetColor(UserProfile::CtMapNote);
    if(colorTemp.isValid())
        mTextColor = colorTemp;

    update();
}

void GUIMapNote::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    mDropDownMenu->popup(QCursor::pos());
}

void GUIMapNote::OnRemove()
{
    emit deleteNote(id());
}

void GUIMapNote::OnEdit()
{
    emit editNote(id());
}
