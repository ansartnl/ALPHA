cmake_minimum_required(VERSION 2.6)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# ------------------------------------------------------------
# CMake setup
# ------------------------------------------------------------

# Declare project
project(cwp)

set(CMAKE_INCLUDE_CURRENT_DIR true)

#set(CMAKE_BUILD_TYPE debug)
# -Wno-variadic-macros - disables warnings for variadic macros with GCC >= 4.0.0.
# Earlier versions warn because of anonymous variadic macros in pedantic mode
# but do not have a flag to disable these warnings.
if (MSVC)
#	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " /MP /W3 /WX /wd4512 /wd4127 /wd4718")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " /Zc:wchar_t- /MP /W3 /WX /wd4512 /wd4127 /wd4718")
    SET(QT_USE_QTMAIN TRUE)

    set(BIN_INSTALLATION_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "path to install binaries" FORCE)
elseif (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Wno-variadic-macros -Wno-literal-suffix")
else ()
    message ("Unknown compiler")
endif()

if(WIN32)
    set(POPPLER_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/poppler-0.26.4/bin CACHE PATH "path to external libraries" FORCE)
else(WIN32)

endif(WIN32)


IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

include(../common/cmake/version-dep-commands.cmake)
find_qt(Concurrent DBus Network PrintSupport Svg Sql Widgets)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x000000)

if(USE_QT5)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(USE_QT5)

# -----------------------------------------------------

# 3rd party
# -----------------------------------------------------

if(USE_QT5)
    include_directories(../3rdparty/poppler-0.26.4/qt5/src)
else(USE_QT5)
    include_directories(../3rdparty/poppler-0.26.4/qt4/src)
endif(USE_QT5)


add_subdirectory(../common/asterix ${CMAKE_CURRENT_BINARY_DIR}/asterix)
include_directories(
    ../common/asterix/src
    ../common/asterix/common/factory
    )


add_subdirectory(../common/qtxmlsettings ${CMAKE_CURRENT_BINARY_DIR}/qtxmlsettings)
include_directories(../common/qtxmlsettings)

add_subdirectory(../common/qtautotransaction ${CMAKE_CURRENT_BINARY_DIR}/qtautotransaction)
include_directories(../common/qtautotransaction)

add_subdirectory(../common/qtlanguagemenu ${CMAKE_CURRENT_BINARY_DIR}/qtlanguagemenu)
include_directories(../common/qtlanguagemenu)

set(LOG4QT_BUILD_STATIC TRUE)
set(LOG4QT_DONT_INSTALL TRUE)
add_subdirectory(../common/log4qt ${CMAKE_CURRENT_BINARY_DIR}/log4qt)
include_directories(../common/log4qt/src)

add_subdirectory(../common/echelonswidget ${CMAKE_CURRENT_BINARY_DIR}/echelonswidget)
include_directories(../common/echelonswidget)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/echelonswidget)

add_subdirectory(../common/flightlevelwidget ${CMAKE_CURRENT_BINARY_DIR}/flightlevelwidget)
include_directories(../common/flightlevelwidget)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/flightlevelwidget)

add_subdirectory(../common/license ${CMAKE_CURRENT_BINARY_DIR}/license)
include_directories(../common/license)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/license)

add_subdirectory(../common/protocols/xtproto ${CMAKE_CURRENT_BINARY_DIR}/xtproto)
include_directories(../common/protocols/xtproto/src)

set(QJSON_SUBPROJECT TRUE)
set(QJSON_NOT_SET_OWN_DIRECTORY TRUE)
add_subdirectory(../common/qjson ${CMAKE_CURRENT_BINARY_DIR}/qjson)
include_directories(../common/qjson/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/qjson/include)

include(../stca/stca/stca.cmake)

include(../common/initmain/initmain.cmake)

include(../3rdparty/libqxt/qxtspanslider.cmake)
include(../3rdparty/libqxt/qxtcheckcombobox.cmake)

include(../common/oldi/oldi.cmake)

include(../common/oldi/mesviewer/mesviewer.cmake)

include(../common/oldi/nodesviewer/nodesviewer.cmake)

include(../common/qtcheckheader/qtcheckheader.cmake)

include(../common/uppercasevalidator/uppercasevalidator.cmake)

add_subdirectory(../common/aftnmessageprocessor ${CMAKE_CURRENT_BINARY_DIR}/aftnmessageprocessor)
include_directories(../common/aftnmessageprocessor/src/main)

include(../aftn/aftnws/aftnws_fpl.cmake)

add_subdirectory(../common/qtdelegates ${CMAKE_CURRENT_BINARY_DIR}/qtdelegates)
include_directories(../common/qtdelegates)

include(../common/pulsar/pulsar.cmake)

include(../common/stdinreader/stdinreader.cmake)

include(../common/version/version.cmake)

include(../3rdparty/protect/lic.cmake)

include(../common/urzwidgets/urzcombobox.cmake)
include(../common/utils/utils.cmake)

include(../common/editableheader/EditableHeader.cmake)

