cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

project(qtautotransaction)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include(../cmake/version-dep-commands.cmake)
find_qt(Sql Widgets)

set(qtautotransaction_SOURCES qtautotransaction.cpp)

set(qtautotransaction_Q_HEADERS)
set(qtautotransaction_HEADERS qtautotransaction.h)

qt45_wrap_cpp(qtautotransaction_MOC_SOURCES ${qtautotransaction_Q_HEADERS})

qt45_wrap_ui(qtautotransaction_UI_HEADERS ${qtautotransaction_FORMS})

add_library(${PROJECT_NAME} STATIC ${qtautotransaction_SOURCES} ${qtautotransaction_HEADERS} ${qtautotransaction_Q_HEADERS} ${qtautotransaction_MOC_SOURCES})

link_qt(${PROJECT_NAME})
