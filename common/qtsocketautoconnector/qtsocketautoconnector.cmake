get_filename_component(qtsocketautoconnector_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qtsocketautoconnector_CURRENTDIR}")

list(APPEND SOURCES "${qtsocketautoconnector_CURRENTDIR}/qtsocketautoconnector.cpp")

list(APPEND Q_HEADERS "${qtsocketautoconnector_CURRENTDIR}/qtsocketautoconnector.h")