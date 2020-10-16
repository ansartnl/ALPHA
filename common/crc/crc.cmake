get_filename_component(crc_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${crc_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND HEADERS "${crc_CURRENTDIR}/CRC.h" "${crc_CURRENTDIR}/lib_crc.h")
	list(APPEND SOURCES "${crc_CURRENTDIR}/CRC.cpp" "${crc_CURRENTDIR}/lib_crc.c")
else()
	export_group("crc" "crc" CRC.h CRC.cpp lib_crc.c lib_crc.h)
endif()