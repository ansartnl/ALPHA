
macro(DECLARE_TRANSLATABLES)
	
	option(GENERATE_TRANSLATIONS "Generate translation files (.qm)" OFF)

	foreach(_file ${ARGN})
		string(FIND ${_file} ".ts" ts_pos)
		if(NOT ${ts_pos} EQUAL -1)
			set(TRANSLATIONS ${TRANSLATIONS} ${_file})
		endif()
		set(TRANSLATABLES ${TRANSLATABLES} ${_file})
	endforeach(_file)
	
	if(NOT "${TRANSLATIONS}" STREQUAL "")
		source_group("translations" FILES ${TRANSLATIONS})
	endif()
	
endmacro(DECLARE_TRANSLATABLES)


macro(INSTALL_TRANSLATIONS target source_path)

	file(GLOB qm_files ${source_path}/*.qm)
	if(COMMAND copy_files)
		copy_files(${target} ${BIN_INSTALLATION_PATH}/translations "" ${qm_files})
	else()
		message(WARNING "Cannot find COPY_FILES command - translations installation incomplete")
	endif()
	
endmacro(INSTALL_TRANSLATIONS)
