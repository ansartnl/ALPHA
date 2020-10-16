get_filename_component(qtudpsocket_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qtudpsocket_CURRENTDIR}")

list(APPEND SOURCES "${qtudpsocket_CURRENTDIR}/qtudpsocket.cpp")

list(APPEND Q_HEADERS "${qtudpsocket_CURRENTDIR}/qtudpsocket.h")

include(${qtudpsocket_CURRENTDIR}/../broadcast/broadcast.cmake)
