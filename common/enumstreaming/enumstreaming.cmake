get_filename_component(enumstreaming_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${enumstreaming_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND HEADERS "${enumstreaming_CURRENTDIR}/enumstreaming.h")
else()
	export_group("enumstreaming" "enumstreaming" enumstreaming.h)
endif()