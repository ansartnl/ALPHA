QT += network

INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

HEADERS       += $$PWD/qtudpsocket.h
SOURCES       += $$PWD/qtudpsocket.cpp

include($$PWD/../broadcast/broadcast.pri)