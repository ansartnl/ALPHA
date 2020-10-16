get_filename_component(qtredis_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)
include_directories("${qtredis_CURRENTDIR}")

list(APPEND SOURCES "${qtredis_CURRENTDIR}/reader.cpp")
list(APPEND SOURCES "${qtredis_CURRENTDIR}/qtredis.cpp")

list(APPEND Q_HEADERS "${qtredis_CURRENTDIR}/reader.h")
list(APPEND Q_HEADERS "${qtredis_CURRENTDIR}/qtredis.h")
