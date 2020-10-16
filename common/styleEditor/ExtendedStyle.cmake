get_filename_component(EXTENDED_STYLE_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${EXTENDED_STYLE_CURRENTDIR}/ExtendedStyle")
    
if(NOT COMMAND export_group)
	list(APPEND HEADERS "${EXTENDED_STYLE_CURRENTDIR}/ExtendedStyle/ExtendedStyle.h")
	list(APPEND SOURCES "${EXTENDED_STYLE_CURRENTDIR}/ExtendedStyle/ExtendedStyle.cpp")
else()
	export_group("ExtendedStyle" "ExtendedStyle" ExtendedStyle/ExtendedStyle.h ExtendedStyle/ExtendedStyle.cpp)
endif()