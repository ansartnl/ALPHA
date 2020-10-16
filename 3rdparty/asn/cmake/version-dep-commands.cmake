
macro(QT45_ADD_RESOURCES outfiles)
	
	if(USE_QT5)
		qt5_add_resources(${outfiles} ${ARGN})
	else(USE_QT5)
		qt4_add_resources(${outfiles} ${ARGN})
	endif(USE_QT5)
	
endmacro(QT45_ADD_RESOURCES)


macro(QT45_WRAP_CPP outfiles)

	if(USE_QT5)
		qt5_wrap_cpp(${outfiles} ${ARGN})
	else(USE_QT5)
		qt4_wrap_cpp(${outfiles} ${ARGN})
	endif(USE_QT5)
	
endmacro(QT45_WRAP_CPP)


macro(QT45_WRAP_UI outfiles)

	if(USE_QT5)
		qt5_wrap_ui(${outfiles} ${ARGN})
	else(USE_QT5)
		qt4_wrap_ui(${outfiles} ${ARGN})
	endif(USE_QT5)
	
endmacro(QT45_WRAP_UI)

macro(QT45_ADD_DBUS_INTERFACE outfiles)

	if(USE_QT5)
		qt5_add_dbus_interface(${outfiles} ${ARGN})
	else(USE_QT5)
		qt4_add_dbus_interface(${outfiles} ${ARGN})
	endif(USE_QT5)
	
endmacro(QT45_ADD_DBUS_INTERFACE)


macro(QT45_ADD_DBUS_ADAPTOR outfiles)

	if(USE_QT5)
		qt5_add_dbus_adaptor(${outfiles} ${ARGN})
	else(USE_QT5)
		qt4_add_dbus_adaptor(${outfiles} ${ARGN})
	endif(USE_QT5)
	
endmacro(QT45_ADD_DBUS_ADAPTOR)


macro(QT45_CREATE_TRANSLATION _qm_files)

	if(USE_QT5)
		qt5_create_translation(${_qm_files} ${ARGN})
	else(USE_QT5)
		qt4_create_translation(${_qm_files} ${ARGN})
	endif(USE_QT5)
	
endmacro(QT45_CREATE_TRANSLATION)


macro(QT45_ADD_TRANSLATION _qm_files)

	if(USE_QT5)
		qt5_add_translation(${_qm_files} ${ARGN})
	else(USE_QT5)
		qt4_add_translation(${_qm_files} ${ARGN})
	endif(USE_QT5)
	
endmacro(QT45_ADD_TRANSLATION)


macro(FIND_QT)

	option(USE_QT5 "Use Qt5 instead of Qt4" OFF)

	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	set(CMAKE_AUTOMOC ON)
	
	if(USE_QT5)
		message("Looking for Qt5 ...")
		foreach(module ${ARGN})
			set(USE_QT_Main FALSE)
			message("    ${module}")
			if(NOT "${module}" STREQUAL "Main")
				find_package(Qt5${module} REQUIRED)
				if(NOT "${module}" STREQUAL "LinguistTools")
					set(QT_MODULES ${QT_MODULES} ${module})
				endif()
			else()
				set(USE_QT_Main TRUE)
			endif()
		endforeach(module)
	else(USE_QT5)
		message("Looking for Qt4 ...")
		find_package(Qt4 REQUIRED)
		foreach(module ${ARGN})
			if(NOT "${module}" STREQUAL "Core" AND NOT "${module}" STREQUAL "Widgets" AND NOT "${module}" STREQUAL "Gui" AND NOT "${module}" STREQUAL "LinguistTools")
				message("    ${module}")
				string(TOUPPER ${module} MODULE_UPPER)
				set(QT_USE_QT${MODULE_UPPER} TRUE)
			endif()
		endforeach(module)
		include(${QT_USE_FILE})
		add_definitions(${QT_DEFINITIONS})
	endif(USE_QT5)

	
endmacro(FIND_QT) 


macro(LINK_QT _project)

	if(USE_QT5)
		qt5_use_modules(${_project} ${QT_MODULES})
		if(${USE_QT_Main} STREQUAL "TRUE" AND MSVC)
			target_link_libraries(${_project} Qt5::WinMain)
		endif()
	else(USE_QT5)
		target_link_libraries(${_project} ${QT_LIBRARIES})
	endif(USE_QT5)

endmacro(LINK_QT)
