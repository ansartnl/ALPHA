cmake_minimum_required(VERSION 2.6)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# ------------------------------------------------------------
# CMake setup
# ------------------------------------------------------------

# Declare project
project(environmenteditor)

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
# Create libraries list
include(${QT_USE_FILE})
# -----------------------------------------------------
# 3rd party
# -----------------------------------------------------

# all 3rd party subdirectories ---------------------------------------------------------
add_subdirectory(../common/qtxmlsettings ${CMAKE_CURRENT_BINARY_DIR}/qtxmlsettings)
include_directories(../common/qtxmlsettings)

add_subdirectory(../common/qtlanguagemenu ${CMAKE_CURRENT_BINARY_DIR}/qtlanguagemenu)
include_directories(../common/qtlanguagemenu)

add_subdirectory(../common/qtautotransaction ${CMAKE_CURRENT_BINARY_DIR}/qtautotransaction)
include_directories(../common/qtautotransaction)

include(../common/initmain/initmain.cmake)

include(../common/qtdatabaseconnectiondialog/qtdatabaseconnectiondialog.cmake)

include(../common/geo/geo.cmake)

include(../common/waitcursor/waitcursor.cmake)

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

# project version
set (VERSION_MAJOR 2)
set (VERSION_MINOR 0)
set (VERSION_RELEASE 4)
set (VERSION_BUILD 0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}" )

add_definitions(-DVERSION_STR="${VERSION}")
add_definitions(-DAPP_VERSION="${VERSION}")

set(COMPANY_NAME "Peleng Ltd.")
set(FILE_DESCRIPTION "${PROJECT_NAME}")
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}.${VERSION_BUILD}")
set(LEGAL_COPYRIGHT "All rights reserved")
set(PRODUCT_NAME "${PROJECT_NAME}")
set(OUTPUT_NAME "${PROJECT_NAME}.exe")

# Get project sources
list(APPEND SOURCES src/main.cpp
    src/arinc/parser/ArincParser.cpp
    src/arinc/storage/ArincEmptyStorage.cpp
    src/arinc/storage/ArincNativeStorage.cpp
    src/arinc/structures/BoundaryPoint.cpp
    src/arinc/structures/FirUir.cpp
    src/arinc/structures/UpperLowerLimit.cpp
    src/arinc/structures/ControlledAirspace.cpp
    src/arinc/structures/ContinuantionNumber.cpp
    src/arinc/structures/RestrictedAirspace.cpp
    src/arinc/structures/Airspace.cpp
    src/arinc/structures/VhfNavaid.cpp
    src/arinc/structures/NdbNavaid.cpp
    src/arinc/structures/Navaid.cpp
    src/arinc/structures/Enroute.cpp
    src/arinc/structures/Airway.cpp
    src/arinc/structures/Holding.cpp
    src/arinc/structures/Airport.cpp
    src/arinc/structures/AirportRefPoint.cpp
    src/arinc/iterator/ArincFilePrimitiveIterator.cpp
    src/arinc/iterator/ArincFilePosIterator.cpp
    src/arinc/structures/EnrouteWaypoint.cpp
    src/arinc/structures/Waypoint.cpp
    src/arinc/structures/TerminalWaypoint.cpp
    src/arinc/structures/BoundaryPointFirUir.cpp
    src/arinc/structures/SSA.cpp
    src/arinc/structures/ArincBaseStruct.cpp
    src/arinc/structures/ArincGlobal.cpp
    src/arinc/structures/Runway.cpp
    src/arinc/structures/Ndb.cpp
    src/arinc/structures/NdbTerminal.cpp
    src/arinc/parser/ArincParserDialog.cpp
    src/arinc/parser/ArincParserThread.cpp
    src/arinc/storage/ArincStatusStorage.cpp
    src/arincmergemainwindow.cpp
    src/editors/viewdelegate.cpp
    src/editors/ssaeditor.cpp
    src/editors/routeeditor.cpp
    src/editors/restricteditor.cpp
    src/editors/restrictusereditor.cpp
    src/editors/pointeditor.cpp
    src/editors/holdingeditor.cpp
    src/editors/fireditor.cpp
    src/editors/dbscheme.cpp
    src/editors/airporteditor.cpp
    src/editors/sectoreditor.cpp
    src/editors/baseeditor.cpp
    src/dialogs/importarincdialog.cpp
    src/database/ArincDatabaseImporter.cpp
    src/database/ArincDatabaseImporterDialog.cpp
    src/database/ArincDatabaseImporterDC.cpp
    src/database/FirBoundaryReplacer.cpp
    src/database/OldMasterSupport.cpp
    src/common/widgetsaver.cpp
    src/common/DeferredCallThread.cpp
    src/common/misc.cpp
  )

