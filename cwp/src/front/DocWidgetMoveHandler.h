#ifndef DOCWIDGETMOVEHANDLER_H
#define DOCWIDGETMOVEHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QRect>

class XMasterWindow;
class XDocWidget;
class QDockWidget;

class DocWidgetMoveHandler : public QObject
{
    Q_OBJECT
public:
    explicit DocWidgetMoveHandler(XMasterWindow *parent = 0);
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void OnTimerMove();

private:
    bool canMove(const QList<QDockWidget *> &widgets, const QRect &geom);
    bool tryToMove(const QList<QDockWidget *> &widgets, const QRect &geom, QWidget *sender);
    XMasterWindow   *master;
    QWidget         *mSender;
    QPoint          mPoint;
};

#endif // DOCWIDGETMOVEHANDLER_H
