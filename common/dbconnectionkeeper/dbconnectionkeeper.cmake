get_filename_component(dbconnectionkeeper_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${dbconnectionkeeper_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND SOURCES "${dbconnectionkeeper_CURRENTDIR}/dbconnectionkeeper.cpp")
	list(APPEND Q_HEADERS "${dbconnectionkeeper_CURRENTDIR}/dbconnectionkeeper.h")
else()
	export_group("dbconnectionkeeper" "dbconnectionkeeper" dbconnectionkeeper.h dbconnectionkeeper.cpp)
endif()