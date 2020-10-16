
macro(INSTALL_TRANSLATIONS ProjectName)

	set(QM_FILES ${ARGV})
	list(REMOVE_ITEM QM_FILES ${ProjectName})

	if(WIN32)
		add_custom_command(TARGET ${ProjectName} COMMAND
				if not exist $(TargetDir)Translations\\ mkdir $(TargetDir)Translations\\
		)
		foreach(FILENAME ${QM_FILES})
			add_custom_command(TARGET ${ProjectName} COMMAND
				copy \"${FILENAME}\" $(TargetDir)Translations\\
			)
		endforeach(FILENAME)
	endif(WIN32)

	if(APPLE)
		get_target_property(projLocation ${ProjectName} LOCATION)
		string(REPLACE "/Contents/MacOS/${ProjectName}" "" MACOSX_BUNDLE_LOCATION ${projLocation})
		set(maxosxTranslatinosLocation
			${MACOSX_BUNDLE_LOCATION}/Contents/Resources/Translations
		)

		set(POST_BUILD_SHELL ${CMAKE_CURRENT_BINARY_DIR}/postBuildShell)
		write_file(${POST_BUILD_SHELL} "#!/bin/sh\nif [ ! -d $1 ] ; then mkdir $1 ; fi ;")

		add_custom_command(TARGET ${ProjectName} PRE_BUILD COMMAND chmod +x ${POST_BUILD_SHELL})
		add_custom_command(TARGET ${ProjectName} COMMAND ${POST_BUILD_SHELL} "${maxosxTranslatinosLocation}")

		foreach(FILENAME ${QM_FILES})
			add_custom_command(TARGET ${ProjectName} COMMAND
				cp \"${FILENAME}\" ${maxosxTranslatinosLocation}
			)
		endforeach(FILENAME)
	endif(APPLE)

endmacro(INSTALL_TRANSLATIONS)
