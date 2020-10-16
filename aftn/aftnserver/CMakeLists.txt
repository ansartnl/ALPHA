cmake_minimum_required(VERSION 2.6)


# ------------------------------------------------------------
# CMake setup
# ------------------------------------------------------------
# Declare project
project(aftnserver)

find_package(Qt4 REQUIRED)

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
# project version
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_RELEASE 0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}" )


# SOURCE FILES AND DIRECTORIES
# ------------------------------------------------------------------
# Setup Qt
# set(QT_USE_QTXML        TRUE)
set(QT_USE_QTNETWORK    TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# For example
# using qt script
# set(QT_USE_QTSCRIPT TRUE)
# SET DONT USING Qt MODULES
set(QT_DONT_USE_QTGUI TRUE)
# SET USING Qt MODULES
#set(QT_USE_QTSQL TRUE)

# Create libraries list
include(${QT_USE_FILE})

# Get project sources
  # Get project sources
SET(SOURCES src/main/serverthread.cpp
    src/main/main.cpp
    src/main/aftnserver.cpp)
set(Q_HEADERS src/main/aftnserver.h
    src/main/serverthread.h
    src/main/aftnproto.h)


# -----------------------------------------------------
# 3rd party
# -----------------------------------------------------
add_subdirectory(../../common/qtxmlsettings ${CMAKE_BINARY_DIR}/qtxmlsettings)
include_directories(../../common/qtxmlsettings)
# -----------------------------------------------------
#list(APPEND Q_HEADERS src/main/ser

# if you use Q_OBJECT
qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})
qt4_automoc(${Q_SOURCES})
qt4_wrap_ui(UI_HEADERS ${FORMS})
qt4_add_resources(QRC_SOURCES ${RESOURCES})

# COMPILATION
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
add_executable(${PROJECT_NAME} WIN32
  ${SOURCES}
  ${Q_SOURCES}
  ${MOC_SOURCES}
  ${HEADERS}
  ${Q_HEADERS})

# LINKING
# main library
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} qtxmlsettings)
