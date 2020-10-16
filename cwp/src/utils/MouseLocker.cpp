#include "MouseLocker.h"

#include <QApplication>
#include <QMouseEvent>
#include <QWidget>

MouseLocker * mouseLocker(QObject *parent)
{
    static MouseLocker *locker = new MouseLocker(parent);
    return locker;
}

MouseLocker::MouseLocker(QObject *parent) :
    QObject(parent), mMonitoredWidget(0), mOldMouseTracking(false), mActivated(false)
{
    qApp->installEventFilter(this);
}

void MouseLocker::lockInside(QWidget *w)
{
    if (mMonitoredWidget) {
        disconnect(mMonitoredWidget, 0, this, 0);
        mMonitoredWidget->setMouseTracking(mOldMouseTracking);
    }

    if (w) {
        mOldMouseTracking = w->hasMouseTracking();
        w->setMouseTracking(true);
        connect(w, SIGNAL(destroyed()), SLOT(onDestroyed()));
        if (w->isActiveWindow()) {
            centerMouse(w);
            mActivated = true;
        } else {
            mActivated = false;
        }
    }

    mMonitoredWidget = w;
}

bool MouseLocker::eventFilter(QObject *obj, QEvent *e)
{
    if (!mMonitoredWidget)
        return QObject::eventFilter(obj, e);

    if (e->type() == QEvent::MouseMove || e->type() == QEvent::HoverMove) {
        const QRect widgetRect(mMonitoredWidget->mapToGlobal(QPoint(0, 0))
                               , mMonitoredWidget->geometry().size());
        const QPoint mousePos = QCursor::pos();

        QPoint futurePos = mousePos;
        bool revert = false;
        if (mousePos.x() <= widgetRect.left()) {
            futurePos.setX(widgetRect.left() + 1);
            revert = true;
        } else if (mousePos.x() >= widgetRect.right() - 2) {
            futurePos.setX(widgetRect.right() - 3);
            revert = true;
        }
        if (mousePos.y() <= widgetRect.top()) {
            futurePos.setY(widgetRect.top() + 1);
            revert = true;
        } else if (mousePos.y() >= widgetRect.bottom()) {
            futurePos.setY(widgetRect.bottom() - 1);
            revert = true;
        }
        if (revert)
            QCursor::setPos(futurePos);

        return false;
    } else if (e->type() == QEvent::WindowActivate && !mActivated) {
        centerMouse(mMonitoredWidget);
        mActivated = true;

        return false;
    }
    return QObject::eventFilter(obj, e);
}

void MouseLocker::centerMouse(QWidget *w)
{
    const QRect rect(w->mapToGlobal(QPoint(0, 0)), w->geometry().size());
    QCursor::setPos(rect.center());
}

void MouseLocker::onDestroyed()
{
    mOldMouseTracking = false;
    mMonitoredWidget = 0;
}
