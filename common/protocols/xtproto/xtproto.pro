# -------------------------------------------------
# Project created by QtCreator 2010-12-08T14:11:11
# -------------------------------------------------

QT -= gui

TEMPLATE = lib
TARGET = XtProto

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}
unix:CONFIG += debug_and_release
DESTDIR = $$PWD/bin

INCLUDEPATH += src

DEFINES += XTPROTO_LIBRARY

SOURCES += \
    src/XtMessageImpl.cpp \
    src/XtObjectImpl.cpp \
    src/XtCommon.cpp \
    src/XtProtocol.cpp \
    src/XtAsterix62.cpp \
    src/XtPlotTrack.cpp \
    src/XtObject.cpp \
    src/XtPeleng.cpp \
    src/XtRadar.cpp \
    src/XtMeteo.cpp \
    src/XtAirplaneInfo.cpp \
    src/XtStand.cpp \
    src/XtSimCmd.cpp \
    src/XtVehicle.cpp

HEADERS += \
    src/XtAsterix62.h \
    src/XtAirplaneInfo.h \
    src/XtMeteo.h \
    src/XtPeleng.h \
    src/XtRadar.h \
    src/XtPlotTrack.h \
    src/XtDataMessage.h \
    src/XtDefines.h \
    src/XtGlobal.h \
    src/XtCommon.h \
    src/XtMasterServer.h \
    src/XtMessage.h \
    src/XtMRequestAll.h \
    src/XtMRequestMaster.h \
    src/XtMRequestType.h \
    src/XtMSubscribeType.h \
    src/XtObject.h \
    src/XtProtocol.h \
    src/XtRequestMessage.h \
    src/XtSubscribeMessage.h \
    src/XtTypes.h \
    src/XtStand.h \
    src/XtSimCmd.h \
    src/XtVehicle.h

#OTHER_FILES += \
#    xtprotoexport.pri

# Installation procedure

FILE_EXTENSION =
unix:FILE_EXTENSION = .so
win32:FILE_EXTENSION = .dll

package_xtproto.path =  /package/bin
package_xtproto.files = $${PWD}/bin/$${TARGET}$${FILE_EXTENSION}

INSTALLS += package_xtproto
