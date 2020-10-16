get_filename_component(qttcpdatagramconnectionchecker_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qttcpdatagramconnectionchecker_CURRENTDIR}")

list(APPEND SOURCES "${qttcpdatagramconnectionchecker_CURRENTDIR}/qttcpdatagramconnectionchecker.cpp")

list(APPEND Q_HEADERS "${qttcpdatagramconnectionchecker_CURRENTDIR}/qttcpdatagramconnectionchecker.h")
