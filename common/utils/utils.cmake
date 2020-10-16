get_filename_component(UTILS_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${UTILS_CURRENTDIR}")
    
list(APPEND HEADERS "${UTILS_CURRENTDIR}/QOciQuery.h"
  "${UTILS_CURRENTDIR}/TVAError.h"
  "${UTILS_CURRENTDIR}/TVAMacro.h"
  "${UTILS_CURRENTDIR}/TVAUtils_global.h")

list(APPEND Q_HEADERS "${UTILS_CURRENTDIR}/TVATimer.h")

list(APPEND SOURCES "${UTILS_CURRENTDIR}/QOciQuery.cpp"
  "${UTILS_CURRENTDIR}/TVAError.cpp"
  "${UTILS_CURRENTDIR}/TVATimer.cpp"
  "${UTILS_CURRENTDIR}/TVAFunction.cpp")

message("Save Our Sources.")
