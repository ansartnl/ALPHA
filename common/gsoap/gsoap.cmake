get_filename_component(gsoap_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories(
	"${gsoap_CURRENTDIR}/gsoap"
	"${gsoap_CURRENTDIR}/gsoap/import"
	"${gsoap_CURRENTDIR}/gsoap/plugin"
)

if(UNIX)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		link_directories("${gsoap_CURRENTDIR}/lib/linux-x64")
	else()
		link_directories("${gsoap_CURRENTDIR}/lib/linux-x32")
	endif()
endif()
