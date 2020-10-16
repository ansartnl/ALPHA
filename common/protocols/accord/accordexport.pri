INCLUDEPATH   += $$PWD/src
DEPENDPATH    += $$PWD/src

HEADERS       += accordproto.h \
                accordexport.h \
                accorderrors.h \
                codogram0.h \
                codogram1.h \
                codogram2.h \
                codogram3.h \
                codogram4.h \
                codogram5.h \
                codogram6to9.h \
                codogram10.h \
                codogram11.h

LIBS += -L$$PWD/bin
CONFIG(debug, debug|release) : LIBS += -laccord_debug
CONFIG(release, release|debug) : LIBS += -laccord
