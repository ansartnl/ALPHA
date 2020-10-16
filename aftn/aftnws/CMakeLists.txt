cmake_minimum_required(VERSION 2.6)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# ------------------------------------------------------------
# CMake setup
# ------------------------------------------------------------

# Declare project
project(aftnws)

# Uncomment this definition when building for Microkod
# add_definitions(-DMICROKOD_BUILD)

set(CMAKE_INCLUDE_CURRENT_DIR true)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

find_package(Qt4 REQUIRED)
# SOURCE FILES AND DIRECTORIES
# ------------------------------------------------------------------
# Setup Qt
set(QT_USE_QTSQL TRUE)
set(QT_USE_QTUITOOLS TRUE)
set(QT_USE_QTNETWORK TRUE)
# Create libraries list
include(${QT_USE_FILE})
# -----------------------------------------------------
# 3rd party
# -----------------------------------------------------

# all 3rd party subdirectories ---------------------------------------------------------
add_subdirectory(../../common/qtcsvparser ${CMAKE_CURRENT_BINARY_DIR}/qtcsvparser)
include_directories(../../common/qtcsvparser)
add_subdirectory(../../common/qtmenuview ${CMAKE_CURRENT_BINARY_DIR}/qtmenuview)
include_directories(../../common/qtmenuview)
add_subdirectory(../../common/qttoolbardialog ${CMAKE_CURRENT_BINARY_DIR}/qttoolbardialog)
include_directories(../../common/qttoolbardialog)
add_subdirectory(../../common/qtwindowmenu ${CMAKE_CURRENT_BINARY_DIR}/qtwindowmenu)
include_directories(../../common/qtwindowmenu)
add_subdirectory(../../common/qtdelegates ${CMAKE_CURRENT_BINARY_DIR}/qtdelegates)
include_directories(../../common/qtdelegates)

add_subdirectory(../../common/aftnmessageprocessor ${CMAKE_CURRENT_BINARY_DIR}/aftnmessageprocessor)
include_directories(../../common/aftnmessageprocessor/src/main)

add_subdirectory(../../common/qtxmlsettings ${CMAKE_CURRENT_BINARY_DIR}/qtxmlsettings)
include_directories(../../common/qtxmlsettings)

add_subdirectory(../../common/qtlanguagemenu ${CMAKE_CURRENT_BINARY_DIR}/qtlanguagemenu)
include_directories(../../common/qtlanguagemenu)

add_subdirectory(../../common/qtautotransaction ${CMAKE_CURRENT_BINARY_DIR}/qtautotransaction)
include_directories(../../common/qtautotransaction)

set(LOG4QT_BUILD_STATIC TRUE)
set(LOG4QT_DONT_INSTALL TRUE)
add_subdirectory(../../common/log4qt ${CMAKE_CURRENT_BINARY_DIR}/log4qt)
include_directories(../../common/log4qt/src)

include(../../common/initmain/initmain.cmake)

include(../../common/pulsar/pulsar.cmake)

include(../../common/stdinreader/stdinreader.cmake)

include(../../common/version/version.cmake)

include(../../common/enumstreaming/enumstreaming.cmake)

include(../../common/qtcheckheader/qtcheckheader.cmake)

include(../../common/dbsettings/dbsettings.cmake)

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

set(CMAKE_INCLUDE_CURRENT_DIR true)

# project version
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_RELEASE 0)
set (VERSION_BUILD 0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}" )

add_definitions(-DVERSION_STR="${VERSION}")

set(COMPANY_NAME "Peleng Ltd.")
set(FILE_DESCRIPTION "${PROJECT_NAME}")
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}.${VERSION_BUILD}")
set(LEGAL_COPYRIGHT "All rights reserved")
set(PRODUCT_NAME "${PROJECT_NAME}")
set(OUTPUT_NAME "${PROJECT_NAME}.exe")

add_definitions(-DAFTNWS_PRJ_APP)

# Get project sources

