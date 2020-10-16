cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project
project(XtProto)

set(QT_DONT_USE_QTGUI TRUE)

include(../../cmake/version-dep-commands.cmake)
find_qt(Xml)

include_directories(src)

add_definitions(-DXTPROTO_LIBRARY)

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
    src/XtMessageImpl.cpp
    src/XtObjectImpl.cpp
    src/XtCommon.cpp
    src/XtProtocol.cpp
    src/XtAsterix62.cpp
    src/XtPlotTrack.cpp
    src/XtObject.cpp
    src/XtPeleng.cpp
    src/XtRadar.cpp
    src/XtMeteo.cpp
    src/XtAirplaneInfo.cpp
    src/XtSimCmd.cpp
    src/XtStand.cpp
    src/XtVehicle.cpp
	)

# simple headers
list(APPEND HEADERS
    src/XtAsterix62.h
    src/XtAirplaneInfo.h
    src/XtMeteo.h
    src/XtPeleng.h
    src/XtRadar.h
    src/XtPlotTrack.h
    src/XtDataMessage.h
    src/XtDefines.h
    src/XtGlobal.h
    src/XtCommon.h
    src/XtMasterServer.h
    src/XtMessage.h
    src/XtMRequestAll.h
    src/XtMRequestMaster.h
    src/XtMRequestType.h
    src/XtMSubscribeType.h
    src/XtObject.h
    src/XtProtocol.h
    src/XtRequestMessage.h
    src/XtSubscribeMessage.h
    src/XtTypes.h
    src/XtSimCmd.h
    src/XtStand.h
    src/XtVehicle.h
)

# COMPILATION
add_library(${PROJECT_NAME} SHARED
  ${SOURCES}
  ${HEADERS}
  )

if(WIN32)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "")
endif()

if (MSVC)
#    set(BIN_INSTALLATION_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "path to install binaries" FORCE)    
	
	set_target_properties(${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_INSTALLATION_PATH}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_INSTALLATION_PATH}
		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_INSTALLATION_PATH}
	)
endif()

# LINKING
# main library
link_qt(${PROJECT_NAME})
