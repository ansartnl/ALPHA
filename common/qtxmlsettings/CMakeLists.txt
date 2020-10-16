cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project
project(qtxmlsettings)

set(QT_DONT_USE_QTGUI TRUE)

include(../cmake/version-dep-commands.cmake)
find_qt(Xml)

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

# SOURCE FILES AND DIRECTORIES

# simple sources
set(SOURCES qtxmlsettings.cpp)

# simple headers
set(HEADERS qtxmlsettings.h)

# if you use Q_OBJECT
qt45_wrap_cpp(MOC_SOURCES ${HEADERS})

# COMPILATION
add_library(${PROJECT_NAME} STATIC
  ${SOURCES}
  ${MOC_SOURCES}
  ${HEADERS}
  )

# LINKING
# main library
link_qt(${PROJECT_NAME})
