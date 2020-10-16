get_filename_component(qtinisettings_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qtinisettings_CURRENTDIR}")

list(APPEND SOURCES "${qtinisettings_CURRENTDIR}/qtinisettings.cpp")

list(APPEND Q_HEADERS "${qtinisettings_CURRENTDIR}/qtinisettings.h")