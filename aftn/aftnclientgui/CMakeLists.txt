cmake_minimum_required(VERSION 2.6)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# ------------------------------------------------------------
# CMake setup
# ------------------------------------------------------------
# Variable for project name, used in many places
set (PROJECT aftnclient)

# Declare project
project(${PROJECT})

find_package(Qt4 REQUIRED )

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

# SOURCE FILES AND DIRECTORIES

# sources without #include "*.moc"
set(SOURCES)
# sources with #include "*.moc", for automoc
set(Q_SOURCES)
# headers without Q_OBJECT macro, or headers with exists #include "*.moc" in cpp file
set(HEADERS)
# header with Q_OBJECT macro
set(Q_HEADERS)
#header contains private classes
set(P_HEADERS)
# application forms *.ui
set(FORMS)
# application resources
set(RESOURCES)
# windows resources
set(WIN_RESOURCES)

# generated source code from FORMS
set(UI_HEADERS)
# generated moc_*.cxx from Q_HEADERS
set(MOC_SOURCES)
# generated qtc_*.cxx from RESOURCES
set(QRC_SOURCES)

# project version
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_RELEASE 0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}" )

# SOURCE FILES AND DIRECTORIES
# ------------------------------------------------------------------
# Setup Qt
set(QT_USE_QTNETWORK    TRUE)

# Create libraries list
include(${QT_USE_FILE})

# Get project sources

list(APPEND SOURCES
    ./src/main/qtlogevent.cpp
    ./src/main/main.cpp
    ./src/main/mainwindow.cpp
)

list(APPEND Q_HEADERS
    ./src/main/qtlogevent.h
    ./src/main/mainwindow.h
    ./src/main/main.h
)

list(APPEND FORMS 
    ./src/main/mainwindow.ui
)

# Walk throw file list to include all sources directories
foreach(HEADER_FILE ${Q_HEADERS} ${HEADERS})
  get_filename_component(HEADER_PATH ${HEADER_FILE} PATH)
  include_directories(${HEADER_PATH})
endforeach(HEADER_FILE)


# -----------------------------------------------------
# 3rd party
# -----------------------------------------------------
add_subdirectory(../../common/qtxmlsettings ${CMAKE_CURRENT_BINARY_DIR}/qtxmlsettings)
include_directories(../../common/qtxmlsettings)

add_subdirectory(../aftnclient ${CMAKE_CURRENT_BINARY_DIR}/aftnclient)
include_directories(../aftnclient/src/main)
# -----------------------------------------------------


# if you use Q_OBJECT
qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})
qt4_automoc(${Q_SOURCES})
qt4_wrap_ui(UI_HEADERS ${FORMS})
qt4_add_resources(QRC_SOURCES ${RESOURCES})

# COMPILATION
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
add_executable(${PROJECT_NAME} WIN32
  ${SOURCES}
  ${HEADERS}
  ${Q_HEADERS}
  ${MOC_SOURCES}
  ${QRC_SOURCES}
  ${UI_HEADERS}
  ${QS_SOURCES}
  ${WIN_RESOURCES})

# LINKING
# main library
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} qtxmlsettings)

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
INCLUDE (InstallRequiredSystemLibraries)

# ---------- Find/Install the needed Qt4 libraries.
SET (QTLIBLIST QtCore QtGui QtXml QtNetwork)
IF(NOT Q_WS_MAC)
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
    ENDIF (WIN32)
  ENDFOREACH(qtlib)
  if (MINGW)
      find_file(MINGW_DLL mingwm10.dll)
      find_file(MINGW_GCC_DLL libgcc_s_dw2-1.dll)
      install(FILES ${MINGW_DLL} ${MINGW_GCC_DLL} DESTINATION bin COMPONENT Runtime)
  else (MINGW)
      INCLUDE(InstallRequiredSystemLibraries)
  endif (MINGW)
ENDIF(NOT Q_WS_MAC)

install(DIRECTORY etc DESTINATION . COMPONENT Runtime)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "itos.eu.org")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TCP/IP client for aftn servers")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_COMPONENTS_ALL Runtime)

# This must always be last!
include(CPack)

########################################
## Uninstalling
########################################

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")