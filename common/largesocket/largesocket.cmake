get_filename_component(largesocket_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${largesocket_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND HEADERS
			"${largesocket_CURRENTDIR}/LargeData.h"
			"${largesocket_CURRENTDIR}/LargeReceiver.h"
			"${largesocket_CURRENTDIR}/LargeSender.h"
			"${largesocket_CURRENTDIR}/LargeServer.h"
			"${largesocket_CURRENTDIR}/LargeSocket.h"
			)
	
	list(APPEND SOURCES
			"${largesocket_CURRENTDIR}/LargeData.cpp"
			"${largesocket_CURRENTDIR}/LargeReceiver.cpp"
			"${largesocket_CURRENTDIR}/LargeSender.cpp"
			"${largesocket_CURRENTDIR}/LargeServer.cpp"
			"${largesocket_CURRENTDIR}/LargeSocket.cpp"
			)
else()
	export_group("largesocket" "largesocket" 
		LargeData.h
		LargeData.cpp
		LargeReceiver.h
		LargeReceiver.cpp
		LargeSender.h
		LargeSender.cpp
		LargeServer.h
		LargeServer.cpp
		LargeSocket.h
		LargeSocket.cpp
	)
endif()			
