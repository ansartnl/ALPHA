QT += network

INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

HEADERS       += $$PWD/qtvariantconnection.h
SOURCES       += $$PWD/qtvariantconnection.cpp

include($$PWD/../qttcpdatagramprotocol/qttcpdatagramprotocol.pri)