include(../common/GroundScene/GroundScene.cmake)

include(../common/qttcpdatagramprotocol/qttcpdatagramprotocol.cmake)

include(../3rdparty/redis/QtRedis.cmake)

include(../common/dbaccess/dbaccess.cmake)

include(../common/dbconnectionsmanager/dbconnectionsmanager.cmake)

include(../common/redisconnectionsmanager/redisconnectionsmanager.cmake)

if(WIN32)
    link_directories(
        ${POPPLER_LIB_PATH}
    )
else(WIN32)

endif(WIN32)

# -----------------------------------------------------

message("Building project ${PROJECT_NAME}")
message("
Configuration
-------------
CMAKE_BUILD_TYPE        = ${CMAKE_BUILD_TYPE}
CXXFLAGS                = $ENV{CXXFLAGS}
CMAKE_CXX_FLAGS         = ${CMAKE_CXX_FLAGS}
LDFLAGS                 = $ENV{LDFLAGS}
CMAKE_EXE_LINKER_FLAGS  = ${CMAKE_EXE_LINKER_FLAGS}
CMAKE_INSTALL_PREFIX    = ${CMAKE_INSTALL_PREFIX}
")

if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(CMAKE_VERBOSE_MAKEFILE TRUE)
endif (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release")

# --------------------------------------------------------
# PROJECT COPYRIGHTS

set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_RELEASE 6)
set(VERSION_BUILD 2222)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}" )

add_definitions(-DVERSION_STR="${VERSION}")

set(COMPANY_NAME "Peleng Ltd.")
set(FILE_DESCRIPTION "CWP")
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}.${VERSION_BUILD}")
set(LEGAL_COPYRIGHT "All rights reserved")
set(PRODUCT_NAME "CWP")
set(OUTPUT_NAME "cwp.exe")


# ---------------------------------------------------------

add_definitions(-DCWP_PRJ)

# Uncomment this line in order to build project using updates and set of
# airplaine position via delayed operation (thread).
# add_definitions(-DCWP_AIRPLANE_THREAD)

# sources

