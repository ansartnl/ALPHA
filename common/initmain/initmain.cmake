get_filename_component(initmain_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${initmain_CURRENTDIR}")

if(NOT COMMAND export_group)
	include(${initmain_CURRENTDIR}/../cmdline/cmdline.cmake)

	list(APPEND SOURCES "${initmain_CURRENTDIR}/InitMain.cpp")
	list(APPEND HEADERS "${initmain_CURRENTDIR}/InitMain.h")
else()
	export_group("initmain" "initmain" InitMain.h InitMain.cpp)
endif()