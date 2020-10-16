# -------------------------------------------------
# Project created by QtCreator 2010-10-08T09:36:48
# -------------------------------------------------
TEMPLATE = lib
TARGET = CoordSys

DEFINES += COORDSYS_LIBRARY

INCLUDEPATH += src

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}
unix:CONFIG += debug_and_release

greaterThan(QT_MAJOR_VERSION, 4) {
    QMAKE_CXXFLAGS += -std=c++11
}

DESTDIR = $$PWD/bin

SOURCES += \
    src/cs/CoordSystem.cpp \
    src/cs/CoordSystemGK.cpp \
    src/cs/CoordSystemStereo.cpp \
    src/cs/Coordinate.cpp
	
HEADERS += \
    src/coordsys_global.h \
    src/cs/GeoCS.h \
    src/cs/GaussKrugerCS.h \
    src/cs/StereoCS.h \
    src/cs/CoordSystem.h \
    src/cs/CoordSystemGK.h \
    src/cs/CoordSystemStereo.h \
    src/cs/Coordinate.h

OTHER_FILES += \
    coordsysexport.pri

TRANSLATIONS = share/translations/coordsys_ru.ts

# Installation procedure

FILE_EXTENSION =
unix:FILE_EXTENSION = .so
win32:FILE_EXTENSION = .dll

package_cs.path =   /package/bin
package_cs.files =  $${PWD}/bin/$${TARGET}$${FILE_EXTENSION}

translations_cs.path = /package/share/translations
translations_cs.files = $${PWD}/share/translations/*_ru.qm

INSTALLS += package_cs translations_cs
