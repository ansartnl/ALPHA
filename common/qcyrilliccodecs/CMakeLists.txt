cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# ------------------------------------------------------------
# CMake setup
# ------------------------------------------------------------
# Declare project
project(qcyrilliccodecs)

set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/codecs)

find_package(Qt4 REQUIRED)
set(QT_DONT_USE_QTGUI TRUE)
# Create libraries list
include(${QT_USE_FILE})

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
  set (CMAKE_VERBOSE_MAKEFILE TRUE)
endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

set(CMAKE_INCLUDE_CURRENT_DIR true)

if (QCYRILLICCODECS_BUILD_STATIC)

    list(APPEND QCYRILLICCODECS_SOURCES qia5textcodec.cpp qita2textcodec.cpp qmtk2textcodec.cpp qmtk5textcodec.cpp)
    list(APPEND QCYRILLICCODECS_HEADERS allcodecs.h qia5textcodec.h qita2textcodec.h qmtk2textcodec.h qmtk5textcodec.h)

    add_library(${PROJECT_NAME} STATIC
        ${QCYRILLICCODECS_SOURCES}
        ${QCYRILLICCODECS_HEADERS}
    )

    # LINKING
    # main library
    target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
    
else()

    list(APPEND SOURCES plugin.cpp qia5textcodec.cpp qita2textcodec.cpp qmtk2textcodec.cpp qmtk5textcodec.cpp)
    list(APPEND HEADERS allcodecs.h qia5textcodec.h qita2textcodec.h qmtk2textcodec.h qmtk5textcodec.h)
    
    add_definitions(-DQT_PLUGIN)
    add_definitions(-DQT_SHARED)
    
    qt4_generate_moc(plugin.cpp ${CMAKE_CURRENT_BINARY_DIR}/plugin.moc )
    set_source_files_properties(plugin.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plugin.moc)
    
    add_library(${PROJECT_NAME} SHARED
      ${QCYRILLICCODECS_SOURCES}
      ${QCYRILLICCODECS_HEADERS}
    )
    
    # LINKING
    # main library
    target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
    
    #################################################
    # Installation
    #################################################
    
    install(TARGETS ${PROJECT_NAME}
      ARCHIVE DESTINATION "${QT_PLUGINS_DIR}/codecs"
      LIBRARY DESTINATION "${QT_PLUGINS_DIR}/codecs"
      RUNTIME DESTINATION "${QT_PLUGINS_DIR}/codecs"
      COMPONENT Runtime
    )
    
    ########################################
    ## Uninstalling for unix "make uninstall"
    ## Note: uninstallning system for windows automatically generated by cpack
    ########################################
    
    CONFIGURE_FILE(
      "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
      "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
      IMMEDIATE @ONLY)
    
    ADD_CUSTOM_TARGET(uninstall
      "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif (QCYRILLICCODECS_BUILD_STATIC)