list(APPEND Q_HEADERS
    ./src/back/data/Dao.h
    ./src/back/core/user/RoleModel.h
    ./src/back/core/user/SectoringModelImpl.h
    ./src/back/core/user/RoleModelImpl.h
    ./src/back/core/airplane/AirplaneThread.h
    ./src/back/core/Airplane.h
    ./src/back/proto/TCP/Server.h
    ./src/back/proto/TCP/XMLConnect.h
    ./src/back/proto/TCP/Connect.h
    ./src/back/proto/xt/XtListeners.h
    ./src/back/proto/AsterixOld.h
    ./src/back/proto/AsterixListener.h
    ./src/back/proto/AsterixDB.h
    ./src/back/Master.h
    ./src/front/data/SquawkEditor.h
    ./src/front/data/AirplaneTypeEditor.h
    ./src/front/core/wind/WindEditor.h
    ./src/front/core/reminder/GUIAzimuth.h
    ./src/front/core/reminder/XReminder.h
    ./src/front/core/reminder/ReminderItem.h
    ./src/front/core/reminder/ReminderAdd.h
    ./src/front/core/meteo/MeteoEditor.h
    ./src/front/core/meteo/MeteoRunwayWidget.h
    ./src/front/core/meteo/MeteoFilterDlg.h
    ./src/front/core/meteo/MeteoRunwayDialog.h
    ./src/front/core/meteo/RunwayWidget.h
    ./src/front/core/meteo/MeteoWidget.h
    ./src/front/core/restrictions/RestrictionsWidget.h
    ./src/front/core/restrictions/UserRestrictionDlg.h
    ./src/front/core/airplane/GUIAirplaneLabel.h
    ./src/front/core/airplane/GUIAirplaneLabelExt.h
    ./src/front/core/airplane/GUIConflict.h
    ./src/front/core/airplane/XLabelConstructor.h
    ./src/front/core/airplane/fpldlg.h
    ./src/front/core/airplane/GUIControlDlg.h
    ./src/front/core/airplane/XLabel.h
    ./src/front/core/airplane/GUIReminder.h
    ./src/front/core/airplane/relativedlg.h
    ./src/front/core/airplane/GUILabelText.h
    ./src/front/core/airplane/AClist/ListACView.h
    ./src/front/core/airplane/AClist/ACTDlg.h
    ./src/front/core/airplane/AClist/ACTDlgARR.h
    ./src/front/core/airplane/AClist/ACTDlgDEP.h
    ./src/front/core/airplane/AClist/DailyDlgSID.h
    ./src/front/core/airplane/AClist/DailyDlgTime.h
    ./src/front/core/airplane/AClist/ListACItemDelegate.h
    ./src/front/core/airplane/AClist/ListACC.h
    ./src/front/core/airplane/AClist/ListAPP.h
    ./src/front/core/airplane/AClist/ControlDlg.h
    ./src/front/core/airplane/AClist/ListSILACC.h
    ./src/front/core/airplane/AClist/ListSILAPPARR.h
    ./src/front/core/airplane/AClist/ListSILAPPDEP.h
    ./src/front/core/airplane/AClist/ListAC.h
    ./src/front/core/airplane/AClist/ListACModel.h
    ./src/front/core/airplane/AClist/ListACTIN.h
    ./src/front/core/airplane/AClist/ListACTOUT.h
    ./src/front/core/airplane/AClist/ListACTDEP.h
    ./src/front/core/airplane/AClist/ListDAILYPLAN.h
    ./src/front/core/airplane/AClist/ListCOST.h
    ./src/front/core/GUIAirplane.h
    ./src/front/core/GUIView.h
    ./src/front/core/GUISvgItem.h
    ./src/front/core/GUIRestriction.h
    ./src/front/core/XDocWidget.h
    ./src/front/common/ViewDelegate.h
    ./src/front/common/SqlModelHelper.h
    ./src/front/dialog/LayoutDlg.h
    ./src/front/dialog/CustomSector.h
    ./src/front/dialog/ActiveSectors.h
    ./src/front/dialog/Sectoring.h
    ./src/front/dialog/NewPassword.h
    ./src/front/dialog/UserAdmin.h
    ./src/front/dialog/Password.h
    ./src/front/dialog/LoadFromDB.h
    ./src/front/dialog/XAbout.h
    ./src/front/dialog/FMpassDlg.h
    ./src/front/dialog/planeditor.h
    ./src/front/DocWidgetMoveHandler.h
    ./src/front/XMasterWindow.h
    ./src/front/core/airplane/fpltracer/TracerPoint.h
    ./src/front/core/airplane/fpltracer/TracerHDG.h
    ./src/front/dialog/InputCoordDlg.h
    ./src/front/dialog/RcGridCoordDlg.h
    ./src/front/antioverlap/AntioverlapThread.h
    ./src/front/pdfwidgets/pdfdocumentwidget.h
    ./src/front/pdfwidgets/pdfwindow.h
    ./src/front/pandawidgets/actwidget.h
    ./src/front/pandawidgets/acttablemodel.h
    ./src/front/pandawidgets/BaseToolDialog.h
    ./src/front/dialog/BearingDlg.h
    ./src/front/dialog/BearingEditor.h
    ./src/front/pandawidgets/CheckComboBox.h
    ./src/front/pandawidgets/controlwindow.h
    ./src/front/pandawidgets/flightleveldelegate.h
    ./src/front/pandawidgets/lineeditdelegate.h
    ./src/front/pandawidgets/listwidgetdelegate.h
    ./src/front/pandawidgets/qxtspansliderrestricted.h
    ./src/front/pandawidgets/restrictionwidget.h
    ./src/front/pandawidgets/SpeedWidget.h
    ./src/front/pandawidgets/timedelegate.h
    ./src/front/pandawidgets/VerticalSpeedWidget.h
    ./src/front/pandawidgets/DailyPlanWidget.h
    ./src/front/dialog/vpp.h
    ./src/front/core/stands/StandModel.h
    ./src/front/core/stands/StandWidget.h
    ./src/back/proto/Obelix.h
    ./src/back/proto/obelix/ObRespond.h
    ./src/front/core/airplane/GUIListConstructor.h
    ./src/front/core/airplane/AClist/ListACHeaderView.h
    ./src/front/dialog/RunwaySyncManager.h
    ./src/front/core/GUIMapNote.h
    ./src/front/core/GUIScene.h
    ./src/front/dialog/MultiLineInputDialog.h
    ./src/back/data/redisdbhelper.h
    ./src/front/dialog/sfpleditor.h
    ./src/front/dialog/ORCAMDlg.h
    ./src/front/dialog/ORCAMModel.h
    ./src/utils/ExtendedXmlSettings.h
    ./src/front/dialog/UserPicsWidget.h
    ./src/utils/MouseLocker.h
    ./src/utils/AudioPlayer.h
    )


