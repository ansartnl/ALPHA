
macro(ADD_GROUP name folder)

	if(NOT "${name}" STREQUAL "")
		string(TOUPPER ${name} name_upper) 
		set(name_upper "_${name_upper}")
	else()
		set(name_upper "_MAIN")
	endif()
		
	foreach(source ${ARGN})
		string(REGEX MATCH "\\.h[px]*" header_match ${source})
		string(FIND ${source} "." header_pos2)
		string(REGEX MATCH "\\.inl" header_match3 ${source})
		string(REGEX MATCH "\\.c[px]*" source_match ${source})
		string(FIND ${source} ".ui" ui_pos)

		if(NOT "${header_match}" STREQUAL "")
			set(HEADERS${name_upper} ${HEADERS${name_upper}} ${source})
		elseif(${header_pos2} EQUAL -1)
			set(HEADERS${name_upper} ${HEADERS${name_upper}} ${source})
		elseif(NOT "${header_match3}" STREQUAL "")
			set(HEADERS${name_upper} ${HEADERS${name_upper}} ${source})
		elseif(NOT "${source_match}" STREQUAL "")
			set(SOURCES${name_upper} ${SOURCES${name_upper}} ${source})
		elseif(NOT ${ui_pos} EQUAL -1)
			set(FORMS${name_upper} ${FORMS${name_upper}} ${source})
		else()
			set(OTHERS${name_upper} ${OTHERS${name_upper}} ${source})
		endif()
	endforeach(source)
	
	set(HEADERS ${HEADERS} ${HEADERS${name_upper}})
	set(SOURCES ${SOURCES} ${SOURCES${name_upper}})
	set(FORMS ${FORMS} ${FORMS${name_upper}})
	set(OTHERS ${OTHERS} ${OTHERS${name_upper}})
	
	if("${folder}" STREQUAL "")
		source_group("sources" FILES ${HEADERS${name_upper}} ${SOURCES${name_upper}})
		source_group("forms" FILES ${FORMS${name_upper}})
	else()
		string(REPLACE "/" "\\" folder_ ${folder})
		source_group("sources\\${folder_}" FILES ${HEADERS${name_upper}} ${SOURCES${name_upper}})
		source_group("forms\\${folder_}" FILES ${FORMS${name_upper}})
		source_group("${folder_}" FILES ${OTHERS${name_upper}})
	endif()
	
endmacro(ADD_GROUP)


macro(IMPORT_GROUPS path folder_prefix)

	set(IMPORT_GROUPS_FOLDER_PREFIX ${folder_prefix})
	string(FIND ${path} "/" last_pos REVERSE)
	string(SUBSTRING ${path} 0 ${last_pos} IMPORT_PATH)
	include(${path})
	
endmacro(IMPORT_GROUPS)


macro(EXPORT_GROUP name folder)

	if(NOT DEFINED IMPORT_PATH)
		return()
	endif()
	
	if(DEFINED IMPORT_GROUPS_FOLDER_PREFIX)
		set(import_folder "${IMPORT_GROUPS_FOLDER_PREFIX}/${folder}")
	else()
		set(_folder ${folder})
	endif()
	
	foreach(source ${ARGN})
		set(EXPORTED_SOURCES ${EXPORTED_SOURCES} "${IMPORT_PATH}/${source}")
	endforeach()

	add_group(${name} ${import_folder} ${EXPORTED_SOURCES})

endmacro(EXPORT_GROUP)
