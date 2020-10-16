get_filename_component(MESVIEWER_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${MESVIEWER_CURRENTDIR}")
    
list(APPEND Q_HEADERS "${MESVIEWER_CURRENTDIR}/mesviewer.h")

list(APPEND SOURCES "${MESVIEWER_CURRENTDIR}/mesviewer.cpp")

list(APPEND FORMS "${MESVIEWER_CURRENTDIR}/mesviewer.ui")

message("Hello, it's me. My name is OLDI message viewer.")