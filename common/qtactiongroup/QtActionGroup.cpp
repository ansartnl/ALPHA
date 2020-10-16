#include "QtActionGroup.h"


QtActionGroup::QtActionGroup(QObject* parent)
    : QActionGroup(parent), mIsLiteExclusive(false)
{
}

bool QtActionGroup::isLiteExclusive() const
{
    return mIsLiteExclusive;
}

void QtActionGroup::setLiteExclusive(bool on)
{
    mIsLiteExclusive = on;

    if ( on )
    {
        setExclusive(false);
        connect(this, SIGNAL(triggered(QAction *)), SLOT(on_triggered(QAction *)));
    }
    else
        disconnect(this, SLOT(on_triggered(QAction *)));
}

void QtActionGroup::on_triggered(QAction *action)
{
    foreach ( QAction *actionCurrent, actions() )
    {
        if ( action != actionCurrent && actionCurrent->isChecked() )
        {
            actionCurrent->blockSignals(true);
            actionCurrent->setChecked(false);
            actionCurrent->blockSignals(false);
        }
    }
}
