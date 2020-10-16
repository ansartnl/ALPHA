INCLUDEPATH   += $$PWD/src
DEPENDPATH    += $$PWD/src

HEADERS       += priordata.h \
                priorexport.h \
                priorerrors.h \
                prior.h \
                prior001.h \
                prior002.h \
                prior008.h

LIBS += -L$$PWD/bin
CONFIG(debug, debug|release) : LIBS += -lprior_debug
CONFIG(release, release|debug) : LIBS += -lprior

include($$PWD/../../factory/factory.pri)
