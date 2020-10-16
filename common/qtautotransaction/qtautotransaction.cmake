get_filename_component(QTAUTOTRANSACTION_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${QTAUTOTRANSACTION_CURRENTDIR}")
    
if(NOT COMMAND export_group)
	list(APPEND HEADERS "${QTAUTOTRANSACTION_CURRENTDIR}/qtautotransaction.h")
	list(APPEND SOURCES "${QTAUTOTRANSACTION_CURRENTDIR}/qtautotransaction.cpp")
else()
	export_group("qtautotransaction" "qtautotransaction" qtautotransaction.h qtautotransaction.cpp)
endif()