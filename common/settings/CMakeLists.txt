cmake_minimum_required(VERSION 2.8.8)

project(Settings)

option(SETTINGS_STATIC_BUILD "Build Settings statically" OFF)

include(cmake/version-dep-commands.cmake)

find_qt(Core Xml)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(
	-DNOMINMAX 
	-D_CRT_SECURE_NO_WARNINGS 
	-D_CRT_NONSTDC_NO_WARNINGS 
	-DSILENT
)

if(SETTINGS_STATIC_BUILD)
	add_definitions(-DSETTINGS_STATIC)
else(SETTINGS_STATIC_BUILD)
	add_definitions(-DSETTINGS_LIBRARY)
endif(SETTINGS_STATIC_BUILD)


set(HEADERS
	memory_storage.h
	settings.h
	settings_global.h
	storage_interface.h
	registry_storage.h
	xml_storage.h
)

set(SOURCES
	memory_storage.cpp
	registry_storage.cpp
	settings.cpp
	xml_storage.cpp
)

source_group(sources FILES
	${HEADERS}
	${SOURCES}
)

set(BUILD_TYPE SHARED)
if(SETTINGS_STATIC_BUILD)
	set(BUILD_TYPE STATIC)
endif(SETTINGS_STATIC_BUILD)

add_library(Settings ${BUILD_TYPE}
	${HEADERS}
	${SOURCES}
)

add_dependencies(Settings
	Utilities
)

if(UNIX OR MINGW)
	list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g")
endif(UNIX OR MINGW)

link_qt(Settings)

set_target_properties(Settings PROPERTIES DEBUG_POSTFIX "_debug")
set_property(TARGET Settings PROPERTY FOLDER "Common")

if(MINGW AND NOT SETTINGS_STATIC_BUILD)
	target_link_libraries(Settings
		debug ${CMAKE_SOURCE_DIR}/bin/libUtilities_debug.dll optimized ${CMAKE_SOURCE_DIR}/bin/libUtilities.dll
	)
else(MINGW AND NOT SETTINGS_STATIC_BUILD)
	target_link_libraries(Settings
		debug Utilities_debug optimized Utilities
	)
endif(MINGW AND NOT SETTINGS_STATIC_BUILD)


if(WIN32)
	set_target_properties(Settings PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIB_INSTALLATION_PATH}
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIB_INSTALLATION_PATH}
		LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_INSTALLATION_PATH}
		LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_INSTALLATION_PATH}
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_INSTALLATION_PATH}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_INSTALLATION_PATH}
	)
else(WIN32)
	set_target_properties(Settings PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${LIB_INSTALLATION_PATH}
		LIBRARY_OUTPUT_DIRECTORY ${LIB_INSTALLATION_PATH}
		RUNTIME_OUTPUT_DIRECTORY ${BIN_INSTALLATION_PATH}
	)
endif(WIN32)

add_custom_command(TARGET Settings
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${INCLUDE_INSTALLATION_PATH}/Settings
	COMMENT "creating directory ... ${INCLUDE_INSTALLATION_PATH}/Settings"
)

foreach(header ${HEADERS})
	add_custom_command(TARGET Settings
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${header} ${INCLUDE_INSTALLATION_PATH}/Settings
		COMMENT "copying ${header} to ... ${INCLUDE_INSTALLATION_PATH}/Settings"
	)
endforeach(header)
