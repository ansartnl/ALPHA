cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

project(qtdelegates)

include(../cmake/version-dep-commands.cmake)
find_qt(Widgets Xml)

set(qtdelegates_SOURCES comboboxdelegate.cpp
    plaintexteditdelegate.cpp
    qtcheckboxdelegate.cpp
    tabledelegate.cpp
    texteditdelegate.cpp
    coordedit.cpp
    coorddelegate.cpp
)

set(qtdelegates_HEADERS comboboxdelegate.h
    plaintexteditdelegate.h
    qtcheckboxdelegate.h
    tabledelegate.h
    texteditdelegate.h
    coordedit.h
    coorddelegate.h
    )
    
set(qtdelegates_P_HEADERS private/comboboxdelegate_p.h
    private/tabledelegate_p.h
    )

qt45_wrap_cpp(qtdelegates_MOC_SOURCES ${qtdelegates_HEADERS})

add_library(${PROJECT_NAME} STATIC ${qtdelegates_SOURCES} ${qtdelegates_HEADERS} ${qtdelegates_MOC_SOURCES}
    ${qtdelegates_P_HEADERS})

link_qt(${PROJECT_NAME})