# Get project sources
list(APPEND HEADERS
    ./src/back/core/wind/Wind.h
    ./src/back/core/CFPL.h
    ./src/back/core/meteo/MeteoPoint.h
    ./src/back/core/meteo/Meteo.h
    ./src/back/core/User.h
    ./src/back/core/View.h
    ./src/back/core/airplane/Reminder.h
    ./src/back/core/airplane/AirplaneData.h
    ./src/back/core/airplane/AirplaneType.h
    ./src/back/core/RouteFragmentation.h
    ./src/back/proto/TCP/XMLCmd.h
    ./src/back/proto/xt/XtParent.h
    ./src/back/utils/ProfileSettings.h
    ./src/back/common/XML.h
    ./src/back/common/AirplaneState.h
    ./src/back/common/BearingData.h
    ./src/back/common/Calculation.h
    ./src/front/core/GUIPoint.h
    ./src/front/core/GUILineText.h
    ./src/front/core/meteo/GUIMeteo.h
    ./src/front/core/GUIAirport.h
    ./src/front/core/GUISSA.h
    ./src/front/core/GUIInterface.h
    ./src/front/core/GUIRoute.h
    ./src/front/core/GUISector.h
    ./src/front/core/airplane/GUIAirplanePoint.h
    ./src/front/core/airplane/GUIPixmapItem.h
    ./src/front/core/airplane/AClist/SFPL.h
    ./src/front/core/airplane/AClist/ListACData.h
    ./src/front/core/GUIObject.h
    ./src/front/core/GUIHolding.h
    ./src/front/core/GUIRunway.h
    ./src/front/core/GUIBound.h
    ./src/front/core/meteo/MeteoRunwayModel.h
    ./src/front/core/meteo/MeteoRunwayDelegate.h
    ./src/front/core/meteo/RunwayWidgetData.h
    ./src/front/common/XAppSettings.h
    ./src/front/common/XGridLabel.h
    ./src/front/common/CoordsDigitizer.h
    ./src/utils/Coordinate.h
    ./src/import/SSA.h
    ./src/import/Runway.h
    ./src/import/Restriction.h
    ./src/import/UserPics.h
    ./src/import/SvgItem.h
    ./src/import/Point.h
    ./src/import/Zone.h
    ./src/import/Holding.h
    ./src/import/BoundaryPoint.h
    ./src/import/Scene.h
    ./src/import/Route.h
    ./src/import/ItemHandler.h
    ./src/import/Airport.h
    ./src/import/MeteoRunway.h
    ./src/main.h
    ./src/front/core/airplane/fpltracer/TracerFPL.h
    ./src/front/core/airplane/fpltracer/TracerHandler.h
    ./src/front/common/EchelonValues.h
    ./src/back/common/FLVal.h
    ../common/pulsar_fdp/pulsar_fdp_settings.h
    ./src/front/core/GUIRcGrid.h
    ./src/back/core/airplane/BearingPoint.h
    ./src/back/core/airplane/AirplaneSTCA.h
    ./src/back/core/Plane.h
    ./src/back/core/SimulatorTrack.h
    ./src/back/core/SimulatorTrackVar.h
    ./src/back/core/RcGridValues.h
    ./src/back/core/airplane/AirplaneMTCD.h
    ./src/front/core/airplane/GUIAirplaneMTCDPoint.h
    ./src/back/proto/obelix/CmdAirplanaCmd.h
    ./src/back/proto/obelix/CmdAirplanaDel.h
    ./src/back/proto/obelix/CmdAirplanaStat.h
    ./src/back/proto/obelix/CmdHello.h
    ./src/back/proto/obelix/CmdPut.h
    ./src/back/proto/obelix/CmdReminderAdd.h
    ./src/back/proto/obelix/CmdReminderDel.h
    ./src/back/proto/obelix/CmdTake.h
    ./src/back/proto/obelix/CmdTaken.h
    ./src/back/proto/obelix/Common.h
    ./src/back/proto/obelix/CmdTransfer.h
    ./src/back/proto/obelix/CmdSFPLRefresh.h
    ./src/back/proto/obelix/CmdGUIRefresh.h
    ./src/back/proto/obelix/CmdChangeSFPL.h
    ./src/back/proto/obelix/CmdReminderAction.h
    ./src/back/proto/obelix/CmdSectorChanged.h
    ./src/back/proto/obelix/CmdSetSector.h
    ./src/back/proto/obelix/CmdReminderList.h
    ./src/back/proto/obelix/CmdSectoringAction.h
    ./src/back/proto/obelix/CmdRequestAllFPL.h
    ./src/back/proto/obelix/CmdSendUserZone.h
    ./src/back/proto/obelix/CmdMeteoUpdate.h
    ./src/back/common/Intersection.h

    ./src/back/proto/obelix/CmdAirplanaCmdRespond.h
    ./src/back/proto/obelix/CmdStandChanged.h
    ./src/back/proto/obelix/CmdVehicleState.h
    ./src/front/core/airplane/ListConstructor.h
    ./src/back/common/VehicleState.h
    ./src/front/pandawidgets/restrictionlistwidgetitem.h
    ./src/front/pandawidgets/restrictionlineedit.h
    ./src/front/pandawidgets/conversionfunctions.h
    ./src/front/antioverlap/FormularDefs.h
    ./src/front/antioverlap/StepLeftRightStrategy.h
    ./src/front/antioverlap/AntioverlapStrategy.h
    ./src/import/MapNote.h

    ./src/utils/OperationsLog.h
    )

