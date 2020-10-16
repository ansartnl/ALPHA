cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

set(CMAKE_INCLUDE_CURRENT_DIR true)

# Declare project
project(qtservice)

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

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_VERBOSE_MAKEFILE TRUE)
endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

set(SOURCES
  qtservice.cpp
  )

set(HEADERS
  qtservice.h
  )

#manual moc generating
qt4_generate_moc(qtservice.cpp ${CMAKE_CURRENT_BINARY_DIR}/qtservice.moc)
set_source_files_properties(qtservice.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/qtservice.moc)

if(WIN32)
  
  add_definitions(-DUNICODE)
  list(APPEND SOURCES
    qtservice_win.cpp)
  
  qt4_generate_moc(qtservice_win.cpp ${CMAKE_CURRENT_BINARY_DIR}/qtservice_win.moc)
  set_source_files_properties(qtservice_win.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/qtservice_win.moc)
  
  list(APPEND QT_LIBRARIES user32)
  
else(WIN32)
  
  list(APPEND SOURCES
    qtservice_unix.cpp
    qtunixsocket.cpp
    qtunixserversocket.cpp
    )

  list(APPEND Q_HEADERS
    qtunixsocket.h
    qtunixserversocket.h
    )

  qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})
  
  qt4_generate_moc(qtservice_unix.cpp ${CMAKE_CURRENT_BINARY_DIR}/qtservice_unix.moc )
  set_source_files_properties(qtservice_unix.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/qtservice_unix.moc)
  
  #setup Qt
  set(QT_USE_QTNETWORK TRUE)
endif(WIN32)

# Create libraries list
include(${QT_USE_FILE})

add_library(qtservice STATIC
  ${SOURCES}
  ${HEADERS}
  ${Q_HEADERS}
  ${MOC_SOURCES}
  )

# LINKING
# main library
target_link_libraries(qtservice ${QT_LIBRARIES})
