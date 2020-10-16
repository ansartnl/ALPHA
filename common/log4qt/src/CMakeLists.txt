cmake_minimum_required(VERSION 2.8.9)

project(Log4Qt)

message("Building Log4Qt solution")

include(../cmake/output-directories.cmake)
include(../cmake/compiler-flags.cmake)
include(../cmake/set-version.cmake)
include(../cmake/using-preset-defaults.cmake)
include(../cmake/sources-management.cmake)
include(../cmake/copy-files.cmake)

if(NOT LOG4QT_SUBPROJECT)
        declare_output_directories()
        set_multithreaded_build()
        set_cxx11_build()
        set_warnings_as_errors_build()
        set_default_definitions()
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
        set_debug_postfix("d")
endif()

option(LOG4QT_STATIC "Build Log4Qt statically" OFF)

find_qt(Core Network Xml Sql)

set_version(Log4Qt 1 0 0)

if(LOG4QT_STATIC)
        add_definitions(-DLOG4QT_STATIC)
else(LOG4QT_STATIC)
        add_definitions(-DLOG4QT_LIBRARY)
endif(LOG4QT_STATIC)


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}/helpers
        ${CMAKE_CURRENT_SOURCE_DIR}/spi
        ${CMAKE_CURRENT_SOURCE_DIR}/varia
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}/helpers
        ${CMAKE_CURRENT_BINARY_DIR}/spi
        ${CMAKE_CURRENT_BINARY_DIR}/varia
)

add_group("" ""
        appender.h
        appenderskeleton.h
        basicconfigurator.h
        colorconsoleappender.h
        consoleappender.h
        dailyrollingfileappender.h
        asyncappender.h
        mainthreadappender.h
        databaseappender.h
        databaselayout.h
        fileappender.h
        hierarchy.h
        layout.h
        level.h
        log4qt.h
        log4qtshared.h
        logger.h
        loggerrepository.h
        loggingevent.h
        logmanager.h
        mdc.h
        ndc.h
        patternlayout.h
        propertyconfigurator.h
        rollingfileappender.h
        signalappender.h
        simplelayout.h
        simpletimelayout.h
        ttcclayout.h
        telnetappender.h
        writerappender.h
        systemlogappender.h
    logstream.h
    colorhtmllayout.h
        appenderskeleton.cpp
        basicconfigurator.cpp
        colorconsoleappender.cpp
        consoleappender.cpp
        dailyrollingfileappender.cpp
        asyncappender.cpp
        mainthreadappender.cpp
        databaseappender.cpp
        databaselayout.cpp
        fileappender.cpp
        hierarchy.cpp
        layout.cpp
        level.cpp
        log4qt.cpp
        logger.cpp
        loggerrepository.cpp
        loggingevent.cpp
        logmanager.cpp
        mdc.cpp
        ndc.cpp
        patternlayout.cpp
        propertyconfigurator.cpp
        rollingfileappender.cpp
        signalappender.cpp
        simplelayout.cpp
        simpletimelayout.cpp
        ttcclayout.cpp
        telnetappender.cpp
        writerappender.cpp
        systemlogappender.cpp
        logstream.cpp
        colorhtmllayout.cpp
)

add_group("helpers" "helpers"
    helpers/classlogger.h
    helpers/appenderattachable.h
    helpers/configuratorhelper.h
    helpers/datetime.h
    helpers/factory.h
    helpers/initialisationhelper.h
    helpers/logerror.h
    helpers/logobject.h
    helpers/logobjectptr.h
    helpers/optionconverter.h
    helpers/patternformatter.h
    helpers/properties.h
    helpers/dispatcher.h
        helpers/classlogger.cpp
        helpers/appenderattachable.cpp
        helpers/configuratorhelper.cpp
        helpers/datetime.cpp
        helpers/factory.cpp
        helpers/initialisationhelper.cpp
        helpers/logerror.cpp
        helpers/logobject.cpp
        helpers/logobjectptr.cpp
        helpers/optionconverter.cpp
        helpers/patternformatter.cpp
        helpers/properties.cpp
        helpers/dispatcher.cpp
)

add_group("spi" "spi"
        spi/filter.h
        spi/filter.cpp
)

add_group("varia" "varia"
        varia/debugappender.h
        varia/denyallfilter.h
        varia/levelmatchfilter.h
        varia/levelrangefilter.h
        varia/listappender.h
        varia/nullappender.h
        varia/stringmatchfilter.h
        varia/debugappender.cpp
        varia/denyallfilter.cpp
        varia/levelmatchfilter.cpp
        varia/levelrangefilter.cpp
        varia/listappender.cpp
        varia/nullappender.cpp
        varia/stringmatchfilter.cpp
)

prepare_build_components()

set(BUILD_TYPE SHARED)
if(LOG4QT_STATIC)
        set(BUILD_TYPE STATIC)
endif(LOG4QT_STATIC)

add_library(Log4Qt ${BUILD_TYPE} ${BUILD_COMPONENTS})

set_property(TARGET Log4Qt PROPERTY FOLDER "Common")

link_qt(Log4Qt)
configure_build(Log4Qt)
set_output_directories(Log4Qt)


copy_files(Log4Qt ${INCLUDE_INSTALLATION_PATH}/Log4Qt ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_MAIN})
copy_files(Log4Qt ${INCLUDE_INSTALLATION_PATH}/Log4Qt/helpers ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_HELPERS})
copy_files(Log4Qt ${INCLUDE_INSTALLATION_PATH}/Log4Qt/spi ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_SPI})
copy_files(Log4Qt ${INCLUDE_INSTALLATION_PATH}/Log4Qt/varia ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_VARIA})