list(APPEND SOURCES
    ./src/back/data/Dao.cpp
    ./src/back/core/user/RoleModel.cpp
    ./src/back/core/user/SectoringModelImpl.cpp
    ./src/back/core/user/RoleModelImpl.cpp
    ./src/back/core/CFPL.cpp
    ./src/back/core/airplane/Reminder.cpp
    ./src/back/core/airplane/AirplaneType.cpp
    ./src/back/core/airplane/AirplaneThread.cpp
    ./src/back/core/User.cpp
    ./src/back/core/RouteFragmentation.cpp
    ./src/back/core/View.cpp
    ./src/import/Scene.cpp
    ./src/back/core/Airplane.cpp
    ./src/back/proto/TCP/Server.cpp
    ./src/back/proto/TCP/Connect.cpp
    ./src/back/proto/TCP/XMLCmd.cpp
    ./src/back/proto/TCP/XMLConnect.cpp
    ./src/back/proto/xt/XtListeners.cpp
    ./src/back/proto/AsterixOld.cpp
    ./src/back/proto/AsterixListener.cpp
    ./src/back/proto/AsterixDB.cpp
    ./src/back/utils/ProfileSettings.cpp
    ./src/back/common/AirplaneState.cpp
    ./src/back/common/Calculation.cpp
    ./src/back/common/XML.cpp
    ./src/back/Master.cpp
    ./src/front/data/SquawkEditor.cpp
    ./src/front/data/AirplaneTypeEditor.cpp
    ./src/front/core/wind/WindEditor.cpp
    ./src/front/core/GUIPoint.cpp
    ./src/front/core/GUISSA.cpp
    ./src/front/core/reminder/ReminderItem.cpp
    ./src/front/core/reminder/GUIAzimuth.cpp
    ./src/front/core/reminder/XReminder.cpp
    ./src/front/core/reminder/ReminderAdd.cpp
    ./src/front/core/GUILineText.cpp
    ./src/front/core/meteo/MeteoEditor.cpp
    ./src/front/core/meteo/GUIMeteo.cpp
    ./src/front/core/meteo/MeteoFilterDlg.cpp
    ./src/front/core/meteo/MeteoRunwayWidget.cpp
    ./src/front/core/meteo/MeteoRunwayModel.cpp
    ./src/front/core/meteo/MeteoRunwayDelegate.cpp
    ./src/front/core/meteo/MeteoRunwayDialog.cpp
    ./src/front/core/meteo/RunwayWidget.cpp
    ./src/front/core/meteo/MeteoWidget.cpp
    ./src/front/core/meteo/RunwayWidgetData.cpp
    ./src/front/core/restrictions/RestrictionsWidget.cpp
    ./src/front/core/restrictions/UserRestrictionDlg.cpp
    ./src/front/core/XDocWidget.cpp
    ./src/front/core/GUIHolding.cpp
    ./src/front/core/GUIRunway.cpp
    ./src/front/core/GUIAirport.cpp
    ./src/front/core/airplane/GUILabelText.cpp
    ./src/front/core/airplane/relativedlg.cpp
    ./src/front/core/airplane/XLabel.cpp
    ./src/front/core/airplane/GUIControlDlg.cpp
    ./src/front/core/airplane/XLabelConstructor.cpp
    ./src/front/core/airplane/GUIAirplanePoint.cpp
    ./src/front/core/airplane/fpldlg.cpp
    ./src/front/core/airplane/GUIAirplaneLabel.cpp
    ./src/front/core/airplane/GUIAirplaneLabelExt.cpp
    ./src/front/core/airplane/GUIConflict.cpp
    ./src/front/core/airplane/GUIReminder.cpp
    ./src/front/core/airplane/GUIPixmapItem.cpp
    ./src/front/core/airplane/AClist/ACTDlg.cpp
    ./src/front/core/airplane/AClist/ACTDlgARR.cpp
    ./src/front/core/airplane/AClist/ACTDlgDEP.cpp
    ./src/front/core/airplane/AClist/DailyDlgSID.cpp
    ./src/front/core/airplane/AClist/DailyDlgTime.cpp
    ./src/front/core/airplane/AClist/ListSILAPPARR.cpp
    ./src/front/core/airplane/AClist/ListACView.cpp
    ./src/front/core/airplane/AClist/ListACModel.cpp
    ./src/front/core/airplane/AClist/ListSILAPPDEP.cpp
    ./src/front/core/airplane/AClist/ListACC.cpp
    ./src/front/core/airplane/AClist/ListACData.cpp
    ./src/front/core/airplane/AClist/ListAC.cpp
    ./src/front/core/airplane/AClist/ListAPP.cpp
    ./src/front/core/airplane/AClist/ListACItemDelegate.cpp
    ./src/front/core/airplane/AClist/ControlDlg.cpp
    ./src/front/core/airplane/AClist/ListSILACC.cpp
    ./src/front/core/airplane/AClist/ACTDlg.cpp
    ./src/front/core/airplane/AClist/ListACTIN.cpp
    ./src/front/core/airplane/AClist/ListACTOUT.cpp
    ./src/front/core/airplane/AClist/ListACTDEP.cpp
    ./src/front/core/airplane/AClist/ListDAILYPLAN.cpp
    ./src/front/core/airplane/AClist/ListCOST.cpp
    ./src/front/core/GUISector.cpp
    ./src/front/core/GUIBound.cpp
    ./src/front/core/GUIObject.cpp
    ./src/front/core/GUIRestriction.cpp
    ./src/front/core/GUIUserPics.cpp
    ./src/front/core/GUISvgItem.cpp
    ./src/front/core/GUIView.cpp
    ./src/front/core/GUIAirplane.cpp
    ./src/front/core/GUIScene.cpp
    ./src/front/core/GUIRoute.cpp
    ./src/front/common/ViewDelegate.cpp
    ./src/front/common/XGridLabel.cpp
    ./src/front/common/SqlModelHelper.cpp
    ./src/front/common/XAppSettings.cpp
    ./src/front/common/CoordsDigitizer.cpp
    ./src/front/dialog/NewPassword.cpp
    ./src/front/dialog/Password.cpp
    ./src/front/dialog/ActiveSectors.cpp
    ./src/front/dialog/LayoutDlg.cpp
    ./src/front/dialog/CustomSector.cpp
    ./src/front/dialog/Sectoring.cpp
    ./src/front/dialog/UserAdmin.cpp
    ./src/front/dialog/LoadFromDB.cpp
    ./src/front/dialog/XAbout.cpp
    ./src/front/dialog/FMpassDlg.cpp
    ./src/front/dialog/planeditor.cpp
    ./src/front/XMasterWindow.cpp
    ./src/front/DocWidgetMoveHandler.cpp
    ./src/utils/Coordinate.cpp
    ./src/main.cpp
    ./src/import/Runway.cpp
    ./src/import/BoundaryPoint.cpp
    ./src/import/Restriction.cpp
    ./src/import/UserPics.cpp
    ./src/import/SvgItem.cpp
    ./src/import/SSA.cpp
    ./src/import/Route.cpp
    ./src/import/Point.cpp
    ./src/import/Holding.cpp
    ./src/import/Airport.cpp
    ./src/import/Zone.cpp
    ./src/import/MeteoRunway.cpp
    ./src/front/core/airplane/fpltracer/TracerFPL.cpp
    ./src/front/core/airplane/fpltracer/TracerHandler.cpp
    ./src/front/core/airplane/fpltracer/TracerPoint.cpp
    ./src/front/core/airplane/fpltracer/TracerHDG.cpp
    ./src/front/common/EchelonValues.cpp
    ./src/back/common/FLVal.cpp
    ./src/front/core/GUIRcGrid.cpp
    ./src/front/dialog/InputCoordDlg.cpp
    ./src/front/antioverlap/AntioverlapThread.cpp
    ./src/front/antioverlap/AntioverlapStrategy.cpp
    ./src/front/antioverlap/StepLeftRightStrategy.cpp
    ./src/front/antioverlap/StepLengthAngleStrategy.cpp
    ./src/front/pandawidgets/acttablemodel.cpp
    ./src/front/pdfwidgets/pdfdocumentwidget.cpp
    ./src/front/pdfwidgets/pdfwindow.cpp
    ./src/front/pandawidgets/actwidget.cpp
    ./src/front/pandawidgets/BaseToolDialog.cpp
    ./src/front/dialog/RcGridCoordDlg.cpp
    ./src/front/dialog/BearingDlg.cpp
    ./src/front/dialog/BearingEditor.cpp
    ./src/front/pandawidgets/CheckComboBox.cpp
    ./src/front/pandawidgets/controlwindow.cpp
    ./src/front/pandawidgets/conversionfunctions.cpp
    ./src/front/pandawidgets/flightleveldelegate.cpp
    ./src/front/pandawidgets/lineeditdelegate.cpp
    ./src/front/pandawidgets/listwidgetdelegate.cpp
    ./src/front/pandawidgets/qxtspansliderrestricted.cpp
    ./src/front/pandawidgets/restrictionlistwidgetitem.cpp
    ./src/front/pandawidgets/restrictionwidget.cpp
    ./src/front/pandawidgets/restrictionlineedit.cpp
    ./src/front/pandawidgets/SpeedWidget.cpp
    ./src/front/pandawidgets/timedelegate.cpp
    ./src/front/pandawidgets/VerticalSpeedWidget.cpp
    ./src/front/pandawidgets/DailyPlanWidget.cpp
    ./src/front/dialog/vpp.cpp
    ./src/back/core/airplane/AirplaneSTCA.cpp
    ./src/back/core/Plane.cpp
    ./src/back/core/SimulatorTrack.cpp
    ./src/back/core/RcGridValues.cpp
    ./src/back/core/airplane/AirplaneMTCD.cpp
    ./src/front/core/airplane/GUIAirplaneMTCDPoint.cpp
    ./src/front/core/stands/StandModel.cpp
    ./src/front/core/stands/StandWidget.cpp
    ./src/back/proto/Obelix.cpp
    ./src/back/proto/obelix/CmdAirplanaCmd.cpp
    ./src/back/proto/obelix/CmdAirplanaDel.cpp
    ./src/back/proto/obelix/CmdAirplanaStat.cpp
    ./src/back/proto/obelix/CmdHello.cpp
    ./src/back/proto/obelix/CmdPut.cpp
    ./src/back/proto/obelix/CmdReminderAdd.cpp
    ./src/back/proto/obelix/CmdReminderDel.cpp
    ./src/back/proto/obelix/CmdTake.cpp
    ./src/back/proto/obelix/CmdTaken.cpp
    ./src/back/proto/obelix/ObRespond.cpp
    ./src/back/proto/obelix/CmdTransfer.cpp
    ./src/back/proto/obelix/CmdSFPLRefresh.cpp
    ./src/back/proto/obelix/CmdGUIRefresh.cpp
    ./src/back/proto/obelix/CmdChangeSFPL.cpp
    ./src/back/proto/obelix/CmdReminderAction.cpp
    ./src/back/proto/obelix/CmdSectorChanged.cpp
    ./src/back/proto/obelix/CmdSetSector.cpp
    ./src/back/proto/obelix/CmdReminderList.cpp
    ./src/back/proto/obelix/CmdSectoringAction.cpp
    ./src/back/proto/obelix/CmdRequestAllFPL.cpp
    ./src/back/proto/obelix/CmdSendUserZone.cpp
    ./src/back/proto/obelix/CmdMeteoUpdate.cpp
    ./src/back/proto/obelix/CmdStandChanged.cpp
    ./src/back/proto/obelix/CmdVehicleState.cpp
    ./src/back/common/Intersection.cpp
    ./src/back/proto/obelix/CmdAirplanaCmdRespond.cpp
    ./src/front/core/airplane/ListConstructor.cpp
    ./src/front/core/airplane/GUIListConstructor.cpp
    ./src/front/core/airplane/AClist/ListACHeaderView.cpp
    ./src/front/dialog/RunwaySyncManager.cpp
    ./src/front/core/GUIMapNote.cpp
    ./src/front/dialog/MultiLineInputDialog.cpp
    ./src/back/data/redisdbhelper.cpp
    ./src/front/core/airplane/AClist/SFPL.cpp
    ./src/front/dialog/sfpleditor.cpp
    ./src/front/dialog/ORCAMDlg.cpp
    ./src/front/dialog/ORCAMModel.cpp
    ./src/utils/ExtendedXmlSettings.cpp
    ./src/utils/OperationsLog.cpp
    ./src/front/dialog/UserPicsWidget.cpp
    ./src/utils/MouseLocker.cpp
    ./src/utils/AudioPlayer.cpp
    )

