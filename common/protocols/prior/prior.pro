# -------------------------------------------------
# Project created by QtCreator 2010-10-15T18:06:13
# -------------------------------------------------
TARGET = prior
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}
DESTDIR = $$PWD/bin
TEMPLATE = lib
DEFINES += PRIOR_LIBRARY
INCLUDEPATH += src ../share
SOURCES += src/priordata.cpp \
    src/priorerrors.cpp \
    src/prior.cpp \
    src/prior001.cpp \
    src/prior002.cpp \
    src/prior008.cpp

HEADERS += src/priordata.h \
    src/priorexport.h \
    src/priorerrors.h \
    src/prior.h \
    src/prior001.h \
    src/prior002.h \
    src/prior008.h

OTHER_FILES += \
    priorexport.pri

include(../../factory/factory.pri)
include(../share/share.pri)

# Installation procedure

FILE_EXTENSION =
unix:FILE_EXTENSION = .so
win32:FILE_EXTENSION = .dll

package_prior.path =  /package/bin
package_prior.files = $${PWD}/bin/$${TARGET}$${FILE_EXTENSION}

INSTALLS += package_prior
