/****************************************************************************
 **
 ** Copyright (C) Qxt Foundation. Some rights reserved.
 **
 ** This file is part of the QxtGui module of the Qxt library.
 **
 ** This library is free software; you can redistribute it and/or modify it
 ** under the terms of the Common Public License, version 1.0, as published
 ** by IBM, and/or under the terms of the GNU Lesser General Public License,
 ** version 2.1, as published by the Free Software Foundation.
 **
 ** This file is provided "AS IS", without WARRANTIES OR CONDITIONS OF ANY
 ** KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 ** WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY OR
 ** FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** You should have received a copy of the CPL and the LGPL along with this
 ** file. See the LICENSE file and the cpl1.0.txt/lgpl-2.1.txt files
 ** included with the source distribution for more information.
 ** If you did not receive a copy of the licenses, contact the Qxt Foundation.
 **
 ** <http://libqxt.org>  <foundation@libqxt.org>
 **
 ****************************************************************************/
#include "qxtwindowsystem.h"
#include <QLibrary>
#include <QX11Info>
#include <X11/Xutil.h>

static WindowList qxt_getWindows(Atom prop)
{
    WindowList res;
    Atom type = 0;
    int format = 0;
    uchar* data = 0;
    ulong count, after;
    Display* display = QX11Info::display();
    Window window = QX11Info::appRootWindow();
    if (XGetWindowProperty(display, window, prop, 0, 1024 * sizeof(Window) / 4, False, AnyPropertyType,
                           &type, &format, &count, &after, &data) == Success)
    {
        Window* list = reinterpret_cast<Window*>(data);
        for (uint i = 0; i < count; ++i)
            res += list[i];
        if (data)
            XFree(data);
    }
    return res;
}

WindowList QxtWindowSystem::windows()
{
    static Atom net_clients = 0;
    if (!net_clients)
        net_clients = XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", True);

    return qxt_getWindows(net_clients);
}

WId QxtWindowSystem::activeWindow()
{
    static Atom net_active = 0;
    if (!net_active)
        net_active = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", True);

    return qxt_getWindows(net_active).value(0);
}

WId QxtWindowSystem::findWindow(const QString& title)
{
    Window result = 0;
    WindowList list = windows();
    foreach (const Window &wid, list)
    {
        if (windowTitle(wid) == title)
        {
            result = wid;
            break;
        }
    }
    return result;
}

WId QxtWindowSystem::windowAt(const QPoint& pos)
{
    Window result = 0;
    WindowList list = windows();
    for (int i = list.size() - 1; i >= 0; --i)
    {
        WId wid = list.at(i);
        if (windowGeometry(wid).contains(pos))
        {
            result = wid;
            break;
        }
    }
    return result;
}

QString QxtWindowSystem::windowTitle(WId window)
{
    QString name;
    char* str = 0;
    if (XFetchName(QX11Info::display(), window, &str))
        name = QString::fromLatin1(str);
    if (str)
        XFree(str);
    return name;
}

QRect QxtWindowSystem::windowGeometry(WId window)
{
    int x, y;
    uint width, height, border, depth;
    Window root, child;
    Display* display = QX11Info::display();
    XGetGeometry(display, window, &root, &x, &y, &width, &height, &border, &depth);
    XTranslateCoordinates(display, window, root, x, y, &x, &y, &child);

    static Atom net_frame = 0;
    if (!net_frame)
        net_frame = XInternAtom(QX11Info::display(), "_NET_FRAME_EXTENTS", True);

    QRect rect(x, y, width, height);
    Atom type = 0;
    int format = 0;
    uchar* data = 0;
    ulong count, after;
    if (XGetWindowProperty(display, window, net_frame, 0, 4, False, AnyPropertyType,
                           &type, &format, &count, &after, &data) == Success)
    {
        // _NET_FRAME_EXTENTS, left, right, top, bottom, CARDINAL[4]/32
        if (count == 4)
        {
            long* extents = reinterpret_cast<long*>(data);
            rect.adjust(-extents[0], -extents[2], extents[1], extents[3]);
        }
        if (data)
            XFree(data);
    }
    return rect;
}

typedef struct {
    Window  window;     /* screen saver window - may not exist */
    int     state;      /* ScreenSaverOff, ScreenSaverOn, ScreenSaverDisabled*/
    int     kind;       /* ScreenSaverBlanked, ...Internal, ...External */
    unsigned long    til_or_since;   /* time til or since screen saver */
    unsigned long    idle;      /* total time since last user input */
    unsigned long   eventMask; /* currently selected events for this client */
} XScreenSaverInfo;

typedef XScreenSaverInfo* (*XScreenSaverAllocInfo)();
typedef Status (*XScreenSaverQueryInfo)(Display* display, Drawable* drawable, XScreenSaverInfo* info);

static XScreenSaverAllocInfo _xScreenSaverAllocInfo = 0;
static XScreenSaverQueryInfo _xScreenSaverQueryInfo = 0;

uint QxtWindowSystem::idleTime()
{
    static bool xssResolved = false;
    if (!xssResolved) {
        QLibrary xssLib(QLatin1String("Xss"), 1);
        if (xssLib.load()) {
            _xScreenSaverAllocInfo = (XScreenSaverAllocInfo) xssLib.resolve("XScreenSaverAllocInfo");
            _xScreenSaverQueryInfo = (XScreenSaverQueryInfo) xssLib.resolve("XScreenSaverQueryInfo");
            xssResolved = true;
        }
    }

    uint idle = 0;
    if (xssResolved)
    {
        XScreenSaverInfo* info = _xScreenSaverAllocInfo();
        const int screen = QX11Info::appScreen();
        Qt::HANDLE rootWindow = QX11Info::appRootWindow(screen);
        _xScreenSaverQueryInfo(QX11Info::display(), (Drawable*) rootWindow, info);
        idle = info->idle;
        if (info)
            XFree(info);
    }
    return idle;
}