list(APPEND SOURCES ./src/gui/objectsmanager.cpp
    ./src/gui/uppercasevalidator.cpp
    ./src/gui/objectsmanagerhelper.cpp
    ./src/gui/dialog/userpassworddialog.cpp
    ./src/gui/dialog/logindialog.cpp
    ./src/gui/dialog/abstractdialog.cpp
    ./src/gui/dialog/importaringdialog.cpp
    ./src/gui/dialog/useraccountsdialog.cpp
    ./src/gui/dialog/printdatadialog.cpp
    ./src/gui/custom/customtextedit.cpp
    ./src/gui/editor/rpleditor.cpp
    ./src/gui/editor/fpleditor.cpp
    ./src/gui/editor/fplviewer.cpp
    ./src/gui/editor/notamviewer.cpp
    ./src/gui/editor/manualeditor.cpp
    ./src/gui/widgets/fplwidget.cpp
    ./src/gui/widgets/fpleditwidget.cpp
    ./src/gui/browser/globalhistorybrowser.cpp
    ./src/gui/browser/fplhistorybrowser.cpp
    ./src/gui/browser/rplbrowser.cpp
    ./src/gui/browser/aftnservicelog.cpp
    ./src/gui/browser/useractionsbrowser.cpp
    ./src/gui/browser/fplbrowser.cpp
    ./src/gui/browser/notambrowser.cpp
    ./src/gui/browser/notamhistorybrowser.cpp
    ./src/gui/browser/zonebrowser.cpp
    ./src/gui/browser/abstractbrowser.cpp
    ./src/gui/browser/sqltablebrowser.cpp
    ./src/gui/browser/fplonfuturebrowser.cpp
    ./src/gui/browser/sendtelegrambrowser.cpp
    ./src/gui/browser/receivedtelegrambrowser.cpp
    ./src/data/AFTNDao.cpp
    ./src/core/rpl.cpp
    ./src/core/notam.cpp
    ./src/core/fpl.cpp
    ./src/core/fpltan.cpp
    ./src/main/main.cpp
    ./src/main/managers/navigateactionsmanager/navigateactionsmanager.cpp
    ./src/main/managers/navigateactionsmanager/navigateactionsmanagerhelper.cpp
    ./src/main/managers/customactionsmanager/customactionsmanager.cpp
    ./src/main/managers/customactionsmanager/customactionsmanagerhelper.cpp
    ./src/main/managers/printactionsmanager/printactionsmanager.cpp
    ./src/main/managers/printactionsmanager/printactionsmanagerhelper.cpp
    ./src/main/managers/editactionsmanager/editactionsmanager.cpp
    ./src/main/managers/editactionsmanager/editactionsmanagerhelper.cpp
    ./src/main/mainwindow.cpp
    ./src/global/ociutils.cpp
    ./src/global/translations.cpp
    ./src/global/printutils.cpp

    ./src/kernel/views/qtsqltableview.cpp
    ./src/kernel/views/ocitableview.cpp
    ./src/kernel/views/qtheaderview.cpp
    ./src/kernel/views/ociqueryview.cpp
    ./src/kernel/models/standardtreemodel.cpp
    ./src/kernel/models/proxy/sectionsproxymodel.cpp
    ./src/kernel/models/ocitablemodel.cpp
    ./src/kernel/models/ociquerymodel.cpp

    ./src/telegram/pages/ArrWidget.cpp
    ./src/telegram/pages/ChgWidget.cpp
    ./src/telegram/pages/CnlWidget.cpp
    ./src/telegram/pages/DepWidget.cpp
    ./src/telegram/pages/DlaWidget.cpp
    ./src/telegram/pages/FplTelegramWidget.cpp
    ./src/telegram/telegram/CreateTelegramWizard.cpp
    ./src/telegram/parsererrors/ParserErrors.cpp
    ./src/telegram/atsmessages/AftnMessage.cpp
    ./src/telegram/atsmessages/ArrMessage.cpp
    ./src/telegram/atsmessages/AtsMessage.cpp
    ./src/telegram/atsmessages/ChgMessage.cpp
    ./src/telegram/atsmessages/CnlMessage.cpp
    ./src/telegram/atsmessages/DepMessage.cpp
    ./src/telegram/atsmessages/DlaMessage.cpp
    ./src/telegram/atsmessages/FplMessage.cpp
    ./src/telegram/atsmessages/LamMessage.cpp
    ./src/telegram/atsmessages/NotamMessage.cpp
    ./src/telegram/atsmessages/PpnMessage.cpp
    ./src/telegram/atsmessages/UnknownMessage.cpp
    ./src/telegram/smartsender/SmartSender.cpp
    ./src/telegram/InternalClient.cpp
    ./src/telegram/getmapdialog.cpp
    ./src/telegram/addressbook/addressbook.cpp
    ./src/telegram/addressbook/abonentdlg.cpp
    ./src/telegram/addressbook/autotypeswidget.cpp
)

