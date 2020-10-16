get_filename_component(QTLANGUAGEMENU_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${QTLANGUAGEMENU_CURRENTDIR}")

add_definitions(-DQT_TRANSLATOR_INCLUDED)    

if(NOT COMMAND export_group)
	list(APPEND HEADERS 
		"${QTLANGUAGEMENU_CURRENTDIR}/qtlanguagemenu.h"
		"${QTLANGUAGEMENU_CURRENTDIR}/qttranslator.h")
	list(APPEND SOURCES 
		"${QTLANGUAGEMENU_CURRENTDIR}/qtlanguagemenu.cpp"
		"${QTLANGUAGEMENU_CURRENTDIR}/qttranslator.cpp")
else()
	export_group("qtlanguagemenu" "qtlanguagemenu" 
		qtlanguagemenu.h 
		qtlanguagemenu.cpp 
		qttranslator.h
		qttranslator.cpp
	)
endif()