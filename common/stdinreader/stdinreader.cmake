get_filename_component(stdinreceiver_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${stdinreceiver_CURRENTDIR}")
include_directories("${stdinreceiver_CURRENTDIR}/../sleep/")

list(APPEND HEADERS
        "${stdinreceiver_CURRENTDIR}/../sleep/sleep.h"
        )

list(APPEND Q_HEADERS
        "${stdinreceiver_CURRENTDIR}/StdinReader.h"
        )

list(APPEND SOURCES
        "${stdinreceiver_CURRENTDIR}/StdinReader.cpp"
        )
