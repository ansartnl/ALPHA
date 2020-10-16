cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project
project(accord)

find_package(Qt4 REQUIRED)
set(QT_DONT_USE_QTGUI TRUE)
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

# project sources
list(APPEND SOURCES 
		src/accordproto.cpp
		src/accorderrors.cpp
		src/codogramplane.cpp
		src/codogram0.cpp
		src/codogram1.cpp
		src/codogram2.cpp
		src/codogram3.cpp
		src/codogram4.cpp
		src/codogram5.cpp
		src/codogram6to9.cpp
		src/codogram10.cpp
		src/codogram11.cpp
		src/accord.cpp
	)

# project headers
list(APPEND HEADERS 
		src/accordproto.h
		src/accordexport.h
		src/accordutility.h
		src/accorderrors.h
		src/codogramplane.h
		src/codogram0.h
		src/codogram1.h
		src/codogram2.h
		src/codogram3.h
		src/codogram4.h
		src/codogram5.h
		src/codogram6to9.h
		src/codogram10.h
		src/codogram11.h
		src/accord.h
	)

include_directories(src/)

# if you use Q_OBJECT
qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})

# COMPILATION
add_definitions(-DACCORD_LIBRARY)
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
