
macro(SET_DEFAULT_DEFINITIONS)

	add_definitions(
		-DWIN32_LEAN_AND_MEAN # sockets 2.0
		-DNOMINMAX # do not define
		-D_CRT_SECURE_NO_WARNINGS 
		-D_CRT_NONSTDC_NO_WARNINGS 
	)

endmacro(SET_DEFAULT_DEFINITIONS)


macro(SET_DEBUG_POSTFIX postfix)

	set(COMMON_DEBUG_POSTFIX ${postfix})

endmacro(SET_DEBUG_POSTFIX)


macro(PREPARE_BUILD_COMPONENTS)

	if(DEFINED FORMS)
		if(NOT "${FORMS}" STREQUAL "")
			qt45_wrap_ui(UI_FILES ${FORMS})
			set(GENERATED_FILES ${GENERATED_FILES} ${UI_FILES})
		endif()
	endif()
	
	if(DEFINED RESOURCES_QT)
		if(NOT "${RESOURCES_QT}" STREQUAL "")
			qt45_add_resources(QRC_RESOURCES ${RESOURCES_QT})
			set(GENERATED_FILES ${GENERATED_FILES} ${QRC_RESOURCES})
		endif()
	endif()
	
	if(DEFINED TRANSLATABLES)
		if(NOT "${TRANSLATABLES}" STREQUAL "")
			if(GENERATE_TRANSLATIONS)
				qt45_create_translation(QM_FILES ${TRANSLATABLES} OPTIONS -no-obsolete)
				message("Generating translations ...")
			endif(GENERATE_TRANSLATIONS)
			qt45_add_translation(QM_FILES ${TRANSLATIONS})
			set(GENERATED_FILES ${GENERATED_FILES} ${QM_FILES})
		endif()
	endif()

	source_group(".generated" FILES ${GENERATED_FILES})
	
	set(BUILD_COMPONENTS
		${EXEC_EXTRA_PARAMS}
		${HEADERS}
		${SOURCES}
		${FORMS}
		${RESOURCES}
		${TRANSLATABLES}
		${OTHERS}
		${GENERATED_FILES}
	)

endmacro(PREPARE_BUILD_COMPONENTS)


macro(CONFIGURE_BUILD target)

	if(DEFINED COMMON_DEBUG_POSTFIX)
		set_target_properties(${target} PROPERTIES DEBUG_POSTFIX ${COMMON_DEBUG_POSTFIX})
	endif()
	
	if(DEFINED WIN_MANIFEST)
		if(WIN32 AND NOT ${CMAKE_GENERATOR} STREQUAL "NMake Makefiles")
			add_custom_command(TARGET ${target} POST_BUILD COMMAND
				mt -manifest "${CMAKE_CURRENT_SOURCE_DIR}/../resources/win7.manifest" -outputresource:"$(TargetDir)$(TargetFileName)"\;\#1
			)
		endif(WIN32 AND NOT ${CMAKE_GENERATOR} STREQUAL "NMake Makefiles")
	endif()
			
endmacro(CONFIGURE_BUILD)


macro(LINK_LIBRARIES_COMMON target)

	foreach(library ${ARGN})
		if(MINGW)
			target_link_libraries(${target} debug lib${library}${COMMON_DEBUG_POSTFIX}.dll optimized lib${library}.dll)
		else()
			target_link_libraries(${target} debug ${library}${COMMON_DEBUG_POSTFIX} optimized ${library})
		endif()
	endforeach(library)

	add_dependencies(${target} ${ARGN})

endmacro(LINK_LIBRARIES_COMMON) 