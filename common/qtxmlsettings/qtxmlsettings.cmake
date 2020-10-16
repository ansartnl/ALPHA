get_filename_component(qtxmlsettings_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qtxmlsettings_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND HEADERS "${qtxmlsettings_CURRENTDIR}/qtxmlsettings.h")
	list(APPEND SOURCES "${qtxmlsettings_CURRENTDIR}/qtxmlsettings.cpp")
else()
	export_group("qtxmlsettings" "qtxmlsettings" qtxmlsettings.h qtxmlsettings.cpp)
endif()