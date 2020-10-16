get_filename_component(compare_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${compare_CURRENTDIR}")

list(APPEND HEADERS
        "${compare_CURRENTDIR}/Compare.h"
        )

list(APPEND SOURCES
        "${compare_CURRENTDIR}/Compare.cpp"
        )