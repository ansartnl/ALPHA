cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project
project(CoordSys)

add_definitions(-DCOORDSYS_LIBRARY)

include_directories(src/)

find_package(Qt4 REQUIRED)
set(QT_DONTUSE_QTGUI TRUE)
include(${QT_USE_FILE})

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
list(APPEND SOURCES 
    src/cs/CoordSystem.cpp
    src/cs/CoordSystemGK.cpp
    src/cs/CoordSystemStereo.cpp
    src/cs/Coordinate.cpp
    )

# simple headers
list(APPEND HEADERS
    src/coordsys_global.h
    src/cs/GeoCS.h
    src/cs/GaussKrugerCS.h
    src/cs/StereoCS.h
    src/cs/CoordSystem.h
    src/cs/CoordSystemGK.h
    src/cs/CoordSystemStereo.h
    src/cs/Coordinate.h
    )

# if you use Q_OBJECT
qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})

# COMPILATION
add_library(${PROJECT_NAME} SHARED
  ${SOURCES}
  ${MOC_SOURCES}
  ${HEADERS}
  )

if(WIN32)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "")
endif()

# LINKING
# main library
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
