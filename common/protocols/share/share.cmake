get_filename_component(SHARE_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${SHARE_CURRENTDIR}")

list(APPEND SOURCES "${SHARE_CURRENTDIR}/presentdata.cpp")

list(APPEND HEADERS "${SHARE_CURRENTDIR}/presentdata.h"
	"${SHARE_CURRENTDIR}/shareutility.h"
	)