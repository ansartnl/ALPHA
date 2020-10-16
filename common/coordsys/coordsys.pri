INCLUDEPATH   += $$PWD/src
DEPENDPATH    += $$PWD/src

DEFINES += COORDSYS_LIBRARY

HEADERS += \
    $$PWD/src/coordsys_global.h \
    $$PWD/src/cs/Coordinate.h

SOURCES += \
   $$PWD/src/cs/Coordinate.cpp