list(APPEND P_HEADERS
    ./src/gui/widgets/private/qtsqlrecordwidget_p.h
    ./src/gui/private/objectsmanager_p.h
    ./src/main/managers/navigateactionsmanager/private/navigateactionsmanager_p.h
    ./src/main/managers/customactionsmanager/private/customactionsmanager_p.h
    ./src/main/managers/printactionsmanager/private/printactionsmanager_p.h
    ./src/main/managers/editactionsmanager/private/editactionsmanager_p.h
    ./src/main/private/mainwindow_p.h
    ./src/kernel/views/private/qtsqltableview_p.h
    ./src/kernel/views/private/ocitableview_p.h
    ./src/kernel/views/private/qtheaderview_p.h
    ./src/kernel/views/private/ociqueryview_p.h
    ./src/kernel/models/proxy/private/sectionsproxymodel_p.h
    ./src/kernel/models/private/ocitablemodel_p.h
    ./src/kernel/models/private/ociquerymodel_p.h
    ../../common/pulsar_fdp/pulsar_fdp_settings.h
)

list(APPEND Q_HEADERS
    ./src/gui/objectsmanager.h
    ./src/gui/uppercasevalidator.h
    ./src/gui/dialog/userpassworddialog.h
    ./src/gui/dialog/abstractdialog.h
    ./src/gui/dialog/useraccountsdialog.h
    ./src/gui/dialog/logindialog.h
    ./src/gui/dialog/importaringdialog.h
    ./src/gui/dialog/printdatadialog.h
    ./src/gui/custom/customtextedit.h
    ./src/gui/custom/intcheckbox.h
    ./src/gui/editor/manualeditor.h
    ./src/gui/editor/rpleditor.h
    ./src/gui/editor/notamviewer.h
    ./src/gui/editor/fplviewer.h
    ./src/gui/editor/fpleditor.h
    ./src/gui/objectsmanagerhelper.h
    ./src/gui/widgets/fplwidget.h
    ./src/gui/widgets/fpleditwidget.h
    ./src/gui/browser/abstractbrowser.h
    ./src/gui/browser/fplbrowser.h
    ./src/gui/browser/sqltablebrowser.h
    ./src/gui/browser/aftnservicelog.h
    ./src/gui/browser/notamhistorybrowser.h
    ./src/gui/browser/fplhistorybrowser.h
    ./src/gui/browser/useractionsbrowser.h
    ./src/gui/browser/zonebrowser.h
    ./src/gui/browser/globalhistorybrowser.h
    ./src/gui/browser/notambrowser.h
    ./src/gui/browser/rplbrowser.h
    ./src/gui/browser/fplonfuturebrowser.h
    ./src/gui/browser/sendtelegrambrowser.h
    ./src/gui/browser/receivedtelegrambrowser.h
    ./src/main/mainwindow.h
    ./src/main/managers/navigateactionsmanager/navigateactionsmanagerhelper.h
    ./src/main/managers/navigateactionsmanager/navigateactionsmanager.h
    ./src/main/managers/customactionsmanager/customactionsmanager.h
    ./src/main/managers/customactionsmanager/customactionsmanagerhelper.h
    ./src/main/managers/printactionsmanager/printactionsmanager.h
    ./src/main/managers/printactionsmanager/printactionsmanagerhelper.h
    ./src/main/managers/editactionsmanager/editactionsmanagerhelper.h
    ./src/main/managers/editactionsmanager/editactionsmanager.h
    ./src/kernel/views/ocitableview.h
    ./src/kernel/views/ociqueryview.h
    ./src/kernel/views/qtsqltableview.h
    ./src/kernel/views/qtheaderview.h
    ./src/kernel/models/proxy/sectionsproxymodel.h
    ./src/kernel/models/ocitablemodel.h
    ./src/kernel/models/standardtreemodel.h
    ./src/kernel/models/ociquerymodel.h
    ./src/global/translations.h
    ./src/main/main.h
	
    ./src/telegram/pages/ArrWidget.h
    ./src/telegram/pages/ChgWidget.h
    ./src/telegram/pages/CnlWidget.h
    ./src/telegram/pages/DepWidget.h
    ./src/telegram/pages/DlaWidget.h
    ./src/telegram/pages/FplTelegramWidget.h
    ./src/telegram/telegram/CreateTelegramWizard.h
    ./src/telegram/InternalClient.h
    ./src/telegram/getmapdialog.h
    ./src/telegram/addressbook/addressbook.h
    ./src/telegram/addressbook/abonentdlg.h
    ./src/telegram/addressbook/autotypeswidget.h
)

