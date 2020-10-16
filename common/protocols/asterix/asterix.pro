# -------------------------------------------------
# Project created by QtCreator 2010-10-15T18:06:13
# -------------------------------------------------
TARGET = asterix
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QMAKE_CXXFLAGS += -std=c++11
}

DESTDIR = $$PWD/bin
TEMPLATE = lib
DEFINES += ASTERIX_LIBRARY
INCLUDEPATH += src
SOURCES += src/asterixdata.cpp \
    src/asterixerrors.cpp \
    src/asterix.cpp \
    src/asterix001.cpp \
    src/asterix002.cpp \
    src/asterix004.cpp \
    src/asterix008.cpp \
    src/asterix020.cpp \
    src/asterix021.cpp \
    src/asterix034.cpp \
    src/asterix048.cpp \
    src/asterix062.cpp \
    src/asterix063.cpp \
# Eurocontrol standard
    src/eurocontrol/asterix020euro.cpp \
    src/eurocontrol/asterix253euro.cpp \
# Aerotechnica standard
    src/aerotechnica/asterix001aero.cpp \
    src/aerotechnica/asterix002aero.cpp \
    src/aerotechnica/asterix062aero.cpp \
    src/aerotechnica/asterix250aero.cpp \
    src/aerotechnica/asterix253aero.cpp \
    src/aerotechnica/asterix254aero.cpp \
# Peleng standard
    src/peleng/asterix062peleng.cpp \
    src/peleng/asterix241peleng.cpp \
    src/peleng/asterix253peleng.cpp

HEADERS += src/asterixdata.h \
    src/asterixexport.h \
    src/asterixerrors.h \
    src/asterix.h \
    src/asterixvariety.h \
    src/asterix001.h \
    src/asterix002.h \
    src/asterix004.h \
    src/asterix008.h \
    src/asterix020.h \
    src/asterix021.h \
    src/asterix034.h \
    src/asterix048.h \
    src/asterix062.h \
    src/asterix063.h \
# Eurocontrol standard
    src/eurocontrol/asterix001euro.h \
    src/eurocontrol/asterix002euro.h \
    src/eurocontrol/asterix004euro.h \
    src/eurocontrol/asterix008euro.h \
    src/eurocontrol/asterix020euro.h \
    src/eurocontrol/asterix021euro.h \
    src/eurocontrol/asterix034euro.h \
    src/eurocontrol/asterix048euro.h \
    src/eurocontrol/asterix062euro.h \
    src/eurocontrol/asterix063euro.h \
    src/eurocontrol/asterix253euro.h \
# Aerotechnica standard
    src/aerotechnica/asterix001aero.h \
    src/aerotechnica/asterix002aero.h \
    src/aerotechnica/asterix062aero.h \
    src/aerotechnica/asterix250aero.h \
    src/aerotechnica/asterix253aero.h \
    src/aerotechnica/asterix254aero.h \
# Peleng standard
    src/peleng/asterix062peleng.h \
    src/peleng/asterix241peleng.h \
    src/peleng/asterix253peleng.h

OTHER_FILES += \
    asterixexport.pri

include(../../factory/factory.pri)
include(../share/share.pri)

# Installation procedure

FILE_EXTENSION =
unix:FILE_EXTENSION = .so
win32:FILE_EXTENSION = .dll

package_asterix.path = /package/bin
package_asterix.files = $${PWD}/bin/$${TARGET}$${FILE_EXTENSION}

INSTALLS += package_asterix
