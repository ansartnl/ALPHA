
macro(DECLARE_RESOURCES)

	foreach(resource ${ARGN})
		string(FIND ${resource} ".qrc" qrc_pos)
		string(FIND ${resource} ".rc" winrc_pos)
		if(NOT ${qrc_pos} EQUAL -1)	
			set(RESOURCES_QT ${RESOURCES_QT} ${resource})
		elseif(NOT ${winrc_pos} EQUAL -1)	
			set(RESOURCES_WIN ${RESOURCES_WIN} ${resource})
		endif()
	endforeach(resource)
	
	set(RESOURCES ${RESOURCES} ${RESOURCES_QT} ${RESOURCES_WIN})
	if(NOT "${RESOURCES}" STREQUAL "")
		source_group("resources" FILES ${RESOURCES})
	endif()
		
endmacro(DECLARE_RESOURCES)