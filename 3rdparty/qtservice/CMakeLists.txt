cmake_minimum_required(VERSION 2.8.9)

project(QtService)

message("Building QtService solution")

include(cmake/output-directories.cmake)
include(cmake/compiler-flags.cmake)
include(cmake/set-version.cmake)
include(cmake/using-preset-defaults.cmake)
include(cmake/sources-management.cmake)
include(cmake/version-dep-commands.cmake)
include(cmake/copy-files.cmake)

option(QTSERVICE_STATIC "Build QtService statically" OFF)

if(NOT QTSERVICE_SUBPROJECT)
	declare_output_directories()
	set_multithreaded_build()
	set_cxx11_build()
	set_warnings_as_errors_build()
	set_default_definitions()
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	set_debug_postfix("d")
endif(NOT QTSERVICE_SUBPROJECT)

find_qt(Core Network)

if(WIN32)
	add_definitions(-D_UNICODE)
	if(NOT QTSERVICE_STATIC)
		add_definitions(-DQT_QTSERVICE_EXPORT)
	endif(NOT QTSERVICE_STATIC)
endif(WIN32)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_BINARY_DIR}/src
)

set(HEADERS
	src/qtservice.h
	src/QtServiceBase
	src/QtServiceController
)

set(HEADERS_DETAIL
	src/qtservice_p.h
)

if(UNIX)
	set(HEADERS_DETAIL
		${HEADERS_DETAIL}
		src/qtunixserversocket.h
		src/qtunixsocket.h
	)
endif(UNIX)

set(SOURCES
	src/qtservice.cpp
)

if(WIN32)
	set(SOURCES
		${SOURCES}
		src/qtservice_win.cpp
	)
endif(WIN32)
if(UNIX)
	set(SOURCES
		${SOURCES}
		src/qtservice_unix.cpp
		src/qtunixserversocket.cpp
		src/qtunixsocket.cpp
	)
endif(UNIX)

source_group("sources" FILES
	${HEADERS}
	${HEADERS_DETAIL}
	${SOURCES}
)

set(BUILD_TYPE SHARED)
if(QTSERVICE_STATIC)
	set(BUILD_TYPE STATIC)
endif(QTSERVICE_STATIC)


add_library(QtService ${BUILD_TYPE}
	${HEADERS}
	${HEADERS_DETAIL}
	${SOURCES}
)

set_property(TARGET QtService PROPERTY FOLDER "3rdparty")

link_qt(QtService)
configure_build(QtService)
set_output_directories(QtService)

copy_files(QtService ${INCLUDE_INSTALLATION_PATH}/QtService ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS})