list(APPEND Q_HEADERS
    src/arinc/iterator/ArincFilePosIterator.h
    src/arinc/parser/ArincParserDialog.h
    src/arinc/parser/ArincParserThread.h
    src/arinc/storage/ArincStatusStorage.h
    src/arincmergemainwindow.h
    src/editors/viewdelegate.h
    src/editors/ssaeditor.h
    src/editors/routeeditor.h
    src/editors/restricteditor.h
    src/editors/restrictusereditor.h
    src/editors/pointeditor.h
    src/editors/holdingeditor.h
    src/editors/fireditor.h
    src/editors/airporteditor.h
    src/editors/baseeditor.h
    src/dialogs/importarincdialog.h
    src/database/ArincDatabaseImporter.h
    src/database/ArincDatabaseImporterDialog.h
    src/editors/sectoreditor.h
  )

list(APPEND HEADERS
    src/arinc/parser/ArincParser.h
    src/arinc/storage/ArincEmptyStorage.h
    src/arinc/storage/ArincNativeStorage.h
    src/arinc/structures/BoundaryPoint.h
    src/arinc/structures/FirUir.h
    src/arinc/structures/UpperLowerLimit.h
    src/arinc/structures/ControlledAirspace.h
    src/arinc/structures/ContinuantionNumber.h
    src/arinc/structures/RestrictedAirspace.h
    src/arinc/structures/Airspace.h
    src/arinc/structures/VhfNavaid.h
    src/arinc/structures/NdbNavaid.h
    src/arinc/structures/Navaid.h
    src/arinc/structures/Enroute.h
    src/arinc/structures/Airway.h
    src/arinc/structures/Holding.h
    src/arinc/structures/Airport.h
    src/arinc/structures/AirportRefPoint.h
    src/arinc/iterator/ArincFilePrimitiveIterator.h
    src/arinc/structures/EnrouteWaypoint.h
    src/arinc/structures/Waypoint.h
    src/arinc/structures/TerminalWaypoint.h
    src/arinc/structures/BoundaryPointFirUir.h
    src/arinc/structures/SSA.h
    src/arinc/structures/ArincBaseStruct.h
    src/arinc/structures/ArincGlobal.h
    src/arinc/structures/Runway.h
    src/arinc/structures/Ndb.h
    src/arinc/structures/NdbTerminal.h
    src/editors/editors.h
    src/editors/dbscheme.h
    src/editors/RefreshInterface.h
    src/database/ArincDatabaseImporterDC.h
    src/database/FirBoundaryReplacer.h
    src/database/OldMasterSupport.h
    src/common/widgetsaver.h
    src/common/DeferredProcedure.h
    src/common/DeferredCallThread.h
    src/common/misc.h
  )

list(APPEND FORMS
    src/arinc/parser/ArincParserDialog.ui
    src/arincmergemainwindow.ui
    src/editors/ssaeditor.ui
    src/editors/routeeditor.ui
    src/editors/restricteditor.ui
    src/editors/restrictusereditor.ui
    src/editors/pointeditor.ui
    src/editors/holdingeditor.ui
    src/editors/fireditor.ui
    src/editors/airporteditor.ui
    src/dialogs/importarincdialog.ui
    src/database/ArincDatabaseImporterDialog.ui
    src/editors/sectoreditor.ui
  )

list(APPEND RESOURCES)

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
  # resource for mingw win32
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/environmenteditor.rc.in ${CMAKE_CURRENT_BINARY_DIR}/environmenteditor.rc)
  list(APPEND WIN_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/environmenteditor.rc)
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

install(DIRECTORY doc DESTINATION . COMPONENT Documentation
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
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "AFTN applications")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/cpack/readme.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/cpack/copyright.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/cpack/welcome.txt")
set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "Environment Editor")

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
  set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/share/images/32x32\\\\merge1.ico")
  set(CPACK_NSIS_MUI_UNICON "${CMAKE_SOURCE_DIR}/share/images/32x32\\\\merge1.ico")
  set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/share/images/32x32\\\\merge1.ico")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${PROJECT_NAME}.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} ${PROJECT_NAME}")
  set(CPACK_NSIS_MODIFY_PATH ON)

  set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
      CreateShortCut \\\"$DESKTOP\\\\Environment Editor.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
      CreateShortCut \\\"$SMSTARTUP\\\\Environment Editor.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
  ")

  set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
      Delete \\\"$DESKTOP\\\\Environment Editor.lnk\\\"
      Delete \\\"$SMSTARTUP\\\\Environment Editor.lnk\\\"
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

