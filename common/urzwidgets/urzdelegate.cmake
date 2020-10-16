get_filename_component(URZWIDGETS_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${URZWIDGETS_CURRENTDIR}")

list(APPEND Q_HEADERS "${URZWIDGETS_CURRENTDIR}/urzdelegate.h")
list(APPEND SOURCES "${URZWIDGETS_CURRENTDIR}/urzdelegate.cpp")

include("${URZWIDGETS_CURRENTDIR}/urzcombobox.cmake")
