cmake_minimum_required(VERSION 2.8.9)

project(izmqt)

message("Building izmqt solution")

include(cmake/output-directories.cmake)
include(cmake/compiler-flags.cmake)
include(cmake/set-version.cmake)
include(cmake/using-preset-defaults.cmake)
include(cmake/sources-management.cmake)
include(cmake/version-dep-commands.cmake)

if(NOT IZMQT_SUBPROJECT)
	declare_output_directories()
	set_multithreaded_build()
	set_cxx11_build()
	set_warnings_as_errors_build()
	set_default_definitions()
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	set_debug_postfix("d")
endif()

option(IZMQT_STATIC "Build izmqt statically" OFF)

if(MSVC)
	set(PLATFORM_PREFIX "win32-msvc")
	set(LIB_NAME "zmq.lib")
	set(BIN_NAME "zmq.dll")
elseif(MINGW)
	set(PLATFORM_PREFIX "win32-mingw")
	set(LIB_NAME "libzmq.dll.a")
	set(BIN_NAME "libzmq.dll")
else(UNIX)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(PLATFORM_PREFIX "linux-x64")
	else()
		set(PLATFORM_PREFIX "oracle-linux-6.3-i686")
	endif()
	set(LIB_NAME "libzmq.a")
	set(BIN_NAME "libzmq.so.3.0.0")
endif()

find_qt(Core)

if(IZMQT_STATIC)
	add_definitions(-DIZMQT_STATIC)
else(IZMQT_STATIC)
	add_definitions(-DIZMQT_LIBRARY)
endif(IZMQT_STATIC)


include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/zmq
	${CMAKE_CURRENT_BINARY_DIR}/src
)

link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/lib/${PLATFORM_PREFIX}
)

add_subdirectory(src)