list(APPEND HEADERS
    ./src/data/AFTNDao.h
    ./src/core/fpl.h
    ./src/core/fpltan.h
    ./src/core/rpl.h
    ./src/core/notam.h
    ./src/global/printutils.h
    ./src/global/ociutils.h
    ./src/kernel/interfaces/modelinterfaces.h
	
    ./src/telegram/interaction/defines.h
    ./src/telegram/interaction/replies.h
    ./src/telegram/interaction/requests.h
    ./src/telegram/interaction/templates.h
    ./src/telegram/parsererrors/ParserErrors.h
    ./src/telegram/atsmessages/AftnMessage.h
    ./src/telegram/atsmessages/AtsMessages.h
    ./src/telegram/smartsender/SmartSender.h
)

list(APPEND FORMS
    ./src/gui/dialog/logindialog.ui
    ./src/gui/dialog/useraccountsdialog.ui
    ./src/gui/dialog/importaringdialog.ui
    ./src/gui/dialog/userpassworddialog.ui
    ./src/gui/dialog/printdatadialog.ui
    ./src/gui/editor/manualeditor.ui
    ./src/gui/editor/rpleditor.ui
    ./src/gui/editor/notamviewer.ui
    ./src/gui/editor/fpleditor.ui
    ./src/gui/editor/fplviewer.ui
    ./src/gui/widgets/fplwidget.ui
    ./src/gui/widgets/fpleditwidget.ui
    ./src/gui/browser/zonebrowser.ui
    ./src/gui/browser/fplhistorybrowser.ui
    ./src/gui/browser/rplbrowser.ui
    ./src/gui/browser/aftnservicelog.ui
    ./src/gui/browser/notamhistorybrowser.ui
    ./src/gui/browser/fplbrowser.ui
    ./src/gui/browser/globalhistorybrowser.ui
    ./src/gui/browser/notambrowser.ui
    ./src/gui/browser/useractionsbrowser.ui
    ./src/gui/browser/sqltablebrowser.ui
    ./src/gui/browser/fplonfuturebrowser.ui
    ./src/gui/browser/sendtelegrambrowser.ui
    ./src/gui/browser/receivedtelegrambrowser.ui
    ./src/main/mainwindow.ui 
	
    ./src/telegram/pages/ArrWidget.ui
    ./src/telegram/pages/ChgWidget.ui
    ./src/telegram/pages/CnlWidget.ui
    ./src/telegram/pages/DepWidget.ui
    ./src/telegram/pages/DlaWidget.ui
    ./src/telegram/pages/FplTelegramWidget.ui
    ./src/telegram/telegram/CreateTelegramWizard.ui
    ./src/telegram/getmapdialog.ui
    ./src/telegram/addressbook/addressbook.ui
    ./src/telegram/addressbook/abonentdlg.ui
    ./src/telegram/addressbook/autotypeswidget.ui
)
list(APPEND RESOURCES 
    ./etc/config.qrc
    ./src/main/managers/navigateactionsmanager/navigateactionsmanager.qrc
    ./src/main/managers/customactionsmanager/itemviewcustomactionsmanagerhelper.qrc
    ./src/main/managers/printactionsmanager/printactionsmanager.qrc
    ./src/main/managers/editactionsmanager/editactionsmanager.qrc)

