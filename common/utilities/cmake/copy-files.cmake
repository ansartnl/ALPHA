
macro(COPY_FILES target output_path input_path)
	
	add_custom_command(TARGET ${target}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${output_path}
		COMMENT "creating directory ... ${output_path}"
	)
	foreach(_file ${ARGN})
		add_custom_command(TARGET ${target}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${input_path}${_file} ${output_path}
			COMMENT "copying ${_file} to ... ${output_path}"
		)
	endforeach(_file)
	
endmacro(COPY_FILES)