list(APPEND FORMS
    ./src/front/data/AirplaneTypeEditor.ui
    ./src/front/data/SquawkEditor.ui
    ./src/front/core/reminder/ReminderItem.ui
    ./src/front/core/reminder/ReminderAdd.ui
    ./src/front/core/airplane/Azimuth.ui
    ./src/front/core/airplane/FormularFPL.ui
    ./src/front/core/airplane/AClist/DailyDlgSID.ui
    ./src/front/core/airplane/AClist/DailyDlgTime.ui
    ./src/front/core/airplane/LabelConstructor.ui
    ./src/front/core/meteo/MeteoFilterDlg.ui
    ./src/front/core/meteo/MeteoRunwayWidget.ui
    ./src/front/core/meteo/MeteoRunwayDialog.ui
    ./src/front/core/meteo/RunwayWidget.ui
    ./src/front/core/meteo/MeteoWidget.ui
    ./src/front/core/restrictions/RestrictionsWidget.ui
    ./src/front/core/restrictions/UserRestrictionDlg.ui
    ./src/front/dialog/ACTDlg.ui
    ./src/front/dialog/ACTDlgARR.ui
    ./src/front/dialog/ACTDlgDEP.ui
    ./src/front/pdfwidgets/pdfwindow.ui
    ./src/front/pandawidgets/actwidget.ui
    ./src/front/pandawidgets/controlwindow.ui
    ./src/front/pandawidgets/restrictionwidget.ui
    ./src/front/pandawidgets/SpeedWidget.ui
    ./src/front/pandawidgets/VerticalSpeedWidget.ui
    ./src/front/pandawidgets/DailyPlanWidget.ui
    ./src/front/dialog/Sectoring.ui
    ./src/front/dialog/UserAdmin.ui
    ./src/front/dialog/ExerciseDlg.ui
    ./src/front/dialog/Password.ui
    ./src/front/dialog/NewPassword.ui
    ./src/front/dialog/ActiveSectors.ui
    ./src/front/dialog/LoadFromDB.ui
    ./src/front/dialog/StartAirplanes.ui
    ./src/front/dialog/LayoutDlg.ui
    ./src/front/dialog/CustomSector.ui
    ./src/front/dialog/About.ui
    ./src/front/dialog/BearingDlg.ui
    ./src/front/dialog/BearingEditor.ui
    ./src/front/dialog/vpp.ui
    ./src/front/dialog/planeditor.ui
    ./src/front/core/airplane/GUIListConstructor.ui
    ./src/front/dialog/MultiLineInputDialog.ui
    ./src/front/dialog/sfpleditor.ui
    ./src/front/dialog/UserPicsWidget.ui
    ./src/front/dialog/ORCAMDlg.ui
    )

