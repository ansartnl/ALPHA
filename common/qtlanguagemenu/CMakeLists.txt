cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

project(qtlanguagemenu)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include(../cmake/version-dep-commands.cmake)
find_qt(Widgets)

set(qtlanguagemenu_SOURCES 
    qtlanguagemenu.cpp
    qttranslator.cpp
)
set(qtlanguagemenu_HEADERS 
    qtlanguagemenu.h
    qttranslator.h
)

qt45_wrap_cpp(qtlanguagemenu_MOC_SOURCES ${qtlanguagemenu_HEADERS})

qt45_wrap_ui(qtlanguagemenu_UI_HEADERS ${qtlanguagemenu_FORMS})

add_library(${PROJECT_NAME} STATIC ${qtlanguagemenu_SOURCES} ${qtlanguagemenu_HEADERS} ${qtlanguagemenu_MOC_SOURCES})

link_qt(${PROJECT_NAME})
