get_filename_component(qtactiongroup_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qtactiongroup_CURRENTDIR}")

list (APPEND SOURCES "${qtactiongroup_CURRENTDIR}/QtActionGroup.cpp")
list (APPEND Q_HEADERS "${qtactiongroup_CURRENTDIR}/QtActionGroup.h")