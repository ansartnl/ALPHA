get_filename_component(waitcursor_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${waitcursor_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND SOURCES "${waitcursor_CURRENTDIR}/waitcursor.cpp")
	list(APPEND HEADERS "${waitcursor_CURRENTDIR}/waitcursor.h")
else()
	export_group("waitcursor" "waitcursor" waitcursor.h waitcursor.cpp)
endif()