# Walk throw file list to include all sources directories
foreach(HEADER_FILE ${Q_HEADERS} ${P_HEADERS} ${HEADERS})
  get_filename_component(HEADER_PATH ${HEADER_FILE} PATH)
  include_directories(${HEADER_PATH})
endforeach(HEADER_FILE)
include_directories(src/)
include_directories(src/main)

# if you use Q_OBJECT
qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})
#qt4_automoc(${Q_SOURCES})
qt4_wrap_ui(UI_HEADERS ${FORMS})
qt4_add_resources(QRC_SOURCES ${RESOURCES})

#create app icon
set(WIN_RESOURCES)
if(MINGW AND WIN32)
  # resource compilation for mingw
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/aftnws.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/aftnws.rc)

  list(APPEND WIN_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/aftnws.rc)
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
  ${WIN_RESOURCES})

################################################
# LINKING
################################################
#rename target
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_debug" VERSION ${VERSION})

# main library
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES}
  qtxmlsettings
  qtcsvparser
  qttoolbardialog
  qtmenuview
  qtwindowmenu
  aftnmessageprocessor
  log4qt
  qtdelegates
  qtlanguagemenu
  qtautotransaction
  )
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
  COMPONENT Runtime
)

# ------------------------------------------------------------------------------
# This CMake code installs the needed support libraries on NON OSX platforms
# ------------------------------------------------------------------------------
include(InstallRequiredSystemLibraries)

# ---------- Find/Install the needed Qt4 libraries.
SET (QTLIBLIST QtCore QtGui QtXml QtNetwork QtSql)
if(NOT Q_WS_MAC)
  FOREACH(qtlib ${QTLIBLIST})
    IF (WIN32)
      INSTALL(FILES ${QT_BINARY_DIR}/${qtlib}d4.dll
          DESTINATION bin
          CONFIGURATIONS Debug
          COMPONENT Runtime)
      INSTALL(FILES ${QT_BINARY_DIR}/${qtlib}4.dll
          DESTINATION bin
          CONFIGURATIONS Release
          COMPONENT Runtime)  
    endif (WIN32)
  ENDFOREACH(qtlib)
  if (MINGW)
      find_file(MINGW_DLL mingwm10.dll)
      find_file(MINGW_GCC_DLL libgcc_s_dw2-1.dll)
      install(FILES ${MINGW_DLL} ${MINGW_GCC_DLL} DESTINATION bin COMPONENT Runtime)
  else (MINGW)
      INCLUDE(InstallRequiredSystemLibraries)
  endif (MINGW)
endif(NOT Q_WS_MAC)


## project files
install(DIRECTORY etc DESTINATION . COMPONENT Runtime PATTERN "CVS" EXCLUDE
    PATTERN "*.qrc" EXCLUDE
    PATTERN ".svn" EXCLUDE
    PATTERN "*.rc" EXCLUDE)
