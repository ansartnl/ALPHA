get_filename_component(scriptrunner_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${scriptrunner_CURRENTDIR}")

list(APPEND Q_HEADERS
        "${scriptrunner_CURRENTDIR}/ScriptRunner.h"
        )

list(APPEND SOURCES
        "${scriptrunner_CURRENTDIR}/ScriptRunner.cpp"
        )
