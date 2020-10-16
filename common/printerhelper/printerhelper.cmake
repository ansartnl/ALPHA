get_filename_component(printerhelper_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${printerhelper_CURRENTDIR}")

list(APPEND HEADERS
        "${printerhelper_CURRENTDIR}/PrinterHelper.h"
        )

list(APPEND SOURCES
        "${printerhelper_CURRENTDIR}/PrinterHelper.cpp"
        )
