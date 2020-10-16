#include "DocWidgetMoveHandler.h"
#include "XMasterWindow.h"

#include <QtCore/QLinkedList>

#include <QtCore/QEvent>

#include <QApplication>
#include <QDesktopWidget>

struct CompareAndMove
{
    bool isIntersect;
    qreal distance;
    inline bool operator<(const CompareAndMove &other) const
    {
        return ((isIntersect && !other.isIntersect) || (isIntersect == other.isIntersect && distance < other.distance));
    }
};

DocWidgetMoveHandler::DocWidgetMoveHandler(XMasterWindow *parent) : QObject(parent)
        , master(parent)
{}

bool DocWidgetMoveHandler::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *sender = qobject_cast<QWidget *>(watched);
    if (!sender)
        return false;

    if (QEvent::Move == event->type())
    {
        sender->setProperty("isLastEventWasMove", true);
    }
    //else if (QEvent::CursorChange == event->type() && sender->property("isLastEventWasMove").toBool())
    else if (QEvent::MouseButtonRelease == event->type() && sender->property("isLastEventWasMove").toBool())
    {
        sender->setProperty("isLastEventWasMove", false);
        QMultiMap<CompareAndMove, QDockWidget *> relationMap;
        bool shouldMove = false;
        foreach (const QSharedPointer<QDockWidget> &doc, master->m_mapDoc)
        {
            if (doc.data() == sender)
                continue;

            CompareAndMove key;
            key.isIntersect = sender->frameGeometry().intersects(doc->frameGeometry());
            key.distance = QLineF(sender->frameGeometry().center(), doc->frameGeometry().center()).length();
            relationMap.insert(key, doc.data());
            shouldMove = shouldMove || key.isIntersect;
        }

        if (!shouldMove)
            return false;

        // try to move near each widget
        QList<QDockWidget *> relationList = relationMap.values();
        QRect senderGeom = sender->frameGeometry();
        foreach (QDockWidget *comparedWidget, relationList)
        {
            // try to move botLeft
            QRect newGeom = senderGeom;
            newGeom.moveTopLeft(comparedWidget->frameGeometry().bottomLeft());
            newGeom.moveTop(newGeom.top() + 1);
            if (tryToMove(relationList, newGeom, sender))
                return false;

            // try to move botRight
            newGeom.moveTopRight(comparedWidget->frameGeometry().bottomRight());
            newGeom.moveTop(newGeom.top() + 1);
            if (tryToMove(relationList, newGeom, sender))
                return false;

            // try to move topLeft
            newGeom.moveBottomLeft(comparedWidget->frameGeometry().topLeft());
            newGeom.moveBottom(newGeom.bottom() - 1);
            if (tryToMove(relationList, newGeom, sender))
                return false;

            // try to move topRight
            newGeom.moveBottomRight(comparedWidget->frameGeometry().topRight());
            newGeom.moveBottom(newGeom.bottom() - 1);
            if (tryToMove(relationList, newGeom, sender))
                return false;

            // try to move LeftTop
            newGeom.moveTopRight(comparedWidget->frameGeometry().topLeft());
            newGeom.moveRight(newGeom.right() - 1);
            if (tryToMove(relationList, newGeom, sender))
                return false;

            // try to move LeftBot
            newGeom.moveBottomRight(comparedWidget->frameGeometry().bottomLeft());
            newGeom.moveRight(newGeom.right() - 1);
            if (tryToMove(relationList, newGeom, sender))
                return false;

            // try to move RightTop
            newGeom.moveTopLeft(comparedWidget->frameGeometry().topRight());
            newGeom.moveLeft(newGeom.left() + 1);
            if (tryToMove(relationList, newGeom, sender))
                return false;

            // try to move RightBot
            newGeom.moveBottomLeft(comparedWidget->frameGeometry().bottomRight());
            newGeom.moveLeft(newGeom.left() + 1);
            if (tryToMove(relationList, newGeom, sender))
                return false;
        }
    }
    return false;
}

bool DocWidgetMoveHandler::canMove(const QList<QDockWidget *> &widgets, const QRect &geom)
{
    foreach (const QDockWidget *widget, widgets) {
        if (geom.intersects(widget->frameGeometry()))
            return false;
    }

    return QApplication::desktop()->availableGeometry().intersected(geom) == geom;
}

bool DocWidgetMoveHandler::tryToMove(const QList<QDockWidget *> &widgets, const QRect &geom, QWidget *sender)
{
    if (canMove(widgets, geom))
    {
        //sender->move(geom.topLeft());
        mSender = sender;
        mPoint = geom.topLeft();
        QTimer::singleShot(0, this, SLOT(OnTimerMove()));
        return true;
    }
    return false;
}

void DocWidgetMoveHandler::OnTimerMove()
{
    mSender->move(mPoint);
}
