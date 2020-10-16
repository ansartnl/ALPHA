PROJECT(qjson)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII" )

# Stop cmake 2.6 from whining
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
CMAKE_POLICY(SET CMP0003 NEW)

include(cmake/version-dep-commands.cmake)
include(cmake/copy-files.cmake)

set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

# compile in debug mode
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")
ENDIF(NOT CMAKE_BUILD_TYPE)

IF("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")
  ADD_DEFINITIONS("-DQT_NO_DEBUG_OUTPUT")
ENDIF("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")

# Ability to disable verbose debug output
IF(QJSON_VERBOSE_DEBUG_OUTPUT)
  ADD_DEFINITIONS("-DQJSON_VERBOSE_DEBUG_OUTPUT")
endif(QJSON_VERBOSE_DEBUG_OUTPUT)

OPTION(OSX_FRAMEWORK "Build a Mac OS X Framework")
SET(FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Library/Frameworks"
    CACHE PATH "Where to place qjson.framework if OSX_FRAMEWORK is selected")

# Don't use absolute path in qjson-targets-*.cmake
# (This will have no effect with CMake < 2.8)
SET(QT_USE_IMPORTED_TARGETS TRUE)

# Find Qt
find_qt(Core)

SET (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )

set(QJSON_LIB_MAJOR_VERSION "0")
set(QJSON_LIB_MINOR_VERSION "8")
set(QJSON_LIB_PATCH_VERSION "1")
	
set(QJSON_LIB_VERSION_STRING "${QJSON_LIB_MAJOR_VERSION}.${QJSON_LIB_MINOR_VERSION}.${QJSON_LIB_PATCH_VERSION}")

#add extra search paths for libraries and includes
if(NOT QJSON_NOT_SET_OWN_DIRECTORY)
if(QJSON_SUBPROJECT)
	SET (LIB_INSTALL_DIR "${LIB_INSTALLATION_PATH}" CACHE STRING "Directory where lib will install")
	SET (BIN_INSTALL_DIR "${BIN_INSTALLATION_PATH}" CACHE STRING "Directory where binaries will install")
	SET (INCLUDE_INSTALL_DIR "${INCLUDE_INSTALLATION_PATH}" CACHE PATH "The directory the headers are installed in")
	SET (CMAKECONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/qjson" CACHE PATH "Directory where to install QJSONConfig.cmake")
	
	set (LIBRARY_OUTPUT_PATH "${LIB_INSTALLATION_PATH}" )
else()
	SET (LIB_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib${LIB_SUFFIX}" CACHE STRING "Directory where lib will install")
	SET (BIN_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin" CACHE STRING "Directory where binaries will install")
	SET (INCLUDE_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE PATH "The directory the headers are installed in")
	SET (CMAKECONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/qjson" CACHE PATH "Directory where to install QJSONConfig.cmake")
	
	set (LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib" )
endif(QJSON_SUBPROJECT)
endif()

# pkg-config
IF (NOT WIN32)
  CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/QJson.pc.in
                  ${CMAKE_CURRENT_BINARY_DIR}/QJson.pc
                  @ONLY)
  INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/QJson.pc
           DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
ENDIF (NOT WIN32)

# Subdirs
ADD_SUBDIRECTORY(src)
IF (KDE4_BUILD_TESTS OR QJSON_BUILD_TESTS)
  enable_testing()
  ADD_SUBDIRECTORY(tests)
ENDIF (KDE4_BUILD_TESTS OR QJSON_BUILD_TESTS)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

#INSTALL(EXPORT qjson-export DESTINATION ${CMAKECONFIG_INSTALL_DIR} FILE QJSONTargets.cmake)

# figure out the relative path from the installed Config.cmake file to the install prefix (which may be at
# runtime different from the chosen CMAKE_INSTALL_PREFIX if under Windows the package was installed anywhere)
# This relative path will be configured into the QJSONConfig.cmake
file(RELATIVE_PATH relInstallDir ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKECONFIG_INSTALL_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

# cmake-modules
if(NOT QJSON_SUBPROJECT)
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/QJSONConfig.cmake.in
				${CMAKE_CURRENT_BINARY_DIR}/QJSONConfig.cmake
				@ONLY)
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/QJSONConfigVersion.cmake.in
				${CMAKE_CURRENT_BINARY_DIR}/QJSONConfigVersion.cmake
				@ONLY)
	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/QJSONConfig.cmake
				${CMAKE_CURRENT_BINARY_DIR}/QJSONConfigVersion.cmake
			DESTINATION "${CMAKECONFIG_INSTALL_DIR}")
	
	ADD_CUSTOM_TARGET(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif(NOT QJSON_SUBPROJECT)
