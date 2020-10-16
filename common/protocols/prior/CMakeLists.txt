cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project
project(prior)

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

# common
include(../../factory/factory.cmake)
include(../share/share.cmake)

# simple sources
list(APPEND SOURCES src/priordata.cpp
    src/priorerrors.cpp
	src/prior.cpp
    src/prior001.cpp
    src/prior002.cpp
    src/prior008.cpp
    )

# simple headers
list(APPEND HEADERS src/priordata.h
    src/priorexport.h
    src/priorerrors.h
	src/prior.h
    src/prior001.h
    src/prior002.h
    src/prior008.h
    )

include_directories(src/)

# if you use Q_OBJECT
qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})

# COMPILATION
add_definitions(-DPRIOR_LIBRARY)
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
