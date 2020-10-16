get_filename_component(cmdline_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${cmdline_CURRENTDIR}")

if(NOT COMMAND export_group)
	if(WIN32 AND NOT MINGW)
		list(APPEND HEADERS "${cmdline_CURRENTDIR}/xgetopt.h")
		list(APPEND SOURCES "${cmdline_CURRENTDIR}/xgetopt.cpp")
	endif(WIN32 AND NOT MINGW)
	
	list(APPEND HEADERS "${cmdline_CURRENTDIR}/CmdLineArguments.h")
	list(APPEND SOURCES "${cmdline_CURRENTDIR}/CmdLineArguments.cpp")
else()
	if(MSVC)
		set(MSVC_CMDLINE xgetopt.h xgetopt.cpp)
	endif(MSVC)
	export_group("cmdline" "cmdline" CmdLineArguments.h CmdLineArguments.cpp ${MSVC_CMDLINE})
endif()