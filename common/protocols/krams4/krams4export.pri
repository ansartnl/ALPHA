INCLUDEPATH   += $$PWD/src
DEPENDPATH    += $$PWD/src

HEADERS       += kramsproto.h \
                kramsexport.h \
                kramserrors.h \
                krams.h \
                krams4.h \
                krams4dump.h \
                krams4metarspeci.h \
                kramsping.h \
                kramstickets.h \
                MetarSpeci.h \
                MetarSpeciParser.h

LIBS += -L$$PWD/bin
CONFIG(debug, debug|release) : LIBS += -lkrams4_debug
CONFIG(release, release|debug) : LIBS += -lkrams4

include(../../enumstreaming/enumstreaming.pri)
