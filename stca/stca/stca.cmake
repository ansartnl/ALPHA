get_filename_component(stca_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${stca_CURRENTDIR}/export")

list(APPEND SOURCES 
    "${stca_CURRENTDIR}/export/StcaObject.cpp"
    "${stca_CURRENTDIR}/export/AirplaneCollision.cpp"
    "${stca_CURRENTDIR}/export/MeteoWarning.cpp"
    "${stca_CURRENTDIR}/export/RestrictionWarning.cpp"
    )

list(APPEND HEADERS 
    "${stca_CURRENTDIR}/export/StcaObject.h"
    "${stca_CURRENTDIR}/export/AirplaneCollision.h"
    "${stca_CURRENTDIR}/export/MeteoWarning.h"
    "${stca_CURRENTDIR}/export/RestrictionWarning.h"
    )

message("Hello, it's me. My name is STCA.")
