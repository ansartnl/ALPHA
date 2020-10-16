get_filename_component(qcorecmdlineargs_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qcorecmdlineargs_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND HEADERS 
		"${qcorecmdlineargs_CURRENTDIR}/qcorecmdlineargs_p.h"
		"${qcorecmdlineargs_CURRENTDIR}/qt_windows.h")
else()
	export_group("qcorecmdlineargs" "qcorecmdlineargs" 
		qcorecmdlineargs_p.h
		qt_windows.h
	)
endif()
