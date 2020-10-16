# -------------------------------------------------
# Project created by QtCreator 2010-12-08T14:11:11
# -------------------------------------------------

INCLUDEPATH   += $$PWD/src
DEPENDPATH    += $$PWD/src

HEADERS       += \
                $$PWD/src/xtproto_global.h \
                $$PWD/src/XtAirplaneInfo.h \
                $$PWD/src/XtAsterix62.h \
                $$PWD/src/XtCommon.h \
                $$PWD/src/XtDataMessage.h \
                $$PWD/src/XtDefines.h \
                $$PWD/src/XtGlobal.h \
                $$PWD/src/XtMasterServer.h \
                $$PWD/src/XtMessage.h \
                $$PWD/src/XtMeteo.h \
                $$PWD/src/XtMRequestAll.h \
                $$PWD/src/XtMRequestMaster.h \
                $$PWD/src/XtMRequestType.h \
                $$PWD/src/XtMSubscribeType.h \
                $$PWD/src/XtObject.h \
                $$PWD/src/XtPeleng.h \
                $$PWD/src/XtRadar.h \
                $$PWD/src/XtPlotTrack.h \
                $$PWD/src/XtProtocol.h \
                $$PWD/src/XtRequestMessage.h \
                $$PWD/src/XtSubscribeMessage.h \
                $$PWD/src/XtTypes.h \
                $$PWD/src/XtStand.h \
                $$PWD/src/XtSimCmd.h

LIBS += -L$$PWD/bin
CONFIG(debug, debug|release) : LIBS += -lXtProto_debug
CONFIG(release, release|debug) : LIBS += -lXtProto 
