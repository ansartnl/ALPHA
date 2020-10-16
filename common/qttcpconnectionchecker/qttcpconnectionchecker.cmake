get_filename_component(qttcpconnectionchecker_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qttcpconnectionchecker_CURRENTDIR}")

list(APPEND SOURCES "${qttcpconnectionchecker_CURRENTDIR}/qttcpconnectionchecker.cpp")

list(APPEND Q_HEADERS "${qttcpconnectionchecker_CURRENTDIR}/qttcpconnectionchecker.h")
