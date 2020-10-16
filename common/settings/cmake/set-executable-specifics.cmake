
macro(SET_EXECUTABLE_SPECIFICS)

	if(WIN32)
		set(EXEC_EXTRA_PARAMS WIN32)
	endif(WIN32)

	if(WIN32)
		set(EXTERNAL_LIBS
			shlwapi
			crypt32
			ws2_32
			Version
		)
	endif()
	
	if(UNIX)
		set(EXTERNAL_LIBS
			pthread
		)
	endif(UNIX)
	
	option(DEVELOPER_FEATURES "use some debug tricks for developers" OFF)

	if(DEVELOPER_FEATURES AND WIN32)
		add_definitions(
			-DDEVELOPER_FEATURES /Zi
		)
		set(CMAKE_EXE_LINKER_FLAGS_RELEASE 
			"${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG"
		)
		set(EXTERNAL_LIBS 
			${EXTERNAL_LIBS}
			imagehlp.lib 
			Psapi.lib
		)
	endif(DEVELOPER_FEATURES AND WIN32)

endmacro(SET_EXECUTABLE_SPECIFICS)
