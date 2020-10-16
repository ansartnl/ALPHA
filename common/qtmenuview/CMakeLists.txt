cmake_minimum_required(VERSION 2.8)

set(CMAKE_DEBUG_POSTFIX _debug)

project(qtmenuview)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

set(SOURCES qtmenuview.cpp)
set(HEADERS qtmenuview.h)


qt4_wrap_cpp(MOC_SOURCES ${HEADERS})

qt4_wrap_ui(UI_HEADERS ${FORMS})

qt4_add_resources(QRC_SOURCES ${RESOURCES})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS} ${MOC_SOURCES} ${UI_HEADERS} ${QRC_SOURCES})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} )
