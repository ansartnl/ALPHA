get_filename_component(qxtspanslider_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

add_definitions(-DQXT_STATIC)

include_directories("${qxtspanslider_CURRENTDIR}/src/gui")
    
list(APPEND HEADERS "${qxtspanslider_CURRENTDIR}/src/core/qxtnamespace.h")
list(APPEND HEADERS "${qxtspanslider_CURRENTDIR}/src/core/qxtglobal.h")

list(APPEND Q_HEADERS "${qxtspanslider_CURRENTDIR}/src/gui/qxtspanslider_p.h")
list(APPEND Q_HEADERS "${qxtspanslider_CURRENTDIR}/src/gui/qxtspanslider.h")

list(APPEND SOURCES "${qxtspanslider_CURRENTDIR}/src/gui/qxtspanslider.cpp")

message("Hello, it's me. My name is qxtspanslider.")