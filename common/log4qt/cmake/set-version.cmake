
macro(SET_VERSION target major minor patch)

	string(TOUPPER ${target} project_upper)
	
	set(${project_upper}_MAJOR_VERSION ${major} CACHE STRING "${target} major version")
	set(${project_upper}_MINOR_VERSION ${minor} CACHE STRING "${target} minor version")
	set(${project_upper}_RELEASE ${patch} CACHE STRING "${target} release")
	
	set(${project_upper}_VERSION "${${project_upper}_MAJOR_VERSION}.${${project_upper}_MINOR_VERSION}.${${project_upper}_RELEASE}")

	if(WIN32)
		# set product version to linker
		set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /MANIFEST:NO /OPT:REF /OPT:ICF")
	endif(WIN32)
	
	add_definitions(
		-D${project_upper}_VERSION=${${project_upper}_VERSION} 
		-D${project_upper}_NAME="${target}"
		-D${project_upper}_VERSION_STR="${${project_upper}_VERSION}"
	)

endmacro(SET_VERSION)