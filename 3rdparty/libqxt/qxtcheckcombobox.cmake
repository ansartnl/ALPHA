get_filename_component(qxtcheckcombobox_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

add_definitions(-DQXT_STATIC)

include_directories("${qxtcheckcombobox_CURRENTDIR}/src/gui")
    
list(APPEND HEADERS "${qxtcheckcombobox_CURRENTDIR}/src/core/qxtnamespace.h")
list(APPEND HEADERS "${qxtcheckcombobox_CURRENTDIR}/src/core/qxtglobal.h")

list(APPEND Q_HEADERS "${qxtcheckcombobox_CURRENTDIR}/src/gui/qxtcheckcombobox_p.h")
list(APPEND Q_HEADERS "${qxtcheckcombobox_CURRENTDIR}/src/gui/qxtcheckcombobox.h")

list(APPEND SOURCES "${qxtcheckcombobox_CURRENTDIR}/src/gui/qxtcheckcombobox.cpp")

message("Hello, it's me. My name is qxtcheckcombobox.")