list(APPEND RESOURCES share/resource.qrc)

# Walk throw file list to include all sources directories
foreach(HEADER_FILE ${Q_HEADERS} ${HEADERS})
  get_filename_component(HEADER_PATH ${HEADER_FILE} PATH)
  include_directories(${HEADER_PATH})
endforeach(HEADER_FILE)
include_directories(src/)
include_directories(src/main)

# if you use Q_OBJECT
#qt45_wrap_cpp(MOC_SOURCES ${Q_HEADERS})
qt45_wrap_ui(UI_HEADERS ${FORMS})
qt45_add_resources(QRC_SOURCES ${RESOURCES})

#create app icon
set(WIN_RESOURCES)

if(MINGW AND WIN32)
  # resource for mingw win32
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/master.rc.in ${CMAKE_CURRENT_BINARY_DIR}/master.rc)
  list(APPEND WIN_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/master.rc)
endif(MINGW AND WIN32)

# COMPILATION
add_executable(${PROJECT_NAME} WIN32
  ${SOURCES}
  ${Q_SOURCES}
  ${MOC_SOURCES}
  ${QRC_SOURCES}
  ${Q_HEADERS}
  ${HEADERS}
  ${UI_HEADERS}
  ${QS_SOURCES}
  ${WIN_RESOURCES}
  )

