get_filename_component(factory_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${factory_CURRENTDIR}")

list(APPEND HEADERS "${factory_CURRENTDIR}/factory.h")