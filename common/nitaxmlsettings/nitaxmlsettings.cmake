add_definitions(-DNITA_XML_SETTINGS)

get_filename_component(nitaxmlsettings_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${nitaxmlsettings_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND HEADERS "${nitaxmlsettings_CURRENTDIR}/nitaxmlsettings.h")
	list(APPEND SOURCES "${nitaxmlsettings_CURRENTDIR}/nitaxmlsettings.cpp")
else()
	export_group("nitaxmlsettings" "nitaxmlsettings" nitaxmlsettings.h nitaxmlsettings.cpp)
endif()
