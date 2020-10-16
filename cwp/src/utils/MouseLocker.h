#ifndef MOUSELOCKER_H
#define MOUSELOCKER_H

#include <QObject>

class MouseLocker : public QObject
{
    Q_OBJECT
public:
    explicit MouseLocker(QObject *parent = 0);

    void lockInside(QWidget *w);

protected:
    bool eventFilter(QObject *obj, QEvent *e);

private:
    void centerMouse(QWidget *w);

private slots:
    void onDestroyed();

private:
    QWidget *mMonitoredWidget;
    bool mOldMouseTracking;
    bool mActivated;
};

MouseLocker * mouseLocker(QObject *parent = 0);

#endif // MOUSELOCKER_H
