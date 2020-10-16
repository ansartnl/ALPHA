#ifndef _WAIT_CURSOR_H__
#define _WAIT_CURSOR_H__

#include <QtCore/QCoreApplication>


//! Application wait cursor wrapper.
class WaitCursor
{
public:
    WaitCursor();
    ~WaitCursor();

    //! Restore override cursor.
    void restore();
    //! Start waiting again.
    void wait();

private:
    /** True when wait cursor is active */
    bool    mIsWaited;
};

#endif // _WAIT_CURSOR_H__
