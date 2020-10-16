cmake_minimum_required( VERSION 2.6.0 )
project( qserialdevice )
set(CMAKE_DEBUG_POSTFIX _debug)

find_package( Qt4 REQUIRED )

set(CMAKE_INCLUDE_CURRENT_DIR true)

include_directories( ${CMAKE_CURRENT_BINARY_DIR})
#                     ${QT_INCLUDES} 
#)

set ( LIB_INCLUDE_DIRS 
    ${PROJECT_SOURCE_DIR}/qserialdevice
    ${PROJECT_SOURCE_DIR}/qserialdeviceenumerator
    CACHE string "Paths to include library headers."
)

# Disable Qt GUI and add definition.
set( QT_DONT_USE_QTGUI 1 )
set( QT_DEFINITIONS
    -DUNICODE
    -DQT_NO_DEBUG
    -DQT_THREAD_SUPPORT
)
add_definitions( ${QT_DEFINITIONS} )
include(${QT_USE_FILE})

# Library source files.
set( QSERIALDEVICE_SRCS
    qserialdevice/abstractserial.cpp
    qserialdevice/abstractserialengine.cpp
    qserialdevice/nativeserialengine.cpp
    qserialdevice/abstractserialnotifier.cpp

    qserialdeviceenumerator/serialdeviceenumerator.cpp
)

# Hooking up a platform-dependent *.h/*.cpp files   
# and include additional system libraries.
# ADDITIONAL_LIBRARY - additional platform-dependent system libraries set.
if(WIN32)
    set( QSERIALDEVICE_SRCS
        ${QSERIALDEVICE_SRCS}
        qserialdevice/nativeserialengine_win.cpp
        qserialdevice/nativeserialnotifier_win.cpp

        qserialdeviceenumerator/serialdeviceenumerator_p_win.cpp
    )
    set( ADDITIONAL_LIBRARY setupapi uuid advapi32 )
elseif(UNIX)
    set( QSERIALDEVICE_SRCS
        ${QSERIALDEVICE_SRCS}
        qserialdevice/nativeserialengine_unix.cpp
        qserialdevice/ttylocker.cpp
        qserialdevice/nativeserialnotifier_unix.cpp
    )
    if(APPLE)
        set( QSERIALDEVICE_SRCS
            ${QSERIALDEVICE_SRCS}
            qserialdeviceenumerator/serialdeviceenumerator_p_mac.cpp
        )
    else(APPLE)
        set( QSERIALDEVICE_SRCS
            ${QSERIALDEVICE_SRCS}
            qserialdeviceenumerator/serialdeviceenumerator_p_unix.cpp
        )
        set( ADDITIONAL_LIBRARY udev )
    endif(APPLE)
else()
    message( FATAL_ERROR "This operating system is not supported library QSerialDevice" )
endif()

qt4_generate_moc( qserialdevice/abstractserial.h ${CMAKE_CURRENT_BINARY_DIR}/moc_abstractserial.cpp )
qt4_generate_moc( qserialdevice/abstractserialengine.h ${CMAKE_CURRENT_BINARY_DIR}/moc_abstractserialengine.cpp )
qt4_generate_moc( qserialdevice/nativeserialengine.h ${CMAKE_CURRENT_BINARY_DIR}/moc_nativeserialengine.cpp )
qt4_generate_moc( qserialdeviceenumerator/serialdeviceenumerator.h ${CMAKE_CURRENT_BINARY_DIR}/moc_serialdeviceenumerator.cpp )

set_source_files_properties( qserialdevice/abstractserial.cpp OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/moc_abstractserial.cpp )
set_source_files_properties( qserialdevice/abstractserialengine.cpp OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/moc_abstractserialengine.cpp )
set_source_files_properties( qserialdevice/nativeserialengine.cpp OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/moc_nativeserialengine.cpp )
set_source_files_properties( qserialdeviceenumerator/serialdeviceenumerator.cpp OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/moc_serialdeviceenumerator.cpp )

add_library( ${PROJECT_NAME} STATIC ${QSERIALDEVICE_SRCS} ${QSERIALDEVICE_MOCS} )

target_link_libraries( ${PROJECT_NAME} ${QT_LIBRARIES} ${ADDITIONAL_LIBRARY} )