install(DIRECTORY share DESTINATION . COMPONENT Runtime PATTERN "CVS" EXCLUDE
  PATTERN "*.rc" EXCLUDE
  PATTERN "*.qrc" EXCLUDE
  PATTERN ".cvsignore" EXCLUDE
  PATTERN ".svn" EXCLUDE
  PATTERN "*.qrc" EXCLUDE)
install(DIRECTORY lib DESTINATION . COMPONENT Runtime
    PATTERN "CVS" EXCLUDE
    PATTERN ".cvsignore" EXCLUDE
    PATTERN ".svn" EXCLUDE
    PATTERN "*.qrc" EXCLUDE)

## dependencies
if (WIN32)
  ## ORACLE dependencies
  find_file(QT_OCI_SQL_PLUGIN qsqloci4.dll HINTS ${QT_PLUGINS_DIR}/sqldrivers)
  if (QT_OCI_SQL_PLUGIN)
    install(FILES ${QT_OCI_SQL_PLUGIN} DESTINATION bin/sqldrivers COMPONENT Runtime)
  endif (QT_OCI_SQL_PLUGIN)

  # UNCOMMENT IF CUSTOMER DONT ISNTALL ORACLE CLIENT
  #find_file(OCI_DLL oci.dll)
  #if (OCI_DLL)
  #  install(FILES ${OCI_DLL} DESTINATION bin COMPONENT Runtime)
  #endif (OCI_DLL)
  #find_file(OCIW32_DLL ociw32.dll)
  #if (OCIW32_DLL)
  #  install(FILES ${OCIW32_DLL} DESTINATION bin COMPONENT Runtime)
  #endif (OCIW32_DLL)
  #find_file(ORAOCIEI_DLL oraociei10.dll)
  #if (ORAOCIEI_DLL)
  #  install(FILES ${ORAOCIEI_DLL} DESTINATION bin COMPONENT Runtime)
  #endif (ORAOCIEI_DLL)

  # NOT USING BY QTSQL OCI PLUGIN
  #find_file(ORAOCCI_DLL ociw32.dll)
  #if (ORAOCCI_DLL)
  #  install(FILES ${ORAOCCI_DLL} DESTINATION bin COMPONENT Runtime)
  #endif (ORAOCCI_DLL)
  #find_file(ORANNZSBB_DLL orannzsbb10.dll)
  #if (ORANNZSBB_DLL)
  #  install(FILES ${ORANNZSBB_DLL} DESTINATION bin COMPONENT Runtime)
  #endif (ORANNZSBB_DLL)
endif (WIN32)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "peleng.ru")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Aftn planning")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/cpack/readme.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/cpack/copyright.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/cpack/welcome.txt")
set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "AFTN Planning Server")

set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_SOURCE_IGNORE_FILES
  "/CVS/"
  "/Build/"
  "/\\\\.hg/"
  "/\\\\.svn/"
  "\\\\.ncb$")

set(CPACK_COMPONENTS_ALL Runtime)

if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/share/images/32x32/application\\\\airplane.ico")
  set(CPACK_NSIS_MUI_UNICON "${CMAKE_SOURCE_DIR}/share/images/32x32/application\\\\airplane.ico")
  set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/share/images/32x32/application\\\\airplane.png")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\aftnws.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} AFTN Planning Server")
  set(CPACK_NSIS_MODIFY_PATH ON)

  set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
      CreateShortCut \\\"$DESKTOP\\\\AFTN Planning Server.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
      CreateShortCut \\\"$SMSTARTUP\\\\AFTN Planning Server.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
  ")

  set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
      Delete \\\"$DESKTOP\\\\AFTN Planning Server.lnk\\\"
      Delete \\\"$SMSTARTUP\\\\AFTN Planning Server.lnk\\\"
  ")


  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.peleng.ru")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.peleng.ru")

  #set(CPACK_NSIS_CONTACT "admin@peleng.ru")

endif(WIN32 AND NOT UNIX)

# This must always be last!
include(CPack)

add_custom_target(cpack
    COMMAND cpack
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  message("Command to create installator package: make (mingw32-make) cpack")

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

