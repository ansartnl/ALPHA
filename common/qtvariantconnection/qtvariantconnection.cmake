get_filename_component(qtvariantconnection_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qtvariantconnection_CURRENTDIR}")

list(APPEND SOURCES "${qtvariantconnection_CURRENTDIR}/qtvariantconnection.cpp")

list(APPEND Q_HEADERS "${qtvariantconnection_CURRENTDIR}/qtvariantconnection.h")

include("${qtvariantconnection_CURRENTDIR}/../qttcpdatagramprotocol/qttcpdatagramprotocol.cmake")