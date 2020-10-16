cmake_minimum_required(VERSION 2.6)

set(CMAKE_DEBUG_POSTFIX _debug)

# Declare project

project(license)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include(../cmake/version-dep-commands.cmake)
find_qt(Core Widgets)

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

set(license_SOURCES Base32.cpp HardwareId.cpp HardwareProvider.cpp License.cpp)
set(license_HEADERS Base32.h HardwareId.h HardwareProvider.h License.h)

# COMPILATION
add_library(${PROJECT_NAME} STATIC
  ${license_SOURCES}
  ${license_HEADERS}
  )

# LINKING
# main library
link_qt(${PROJECT_NAME})

# license generator
set(generator_SOURCES generator/main.cpp generator/MainWindow.cpp)
set(generator_HEADERS generator/MainWindow.h)
set(generator_FORMS generator/MainWindow.ui)

qt45_wrap_ui(generator_UI_HEADERS ${generator_FORMS})

add_executable(LicenseGenerator ${generator_SOURCES} ${generator_HEADERS} ${generator_UI_HEADERS})
target_link_libraries(LicenseGenerator ${PROJECT_NAME})
