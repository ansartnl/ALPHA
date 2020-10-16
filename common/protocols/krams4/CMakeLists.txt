cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project
project(krams4)

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
include(../../enumstreaming/enumstreaming.cmake)

# simple sources
list(APPEND SOURCES src/kramsproto.cpp
    src/kramserrors.cpp
	src/krams.cpp
    src/krams4.cpp
    src/kramsping.cpp
    src/kramstickets.cpp
    src/krams4dump.cpp
    src/krams4metarspeci.cpp
    src/MetarSpeci.cpp
    src/MetarSpeciParser.cpp
    )

# simple headers
list(APPEND HEADERS src/kramsproto.h
    src/kramsexport.h
    src/kramserrors.h
	src/krams.h
    src/krams4.h
    src/kramsping.h
    src/kramstickets.h
    src/krams4dump.h
    src/krams4metarspeci.h
    src/MetarSpeci.h
    src/MetarSpeciParser.h
    )

include_directories(src/)

# if you use Q_OBJECT
qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})

# COMPILATION
add_definitions(-DKRAMS_LIBRARY)
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
