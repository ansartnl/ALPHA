cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project
project(qttoolbardialog)

set(CMAKE_INCLUDE_CURRENT_DIR true)

# QT CUSTOMIZATION
# qt definitions
# dont using gui
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
set(SOURCES qttoolbardialog.cpp)

# simple headers
set(HEADERS qttoolbardialog.h)

# application forms *.ui
set(Q_FORMS qttoolbardialog.ui)

# application resources
set(RESOURCES qttoolbardialog.qrc)


# if you use Q_OBJECT
qt4_generate_moc(qttoolbardialog.cpp ${CMAKE_CURRENT_BINARY_DIR}/qttoolbardialog.moc)
set_source_files_properties(qttoolbardialog.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/qttoolbardialog.moc)

qt4_generate_moc(qttoolbardialog.h ${CMAKE_CURRENT_BINARY_DIR}/moc_qttoolbardialog.cpp)
set_source_files_properties(qttoolbardialog.h PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/moc_qttoolbardialog.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/)

qt4_wrap_ui(UI_HEADERS ${Q_FORMS})
qt4_add_resources(QRC_SOURCES ${RESOURCES})

# COMPILATION
add_library(${PROJECT_NAME} STATIC
  ${SOURCES}
  ${HEADERS}
  ${QRC_SOURCES}
  ${UI_HEADERS}
  )

# LINKING
# main library
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
