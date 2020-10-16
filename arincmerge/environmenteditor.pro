# -------------------------------------------------
# Project created by QtCreator 2010-12-28T16:19:57
# -------------------------------------------------
TARGET = environmenteditor
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}
DESTDIR = $$PWD/bin
TEMPLATE = app
INCLUDEPATH += src \
    src/arinc \
    src/arinc/iterator \
    src/arinc/parser \
    src/arinc/structures \
    src/arinc/storage \
    src/editors \
    src/database \
    src/common \
    src/dialogs
QT += sql \
    network \
    xml

QMAKE_CXXFLAGS += -pedantic -Wall -Wno-long-long

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QMAKE_CXXFLAGS += -std=c++11
    DEFINES += "QT_DISABLE_DEPRECATED_BEFORE=0x000000"
}

SOURCES += src/main.cpp \
    src/arinc/parser/ArincParser.cpp \
    src/arinc/storage/ArincEmptyStorage.cpp \
    src/arinc/storage/ArincNativeStorage.cpp \
    src/arinc/structures/BoundaryPoint.cpp \
    src/arinc/structures/FirUir.cpp \
    src/arinc/structures/UpperLowerLimit.cpp \
    src/arinc/structures/ControlledAirspace.cpp \
    src/arinc/structures/ContinuantionNumber.cpp \
    src/arinc/structures/RestrictedAirspace.cpp \
    src/arinc/structures/Airspace.cpp \
    src/arinc/structures/VhfNavaid.cpp \
    src/arinc/structures/NdbNavaid.cpp \
    src/arinc/structures/Navaid.cpp \
    src/arinc/structures/Enroute.cpp \
    src/arinc/structures/Airway.cpp \
    src/arinc/structures/Holding.cpp \
    src/arinc/structures/Airport.cpp \
    src/arinc/structures/AirportRefPoint.cpp \
    src/arinc/iterator/ArincFilePrimitiveIterator.cpp \
    src/arinc/iterator/ArincFilePosIterator.cpp \
    src/arinc/structures/EnrouteWaypoint.cpp \
    src/arinc/structures/Waypoint.cpp \
    src/arinc/structures/TerminalWaypoint.cpp \
    src/arinc/structures/BoundaryPointFirUir.cpp \
    src/arinc/structures/SSA.cpp \
    src/arinc/structures/ArincBaseStruct.cpp \
    src/arinc/structures/ArincGlobal.cpp \
    src/arinc/structures/Runway.cpp \
    src/arinc/structures/Ndb.cpp \
    src/arinc/structures/NdbTerminal.cpp \
    src/arinc/parser/ArincParserDialog.cpp \
    src/arinc/parser/ArincParserThread.cpp \
    src/arinc/storage/ArincStatusStorage.cpp \
    src/arincmergemainwindow.cpp \
    src/editors/viewdelegate.cpp \
    src/editors/ssaeditor.cpp \
    src/editors/ssamodel.cpp \
    src/editors/routeeditor.cpp \
    src/editors/restricteditor.cpp \
    src/editors/restrictusereditor.cpp \
    src/editors/pointeditor.cpp \
    src/editors/holdingeditor.cpp \
    src/editors/fireditor.cpp \
    src/editors/dbscheme.cpp \
    src/editors/airporteditor.cpp \
    src/editors/sectoreditor.cpp \
    src/editors/baseeditor.cpp \
    src/dialogs/importarincdialog.cpp \
    src/database/ArincDatabaseImporter.cpp \
    src/database/ArincDatabaseImporterDialog.cpp \
    src/database/ArincDatabaseImporterDC.cpp \
    src/database/FirBoundaryReplacer.cpp \
    src/database/OldMasterSupport.cpp \
    src/common/widgetsaver.cpp \
    src/common/DeferredCallThread.cpp \
    src/common/misc.cpp \
    src/dialogs/acdialog.cpp \
    src/dialogs/synchronizationdialog.cpp \
    src/database/synchronizer.cpp \
    src/dialogs/selectfirdialog.cpp

