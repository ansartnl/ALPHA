# -------------------------------------------------
# Project created by QtCreator 2010-10-15T18:06:13
# -------------------------------------------------
TARGET = krams4
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}
DESTDIR = $$PWD/bin
TEMPLATE = lib
DEFINES += KRAMS_LIBRARY
INCLUDEPATH += src ../share
SOURCES += src/krams.cpp \
    src/kramserrors.cpp \
    src/kramsproto.cpp \
    src/kramstickets.cpp \
    src/kramsping.cpp \
    src/krams4.cpp \
    src/krams4dump.cpp \
    src/krams4metarspeci.cpp \
    src/MetarSpeci.cpp \
    src/MetarSpeciParser.cpp

HEADERS += src/krams.h \
    src/kramsexport.h \
    src/kramserrors.h \
    src/kramsproto.h \
    src/kramstickets.h \
    src/kramsping.h \
    src/krams4.h \
    src/krams4dump.h \
    src/krams4metarspeci.h \
    src/MetarSpeci.h \
    src/MetarSpeciParser.h

OTHER_FILES += \
    krams4export.pri

include(../../enumstreaming/enumstreaming.pri)

# Installation procedure

FILE_EXTENSION =
unix:FILE_EXTENSION = .so
win32:FILE_EXTENSION = .dll

package_krams4.path =  /package/bin
package_krams4.files = $${PWD}/bin/$${TARGET}$${FILE_EXTENSION}

INSTALLS += package_krams4
