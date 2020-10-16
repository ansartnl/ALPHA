TEMPLATE = lib
TARGET   = izmqt
CONFIG  += shared

win32:DESTDIR = bin/win32-mingw
win32-msvc2005:DESTDIR = bin/win32-msvc
linux-g++-32:DESTDIR = lib/linux-i686
linux-g++-64:DESTDIR = lib/linux-x64

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}

include(izmqt.pri)
