get_filename_component(QTCHECKHEADER_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${QTCHECKHEADER_CURRENTDIR}")
    
list(APPEND Q_HEADERS "${QTCHECKHEADER_CURRENTDIR}/QtCheckHeaderView.h")

list(APPEND SOURCES "${QTCHECKHEADER_CURRENTDIR}/QtCheckHeaderView.cpp")

message("Hello, it's me. My name is QtCheckHeader.")