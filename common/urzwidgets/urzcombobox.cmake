get_filename_component(URZWIDGETS_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${URZWIDGETS_CURRENTDIR}")

list(APPEND Q_HEADERS "${URZWIDGETS_CURRENTDIR}/urzcombobox.h")
list(APPEND SOURCES "${URZWIDGETS_CURRENTDIR}/urzcombobox.cpp")

include("${URZWIDGETS_CURRENTDIR}/treecombobox.cmake")
