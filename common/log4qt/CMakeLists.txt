cmake_minimum_required(VERSION 2.6)

if(LOG4QT_SUBPROJECT)
	add_definitions(-DLOG4QT_SUBPROJECT)
	add_subdirectory(src)
	return()
endif(LOG4QT_SUBPROJECT)

# cmake options
message("Building Log4Qt")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
# postfix for library
set(CMAKE_DEBUG_POSTFIX _debug)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
# variable for project name, used in many places
set (PROJECT log4qt)

project(${PROJECT})

string(TOUPPER ${PROJECT_NAME} PROJECT_TO_UPPER)

set(QT_DONT_USE_QTGUI TRUE)

include(../cmake/version-dep-commands.cmake)
find_qt(Core Network Sql)

# COMPILER customization
  # compilers flags
  if (MSVC)
    add_definitions (/W3 -Zc:wchar_t-)
  elseif (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions (-Wall)
  else ()
    message ("Unknown compiler")
  endif ()
  #compiler options
  set(OPTIMIZATION_FLAGS "-O2" )
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

# remove when release
if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(CMAKE_VERBOSE_MAKEFILE TRUE)
endif (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release")

# SOURCE FILES AND DIRECTORIES
set(LOG4QT_SOURCES)
# Q_SOURCES, var contains files, for which need call qt4_automoc
set(LOG4QT_Q_SOURCES)
set(LOG4QT_HEADERS)
# Q_HEADERS, var contains files, for which qt4_wrap_cpp called
set(LOG4QT_Q_HEADERS)

set(LOG4QT_HEADERS
    src/basicconfigurator.h
    src/helpers/classlogger.h
    src/helpers/datetime.h
    src/helpers/factory.h
    src/hierarchy.h
    src/helpers/initialisationhelper.h
    src/level.h
    src/log4qt.h
    src/log4qtshared.h
    src/loggerrepository.h
    src/loggingevent.h
    src/logmanager.h
    src/helpers/logobjectptr.h
    src/mdc.h
    src/ndc.h
    src/helpers/optionconverter.h
    src/helpers/patternformatter.h
    src/helpers/properties.h
    src/propertyconfigurator.h
    src/helpers/logerror.h
    src/helpers/appenderattachable.h
    src/logstream.h
    src/colorhtmllayout.h
)

set(LOG4QT_Q_HEADERS src/appender.h
    src/appenderskeleton.h
    src/asyncappender.h
    src/colorconsoleappender.h
    src/consoleappender.h
    src/dailyrollingfileappender.h
    src/fileappender.h
    src/mainthreadappender.h
    src/layout.h
    src/logger.h
    src/patternlayout.h
    src/rollingfileappender.h
    src/signalappender.h
    src/simplelayout.h
    src/simpletimelayout.h
    src/ttcclayout.h
    src/writerappender.h
    src/systemlogappender.h
    src/helpers/configuratorhelper.h
    src/helpers/logobject.h
    src/helpers/dispatcher.h
    src/spi/filter.h
    src/varia/debugappender.h
    src/varia/denyallfilter.h
    src/varia/levelmatchfilter.h
    src/varia/levelrangefilter.h
    src/varia/listappender.h
    src/varia/nullappender.h
    src/varia/stringmatchfilter.h)
    
set(LOG4QT_SOURCES
    src/helpers/classlogger.cpp
    src/helpers/configuratorhelper.cpp
    src/helpers/datetime.cpp
    src/helpers/factory.cpp
    src/helpers/initialisationhelper.cpp
    src/helpers/logerror.cpp
    src/helpers/logobject.cpp
    src/helpers/dispatcher.cpp
    src/helpers/logobjectptr.cpp
    src/helpers/optionconverter.cpp
    src/helpers/patternformatter.cpp
    src/helpers/properties.cpp
    src/helpers/appenderattachable.cpp
    src/spi/filter.cpp
    src/varia/debugappender.cpp
    src/varia/denyallfilter.cpp
    src/varia/levelmatchfilter.cpp
    src/varia/levelrangefilter.cpp
    src/varia/listappender.cpp
    src/varia/nullappender.cpp
    src/varia/stringmatchfilter.cpp
    src/systemlogappender.cpp
    src/appenderskeleton.cpp
    src/basicconfigurator.cpp
    src/colorconsoleappender.cpp
    src/consoleappender.cpp
    src/asyncappender.cpp
    src/dailyrollingfileappender.cpp
    src/fileappender.cpp
    src/mainthreadappender.cpp
    src/hierarchy.cpp
    src/layout.cpp
    src/level.cpp
    src/log4qt.cpp
    src/logger.cpp
    src/loggerrepository.cpp
    src/loggingevent.cpp
    src/logmanager.cpp
    src/mdc.cpp
    src/ndc.cpp
    src/patternlayout.cpp
    src/propertyconfigurator.cpp
    src/rollingfileappender.cpp
    src/signalappender.cpp
    src/simplelayout.cpp
    src/simpletimelayout.cpp
    src/ttcclayout.cpp
    src/writerappender.cpp
    src/logstream.cpp
    src/colorhtmllayout.cpp
)

# project version
set(LOG4QT_VERSION_MAJOR 1)
set(LOG4QT_VERSION_MINOR 0)
set(LOG4QT_VERSION_RELEASE 0)
set(LOG4QT_VERSION "${LOG4QT_VERSION_MAJOR}.${LOG4QT_VERSION_MINOR}.${LOG4QT_VERSION_RELEASE}")
set(VERSION ${LOG4QT_VERSION})
set(SOVERSION ${LOG4QT_VERSION})
add_definitions(-DLOG4QT_VERSION=${LOG4QT_VERSION_MAJOR}*65536+${LOG4QT_VERSION_MINOR}*256+${LOG4QT_VERSION_RELEASE})
add_definitions(-DLOG4QT_VERSION_STR="${LOG4QT_VERSION}")

#if (QT_USE_QTSQL)
	message("With QT_USE_QTSQL DatabaseAppender, DatabaseLayout compiled")
	list(APPEND LOG4QT_Q_HEADERS src/databaseappender.h src/databaselayout.h)
	list(APPEND LOG4QT_SOURCES src/databaseappender.cpp src/databaselayout.cpp)
#endif (QT_USE_QTSQL)

#if (QT_USE_QTNETWORK)
    message("With QT_USE_QTNETWORK TelnetAppender compiled")
    list(APPEND LOG4QT_Q_HEADERS src/telnetappender.h )
    list(APPEND LOG4QT_SOURCES src/telnetappender.cpp)
#endif (QT_USE_QTNETWORK)

include_directories(src)

# COMPILATION
# if you use Q_OBJECT
#qt45_wrap_cpp(LOG4QT_MOC_SOURCES ${LOG4QT_Q_HEADERS})
 
# BUILD STATIC LIBRARY
if (LOG4QT_BUILD_STATIC)
set(LOG4QT_BUILD_CONFIGURATION STATIC)
# BUILD SHARED LIBRARY
else (LOG4QT_BUILD_STATIC)
set(LOG4QT_BUILD_CONFIGURATION SHARED)
endif (LOG4QT_BUILD_STATIC)

add_library(${PROJECT_NAME} ${LOG4QT_BUILD_CONFIGURATION} 
    ${LOG4QT_SOURCES}
    ${LOG4QT_Q_SOURCES}
    ${LOG4QT_MOC_SOURCES}
    ${LOG4QT_HEADERS}
    ${LOG4QT_Q_HEADERS})
    
# LINKING
link_qt(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES 
    SOVERSION ${LOG4QT_VERSION}
    VERSION ${LOG4QT_VERSION}
    DEFINE_SYMBOL ${PROJECT_TO_UPPER}_EXPORTS)

if (NOT LOG4QT_DONT_INSTALL)
    # CMAKE CONFIG FILE
    
    if (NOT APPLE)
        SET(TARGET_INCLUDE_DIR "${QT_INCLUDE_DIR}/${PROJECT_NAME}")
        SET(TARGET_LIB_DIR "${QT_LIBRARY_DIR}")
        SET(TARGET_BIN_DIR "${QT_BINARY_DIR}")
    else()
        #Qt is the framework but log4qt is not. So it should not be placed into the frameworks location
        #Also the include dir should not be retrieved from the Qt itself, see http://www.paraview.org/Bug/view.php?id=10632
        SET(TARGET_INCLUDE_DIR "/usr/include")
        SET(TARGET_LIB_DIR "/usr/lib")
        SET(TARGET_BIN_DIR "/usr/bin")
    endif()
    
    set (INSTALL_PROJECT_INCLUDE_DIR "${TARGET_INCLUDE_DIR}/${PROJECT_NAME}")
    # for using library in your cmake projects
    configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/project-config.cmake.in"
      "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
      IMMEDIATE @ONLY)
    
    # INSTALLATION
    # install and export project target
    # copy project file into qt_libdir and create project.cmake file for use this library in your cmake projects
    set(EXPORT_TARGET_DESTINATION "${QT_MKSPECS_DIR}/cmake/${PROJECT_NAME}")
    set(PROJECT_TARGETS ${PROJECT_NAME}-targets)
    
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_TARGETS}
      RUNTIME DESTINATION ${TARGET_BIN_DIR}
      LIBRARY DESTINATION ${TARGET_LIB_DIR}
      ARCHIVE DESTINATION ${TARGET_LIB_DIR})
    
    # install exported project target cmake file into qt_mkspec_dir/cmake
    install(EXPORT ${PROJECT_TARGETS} DESTINATION ${EXPORT_TARGET_DESTINATION} COMPONENT Devel)
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" DESTINATION "${INSTALL_PROJECT_INCLUDE_DIR}" COMPONENT Devel
        FILES_MATCHING PATTERN "*.h")
    
    # install cmake config file
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake DESTINATION ${EXPORT_TARGET_DESTINATION} COMPONENT Devel)
    
    # UNINSTALL TARGET
    # create cmake uninstall files and unregister docs scripts
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)
    
    add_custom_target(uninstall
        COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif (NOT LOG4QT_DONT_INSTALL)


################################################
# Doxygen documentation generation
################################################
### DOXYGEN DOCUMENTATION SYSTEM
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/doc")
set(DOXYGEN_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/doxygen")
set(DOXYGEN_DOCSET_FEEDNAME "${PROJECT_NAME}")
set(DOXYGEN_OUTPUT_LANGUAGE "English")

find_program(QT_QHELPGENERATOR_EXECUTABLE
  NAMES qhelpgenerator
  PATHS ${QT_BINARY_DIR}
  NO_DEFAULT_PATH)

if(QT_QHELPGENERATOR_EXECUTABLE)
  set(DOXYGEN_GENERATE_QHP "YES")
  # relative to html directory
  set(DOXYGEN_QCH_FILE  "../qch/${PROJECT_NAME}.qch")
  set(DOXYGEN_QHP_NAMESPACE "com.trolltech.${PROJECT_NAME}.1")
  set(DOXYGEN_QHP_VIRTUAL_FOLDER "${PROJECT_NAME}-api")
  set(DOXYGEN_QHG_LOCATION "${QT_QHELPGENERATOR_EXECUTABLE}")
else(QT_QHELPGENERATOR_EXECUTABLE)
  set(DOXYGEN_GENERATE_QHP "NO")
endif(QT_QHELPGENERATOR_EXECUTABLE)

find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
  add_custom_target(log4-qt-doxygen
    COMMAND ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  message("Command to create html and qch doc: make (mingw32-make) log4-qt-doxygen")
endif (DOXYGEN_FOUND)

if (DOXYGEN_DOT_FOUND)
  set(DOXYGEN_HAVE_DOT "YES")
  message("Enabled graphs")
else(DOXYGEN_DOT_FOUND)
  set(DOXYGEN_HAVE_DOT "NO")
endif(DOXYGEN_DOT_FOUND)

set(QCH_DOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc/qch)
set(QCH_DOC_NAMES ${PROJECT_NAME}.qch)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
  "${CMAKE_BINARY_DIR}/Doxyfile"
  IMMEDIATE @ONLY)
#################################################
