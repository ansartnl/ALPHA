#include "waitcursor.h"

#include <QApplication>

WaitCursor::WaitCursor()
    : mIsWaited(false)
{
    wait();
}

WaitCursor::~WaitCursor()
{
    restore();
}

void WaitCursor::restore()
{
    if ( !mIsWaited )
        return;

    QApplication::restoreOverrideCursor();
    mIsWaited = false;
}

void WaitCursor::wait()
{
    if ( mIsWaited )
        return;

    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    mIsWaited = true;
}
