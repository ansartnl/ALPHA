INCLUDEPATH   += $$PWD/src
DEPENDPATH    += $$PWD/src

HEADERS += \
    $$PWD/src/coordsys_global.h \
    $$PWD/src/cs/CoordSystemGK.h \
    $$PWD/src/cs/CoordSystemStereo.h \
#    $$PWD/src/cs/CoordSystemProj.h \
    $$PWD/src/cs/Coordinate.h

LIBS += -L$$PWD/bin
CONFIG(debug, debug|release) : LIBS += -lCoordSys_debug
CONFIG(release, release|debug) : LIBS += -lCoordSys
