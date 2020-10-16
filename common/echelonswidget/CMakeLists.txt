cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project

project(echelonswidget)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include(../cmake/version-dep-commands.cmake)
find_qt(Widgets)

message("Hello, you're building project ${PROJECT_NAME}. I hope, it helps you to make your life easer.")

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
set(echelonswidget_SOURCES EchelonsWidget.cpp)

# simple headers
set(echelonswidget_Q_HEADERS EchelonsWidget.h)

# application forms *.ui
set(echelonswidget_FORMS EchelonsWidget.ui)

# application resources
set(echelonswidget_RESOURCES)

# windows resources
set(echelonswidget_WIN_RESOURCES)

# END CPP CLASSES-WRAPPERS FOR QT SCRIPT (OUTPUT SOURCES VARIABLE: QS_SOURCES)
# -----------------------------------------------------------------------------------------------------

# if you use Q_OBJECT
qt45_wrap_cpp(echelonswidget_MOC_SOURCES ${echelonswidget_Q_HEADERS})

qt45_wrap_ui(echelonswidget_UI_HEADERS ${echelonswidget_FORMS})

qt45_add_resources(echelonswidget_QRC_SOURCES ${echelonswidget_RESOURCES})

# COMPILATION
add_library(${PROJECT_NAME} STATIC
  ${echelonswidget_SOURCES}
  ${echelonswidget_MOC_SOURCES}
  ${echelonswidget_QRC_SOURCES}
  ${echelonswidget_Q_HEADERS}
  ${echelonswidget_UI_HEADERS}
  )

# LINKING
# main library
link_qt(${PROJECT_NAME})
