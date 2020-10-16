cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project
project(asterix)

set(QT_DONT_USE_QTGUI TRUE)

include(../../cmake/version-dep-commands.cmake)
find_qt(Core)

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
list(APPEND SOURCES src/asterixdata.cpp
    src/asterixerrors.cpp
	src/asterix.cpp
    src/asterix001.cpp
    src/asterix002.cpp
    src/asterix004.cpp
    src/asterix008.cpp
    src/asterix020.cpp
    src/asterix021.cpp
    src/asterix034.cpp
    src/asterix048.cpp
    src/asterix062.cpp
    src/asterix063.cpp
# Eurocontrol standard
    src/eurocontrol/asterix020euro.cpp
    src/eurocontrol/asterix253euro.cpp
# Aerotechnica standard
    src/aerotechnica/asterix001aero.cpp
    src/aerotechnica/asterix002aero.cpp
    src/aerotechnica/asterix062aero.cpp
    src/aerotechnica/asterix250aero.cpp
    src/aerotechnica/asterix253aero.cpp
    src/aerotechnica/asterix254aero.cpp
# Peleng standard
    src/peleng/asterix241peleng.cpp
    src/peleng/asterix062peleng.cpp
    src/peleng/asterix253peleng.cpp
    )

# simple headers
list(APPEND HEADERS src/asterixdata.h
    src/asterixexport.h
    src/asterixerrors.h
	src/asterix.h
    src/asterix001.h
    src/asterix002.h
    src/asterix004.h
    src/asterix008.h
    src/asterix020.h
    src/asterix021.h
    src/asterix034.h
    src/asterix048.h
    src/asterix062.h
    src/asterix063.h
# Eurocontrol standard
    src/eurocontrol/asterix001euro.h
    src/eurocontrol/asterix002euro.h
    src/eurocontrol/asterix004euro.h
    src/eurocontrol/asterix008euro.h
    src/eurocontrol/asterix020euro.h
    src/eurocontrol/asterix021euro.h
    src/eurocontrol/asterix034euro.h
    src/eurocontrol/asterix048euro.h
    src/eurocontrol/asterix062euro.h
    src/eurocontrol/asterix063euro.h
    src/eurocontrol/asterix253euro.h
# Aerotechnica standard
    src/aerotechnica/asterix001aero.h
    src/aerotechnica/asterix002aero.h
    src/aerotechnica/asterix062aero.h
    src/aerotechnica/asterix250aero.h
    src/aerotechnica/asterix253aero.h
    src/aerotechnica/asterix254aero.h
# Peleng standard
    src/peleng/asterix241peleng.h
    src/peleng/asterix062peleng.h
    src/peleng/asterix253peleng.h
    )

include_directories(src/)

# if you use Q_OBJECT
qt45_wrap_cpp(MOC_SOURCES ${Q_HEADERS})

# COMPILATION
add_definitions(-DASTERIX_LIBRARY)
add_library(${PROJECT_NAME} SHARED
  ${SOURCES}
  ${MOC_SOURCES}
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
