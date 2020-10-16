cmake_minimum_required(VERSION 2.8.8)

project(Utilities)

option(UTILITIES_STATIC_BUILD "Build Utilities statically" OFF)

include(cmake/version-dep-commands.cmake)

find_qt(Core Widgets Xml)

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

if(UTILITIES_STATIC_BUILD)
	add_definitions(-DUTILITIES_STATIC)
else(UTILITIES_STATIC_BUILD)
	add_definitions(-DUTILITIES_LIBRARY)
endif(UTILITIES_STATIC_BUILD)


set(HEADERS
	file_save_guard.h
	multiparametric.h
	performance_meter.h
	singleton.h
	translatable.h
	translation.h
	utilities_global.h
	utils.h
	xml_storage.h
)

set(SOURCES
	file_save_guard.cpp
	performance_meter.cpp
	translatable.cpp
	translation.cpp
	utils.cpp
	xml_storage.cpp
)

source_group(sources FILES
	${HEADERS}
	${SOURCES}
)

set(BUILD_TYPE SHARED)
if(UTILITIES_STATIC_BUILD)
	set(BUILD_TYPE STATIC)
endif(UTILITIES_STATIC_BUILD)

add_library(Utilities ${BUILD_TYPE}
	${HEADERS}
	${SOURCES}
)	

link_qt(Utilities)

if(UNIX OR MINGW)
	list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g")
endif(UNIX OR MINGW)

set_property(TARGET Utilities PROPERTY FOLDER "Common")
set_target_properties(Utilities PROPERTIES DEBUG_POSTFIX "_debug")

if(WIN32)
	set_target_properties(Utilities PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIB_INSTALLATION_PATH}
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIB_INSTALLATION_PATH}
		LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_INSTALLATION_PATH}
		LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_INSTALLATION_PATH}
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_INSTALLATION_PATH}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_INSTALLATION_PATH}
	)
else(WIN32)
	set_target_properties(Utilities PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${LIB_INSTALLATION_PATH}
		LIBRARY_OUTPUT_DIRECTORY ${LIB_INSTALLATION_PATH}
		RUNTIME_OUTPUT_DIRECTORY ${BIN_INSTALLATION_PATH}
	)
endif(WIN32)

add_custom_command(TARGET Utilities
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${INCLUDE_INSTALLATION_PATH}/Utilities
	COMMENT "creating directory ... ${INCLUDE_INSTALLATION_PATH}/Utilities"
)

foreach(header ${HEADERS})
	add_custom_command(TARGET Utilities
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${header} ${INCLUDE_INSTALLATION_PATH}/Utilities
		COMMENT "copying ${header} to ... ${INCLUDE_INSTALLATION_PATH}/Utilities"
	)
endforeach(header)