HEADERS += \
    src/arinc/parser/ArincParser.h \
    src/arinc/storage/ArincEmptyStorage.h \
    src/arinc/storage/ArincNativeStorage.h \
    src/arinc/structures/BoundaryPoint.h \
    src/arinc/structures/FirUir.h \
    src/arinc/structures/UpperLowerLimit.h \
    src/arinc/structures/ControlledAirspace.h \
    src/arinc/structures/ContinuantionNumber.h \
    src/arinc/structures/RestrictedAirspace.h \
    src/arinc/structures/Airspace.h \
    src/arinc/structures/VhfNavaid.h \
    src/arinc/structures/NdbNavaid.h \
    src/arinc/structures/Navaid.h \
    src/arinc/structures/Enroute.h \
    src/arinc/structures/Airway.h \
    src/arinc/structures/Holding.h \
    src/arinc/structures/Airport.h \
    src/arinc/structures/AirportRefPoint.h \
    src/arinc/iterator/ArincFilePrimitiveIterator.h \
    src/arinc/iterator/ArincFilePosIterator.h \
    src/arinc/structures/EnrouteWaypoint.h \
    src/arinc/structures/Waypoint.h \
    src/arinc/structures/TerminalWaypoint.h \
    src/arinc/structures/BoundaryPointFirUir.h \
    src/arinc/structures/SSA.h \
    src/arinc/structures/ArincBaseStruct.h \
    src/arinc/structures/ArincGlobal.h \
    src/arinc/structures/Runway.h \
    src/arinc/structures/Ndb.h \
    src/arinc/structures/NdbTerminal.h \
    src/arinc/parser/ArincParserDialog.h \
    src/arinc/parser/ArincParserThread.h \
    src/arinc/storage/ArincStatusStorage.h \
    src/arincmergemainwindow.h \
    src/editors/viewdelegate.h \
    src/editors/ssaeditor.h \
    src/editors/ssamodel.h \
    src/editors/routeeditor.h \
    src/editors/restricteditor.h \
    src/editors/restrictusereditor.h \
    src/editors/pointeditor.h \
    src/editors/holdingeditor.h \
    src/editors/fireditor.h \
    src/editors/editors.h \
    src/editors/dbscheme.h \
    src/editors/airporteditor.h \
    src/editors/RefreshInterface.h \
    src/editors/sectoreditor.h \
    src/editors/baseeditor.h \
    src/dialogs/importarincdialog.h \
    src/database/ArincDatabaseImporter.h \
    src/database/ArincDatabaseImporterDialog.h \
    src/database/ArincDatabaseImporterDC.h \
    src/database/FirBoundaryReplacer.h \
    src/database/OldMasterSupport.h \
    src/common/widgetsaver.h \
    src/common/DeferredProcedure.h \
    src/common/DeferredCallThread.h \
    src/common/misc.h \
    src/dialogs/acdialog.h \
    src/database/synchronizer.h \
    src/dialogs/synchronizationdialog.h \
    src/main.h \
    src/dialogs/selectfirdialog.h

include(../common/qtlanguagemenu/qtlanguagemenu.pri)
include(../common/qtxmlsettings/qtxmlsettings.pri)
include(../common/qtautotransaction/qtautotransaction.pri)
include(../common/initmain/initmain.pri)
include(../common/geo/geo.pri)
include(../common/waitcursor/waitcursor.pri)
include(../common/qtdelegates/qtdelegates.pri)
include(../common/version/version.pri)
include(../common/dbaccess/dbaccess.pri)

FORMS += \
    src/arinc/parser/ArincParserDialog.ui \
    src/arincmergemainwindow.ui \
    src/editors/ssaeditor.ui \
    src/editors/routeeditor.ui \
    src/editors/restricteditor.ui \
    src/editors/restrictusereditor.ui \
    src/editors/pointeditor.ui \
    src/editors/holdingeditor.ui \
    src/editors/fireditor.ui \
    src/editors/airporteditor.ui \
    src/dialogs/importarincdialog.ui \
    src/database/ArincDatabaseImporterDialog.ui \
    src/editors/sectoreditor.ui \
    src/dialogs/acdialog.ui \
    src/dialogs/synchronizationdialog.ui \
    src/dialogs/selectfirdialog.ui

TRANSLATIONS = share/translations/environmenteditor_ru.ts
