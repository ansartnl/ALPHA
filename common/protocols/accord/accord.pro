# -------------------------------------------------
# Project created by QtCreator 2010-10-15T18:06:13
# -------------------------------------------------
TARGET = accord
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}
DESTDIR = $$PWD/bin
TEMPLATE = lib
DEFINES += ACCORD_LIBRARY
SOURCES += src/accordproto.cpp \
    src/accorderrors.cpp \
    src/codogramplane.cpp \
    src/codogram0.cpp \
    src/codogram1.cpp \
    src/codogram2.cpp \
    src/codogram3.cpp \
    src/codogram4.cpp \
    src/codogram5.cpp \
    src/codogram6to9.cpp \
    src/codogram10.cpp \
    src/codogram11.cpp \
    src/accord.cpp

HEADERS += src/accordproto.h \
    src/accordexport.h \
    src/accordutility.h \
    src/accorderrors.h \
    src/codogramplane.h \
    src/codogram0.h \
    src/codogram1.h \
    src/codogram2.h \
    src/codogram3.h \
    src/codogram4.h \
    src/codogram5.h \
    src/codogram6to9.h \
    src/codogram10.h \
    src/codogram11.h \
    src/accord.h
    
OTHER_FILES += \
    accordexport.pri

# Installation procedure

FILE_EXTENSION =
unix:FILE_EXTENSION = .so
win32:FILE_EXTENSION = .dll

package_accord.path = /package/bin
package_accord.files = $${PWD}/bin/$${TARGET}$${FILE_EXTENSION}

INSTALLS += package_accord