# LICENSE
SET(SECRET_KEY "" CACHE STRING "License secret key")
target_compile_definitions(${PROJECT_NAME} PRIVATE SECRET_KEY="${SECRET_KEY}")
message("SECRET KEY: ${SECRET_KEY}")

################################################
# LINKING
################################################
# main library
# Rename result target file
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_debug" VERSION ${VERSION})

if (MSVC)
    set(BIN_INSTALLATION_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "path to install binaries" FORCE)

    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_INSTALLATION_PATH}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_INSTALLATION_PATH}
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_INSTALLATION_PATH}
    )
endif(MSVC)

if(WIN32)
    set(EXTERNAL_LIBS
        debug poppler-qt4d optimized poppler-qt4
    )
else(WIN32)
    if(USE_QT5)
        set(EXTERNAL_LIBS poppler-qt5 phonon4qt5)
    else(USE_QT5)
        set(EXTERNAL_LIBS poppler-qt4 phonon)
    endif(USE_QT5)
endif(WIN32)

target_link_libraries(${PROJECT_NAME}
  qtxmlsettings
  qtautotransaction
  qtlanguagemenu
  asterix
  aftnmessageprocessor
  log4qt
  echelonswidget
  flightlevelwidget
  license
  qtdelegates
  XtProto
  qjson
  ${EXTERNAL_LIBS}
  #TVAUtils  #
#  ${proj4}
  )

link_qt(${PROJECT_NAME})
################################################

################################################
# Doxygen documentation generation
################################################
### DOXYGEN DOCUMENTATION SYSTEM
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/doc")
set(DOXYGEN_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/doxygen")
set(DOXYGEN_DOCSET_FEEDNAME "${PROJECT_NAME}")
set(DOXYGEN_OUTPUT_LANGUAGE "English")

find_program(QT_QHELPGENERATOR_EXECUTABLE
  NAMES qhelpgenerator
  PATHS ${QT_BINARY_DIR}
  NO_DEFAULT_PATH)

if(QT_QHELPGENERATOR_EXECUTABLE)
  set(DOXYGEN_GENERATE_QHP "YES")
  # relative to html directory
  set(DOXYGEN_QCH_FILE  "../qch/${PROJECT_NAME}.qch")
  set(DOXYGEN_QHP_NAMESPACE "com.trolltech.${PROJECT_NAME}.1")
  set(DOXYGEN_QHP_VIRTUAL_FOLDER "${PROJECT_NAME}-api")
  set(DOXYGEN_QHG_LOCATION "${QT_QHELPGENERATOR_EXECUTABLE}")
else(QT_QHELPGENERATOR_EXECUTABLE)
  set(DOXYGEN_GENERATE_QHP "NO")
endif(QT_QHELPGENERATOR_EXECUTABLE)

find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
  add_custom_target(doxygen
    COMMAND ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  message("Command to create html and qch doc: make (mingw32-make) doxygen")
endif (DOXYGEN_FOUND)

if (DOXYGEN_DOT_FOUND)
  set(DOXYGEN_HAVE_DOT "YES")
  message("Enabled graphs")
else(DOXYGEN_DOT_FOUND)
  set(DOXYGEN_HAVE_DOT "NO")
endif(DOXYGEN_DOT_FOUND)

set(QCH_DOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc/qch)
set(QCH_DOC_NAMES ${PROJECT_NAME}.qch)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
  "${CMAKE_BINARY_DIR}/Doxyfile"
  IMMEDIATE @ONLY)
#################################################

#################################################
# Installation
#################################################

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT Runtime)

########################################
## Uninstalling for unix "make uninstall"
## Note: uninstallning system for windows automatically generated by cpack
########################################

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
