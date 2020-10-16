#-------------------------------------------------
#
# Project created by QtCreator 2011-01-24T15:31:25
#
#-------------------------------------------------
TARGET = snet
CONFIG(debug, debug|release) : TARGET = $$join(TARGET,,,_debug)

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QMAKE_CXXFLAGS += -std=c++11
}

DESTDIR = $$PWD/bin

TEMPLATE = app

QT       += core network sql

CONFIG   += console thread
CONFIG   -= app_bundle

INCLUDEPATH += src export

SOURCES += src/main.cpp \
    src/Analyzer.cpp \
    src/AirplaneThread.cpp \
    src/Airplane.cpp \
    export/StcaObject.cpp \
    export/AirplaneCollision.cpp \
    export/MeteoWarning.cpp \
    export/RestrictionWarning.cpp \
    src/StcaService.cpp \
    src/RedisDBHelper.cpp

HEADERS += src/Analyzer.h \
    src/AirplaneThread.h \
    src/Airplane.h \
    export/StcaObject.h \
    export/AirplaneCollision.h \
    export/MeteoWarning.h \
    export/RestrictionWarning.h \
    src/StcaService.h \
    src/RedisDBHelper.h

include(../../common/qtxmlsettings/qtxmlsettings.pri)
include(../../common/qtlanguagemenu/qtlanguagemenu.pri)
include(../../common/asterix/asterix.pri)
include(../../common/geo/geo.pri)
include(../../common/log4qt/src/Log4Qt.pri)
include(../../3rdparty/qtservice/src/qtservice.pri)
include(../../common/initmain/initmain.pri)
include(../../common/qtudpsocket/qtudpsocket.pri)
include(../../common/version/version.pri)
include(../rewarn/rewarnexport.pri)
include(../../common/qtdbautoconnector/qtdbautoconnector.pri)
include(../../3rdparty/redis/QtRedis.pri)
include(../../common/qjson/qjson.pri)
include(../../common/redisconnectionsmanager/redisconnectionsmanager.pri)

TRANSLATIONS = share/translations/stca_ru.ts

# Installation procedure

FILE_EXTENSION =
win32:FILE_EXTENSION = .exe

CONFIG(debug, debug|release):CONFIGURATION=debug
CONFIG(release, debug|release):CONFIGURATION=release

package.path = /package/bin
package.files = $${PWD}/bin/$${TARGET}$${FILE_EXTENSION}

translations.path = /package/share/translations
translations.files = $${PWD}/share/translations/*_ru.qm

config.path = /package/etc
config.files = $${PWD}/etc/config.xml

INSTALLS += package translations config
