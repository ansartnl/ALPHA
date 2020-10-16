cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project
project(qtwindowmenu)
set(CMAKE_INCLUDE_CURRENT_DIR true)

find_package(Qt4 REQUIRED)
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

# SOURCE FILES AND DIRECTORIES

# simple sources
set(SOURCES qtwindowmenu.cpp)

# simple headers
set(HEADERS qtwindowmenu.h)

# application forms *.ui
set(FORMS)

# application resources
set(RESOURCES)

# windows resources
set(WIN_RESOURCES)

# END CPP CLASSES-WRAPPERS FOR QT SCRIPT (OUTPUT SOURCES VARIABLE: QS_SOURCES)
# -----------------------------------------------------------------------------------------------------

# if you use Q_OBJECT
qt4_wrap_cpp(MOC_SOURCES ${HEADERS})

qt4_automoc(${Q_SOURCES})

qt4_wrap_ui(UI_HEADERS ${Q_FORMS})

qt4_add_resources(QRC_SOURCES ${Q_RESOURCES})

# COMPILATION
add_library(${PROJECT_NAME} STATIC
  ${SOURCES}
  ${Q_SOURCES}
  ${MOC_SOURCES}
  ${QRC_SOURCES}
  ${HEADERS}
  ${UI_HEADERS}
  )

# LINKING
# main library
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
