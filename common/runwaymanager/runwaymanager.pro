TARGET = runwaymanager

DESTDIR = $$PWD/bin

HEADERS       = $$PWD/src/server.h \
    src/Runway.h \
    src/main.h
SOURCES       = $$PWD/src/server.cpp \
                $$PWD/src/main.cpp \
    src/Runway.cpp
QT           += network

CONFIG(debug, debug|release) : TARGET = $$join(TARGET,,,_debug)

include(../initmain/initmain.pri)
include(../qtxmlsettings/qtxmlsettings.pri)
include(../qttcpdatagramprotocolvariant/qttcpdatagramprotocolvariant.pri)
