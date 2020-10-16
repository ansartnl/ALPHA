get_filename_component(QTDELEGATES_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${QTDELEGATES_CURRENTDIR}")
    
if(NOT COMMAND export_group)
	list(APPEND HEADERS 
		"${QTDELEGATES_CURRENTDIR}/qtcheckboxdelegate.h"
		"${QTDELEGATES_CURRENTDIR}/coordedit.h"
		"${QTDELEGATES_CURRENTDIR}/coorddelegate.h"
		"${QTDELEGATES_CURRENTDIR}/SimpleComboboxDelegate.h"
		"${QTDELEGATES_CURRENTDIR}/htmldelegate.h")
	list(APPEND SOURCES 
		"${QTDELEGATES_CURRENTDIR}/qtcheckboxdelegate.cpp"
		"${QTDELEGATES_CURRENTDIR}/coordedit.cpp"
		"${QTDELEGATES_CURRENTDIR}/coorddelegate.cpp"
		"${QTDELEGATES_CURRENTDIR}/SimpleComboboxDelegate.cpp")
else()
	export_group("qtdelegates" "qtdelegates" 
		qtcheckboxdelegate.h
		qtcheckboxdelegate.cpp
		coordedit.h
		coordedit.cpp
		coorddelegate.h
		coorddelegate.cpp
		SimpleComboboxDelegate.h
		SimpleComboboxDelegate.cpp
		htmldelegate.h
	)
endif()