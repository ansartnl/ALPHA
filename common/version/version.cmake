get_filename_component(version_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${version_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND HEADERS "${version_CURRENTDIR}/version.h")
else()
	export_group("version" "version" version.h)
endif()