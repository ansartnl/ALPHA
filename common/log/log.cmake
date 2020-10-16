get_filename_component(log_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${log_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND HEADERS
			"${log_CURRENTDIR}/Log.h"
			"${log_CURRENTDIR}/ColorConsoleLayout.h"
			"${log_CURRENTDIR}/UserDatabaseLayout.h"
			"${log_CURRENTDIR}/UserSignalHelper.h"
			)
	
	list(APPEND SOURCES
			"${log_CURRENTDIR}/ColorConsoleLayout.cpp"
			"${log_CURRENTDIR}/UserDatabaseLayout.cpp"
			"${log_CURRENTDIR}/UserSignalHelper.cpp"
			)
else()
	export_group("log" "log" 
		Log.h 
		ColorConsoleLayout.h 
		ColorConsoleLayout.cpp
		UserDatabaseLayout.h
		UserDatabaseLayout.cpp
		UserSignalHelper.h
		UserSignalHelper.cpp
	)
	add_definitions(-DLOG4QT_LINKED_JUDICIOUSLY)
	add_definitions(-DIZMQT_LINKED_JUDICIOUSLY)
endif()			
