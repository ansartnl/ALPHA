# -------------------------------------------------
# Project created by QtCreator 2010-10-07T15:55:09
# -------------------------------------------------
TEMPLATE = lib
TARGET = rewarn

QT += sql

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QMAKE_CXXFLAGS += -std=c++11
}

DESTDIR = $$PWD/bin

DEFINES += REWARN_LIBRARY
# Uncomment REWARN_TEST definition if build with the rewarntest test program
#DEFINES += REWARN_TEST

INCLUDEPATH += src

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}
unix:CONFIG += debug_and_release
CONFIG += thread

SOURCES += \
    src/WarnNotifier.cpp \
    src/orm/Area.cpp \
    src/orm/MeteoArea.cpp \
    src/orm/ReArea.cpp \
    src/orm/ReFir.cpp \
    src/helpers/DbReadThread.cpp \
    src/helpers/WorkerThread.cpp \
    src/helpers/NFZchecker.cpp \
    src/helpers/Misc.cpp

HEADERS += \
    src/rewarn_global.h \
    src/WarnNotifier.h \
    src/AirplaneInfo.h \
    src/orm/Area.h \
    src/orm/MeteoTypes.h \
    src/orm/MeteoArea.h \
    src/orm/ReTypes.h \
    src/orm/RePoint.h \
    src/orm/ReArea.h \
    src/orm/ReFir.h \
    src/helpers/DbReadThread.h \
    src/helpers/WorkerThread.h \
    src/helpers/NFZchecker.h \
    src/helpers/Misc.h

include(../../common/coordsys/coordsysexport.pri)
include(../../common/qtautotransaction/qtautotransaction.pri)
# Need qtlanguagemenu inclusion for 'lupdate' translation utility.
include(../../common/qtlanguagemenu/qtlanguagemenu.pri)
include(../../common/dbaccess/dbaccess.pri)
include(../../common/dbconnectionsmanager/dbconnectionsmanager.pri)

TRANSLATIONS = share/translations/rewarn_ru.ts

OTHER_FILES += \
    rewarnexport.pri

# Installation procedure

FILE_EXTENSION =
win32:FILE_EXTENSION = .dll
unix:FILE_EXTENSION = .so

package_rewarn.path = /package/bin
package_rewarn.files = $${PWD}/bin/$${TARGET}$${FILE_EXTENSION}

translations_rewarn.path = /package/share/translations
translations_rewarn.files = $${PWD}/share/translations/*_ru.qm

INSTALLS += package_rewarn translations_rewarn
