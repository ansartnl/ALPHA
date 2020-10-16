QT += network

INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

HEADERS       += $$PWD/qttcpdatagramprotocolvariant.h
SOURCES       += $$PWD/qttcpdatagramprotocolvariant.cpp

include($$PWD/../qttcpdatagramprotocol/qttcpdatagramprotocol.pri)
