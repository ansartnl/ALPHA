get_filename_component(qttcpdatagramprotocol_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qttcpdatagramprotocol_CURRENTDIR}")

list(APPEND SOURCES "${qttcpdatagramprotocol_CURRENTDIR}/qttcpdatagramprotocol.cpp")

list(APPEND Q_HEADERS "${qttcpdatagramprotocol_CURRENTDIR}/qttcpdatagramprotocol.h")