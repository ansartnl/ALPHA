INCLUDEPATH   += $$PWD/src
DEPENDPATH    += $$PWD/src

LIBS += -L$$PWD/bin
CONFIG(debug, debug|release) : LIBS += -lrewarn_debug
CONFIG(release, release|debug) : LIBS += -lrewarn

include(../../common/coordsys/coordsysexport.pri)
