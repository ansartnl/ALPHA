get_filename_component(qserialdevice_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qserialdevice_CURRENTDIR}")

list(APPEND SOURCES 
	"${qserialdevice_CURRENTDIR}/abstractserial.cpp"
	"${qserialdevice_CURRENTDIR}/abstractserialengine.cpp"
	"${qserialdevice_CURRENTDIR}/nativeserialengine.cpp"
	"${qserialdevice_CURRENTDIR}/abstractserialnotifier.cpp"
	)

list(APPEND HEADERS
	"${qserialdevice_CURRENTDIR}/abstractserial.h"
    "${qserialdevice_CURRENTDIR}/abstractserial_p.h"
    "${qserialdevice_CURRENTDIR}/abstractserialengine.h"
    "${qserialdevice_CURRENTDIR}/abstractserialengine_p.h"
    "${qserialdevice_CURRENTDIR}/nativeserialengine.h"
    "${qserialdevice_CURRENTDIR}/nativeserialengine_p.h"
    "${qserialdevice_CURRENTDIR}/abstractserialnotifier.h"
    "${qserialdevice_CURRENTDIR}/nativeserialnotifier.h"
	)

if(WIN32)
  list(APPEND SOURCES
	  "${qserialdevice_CURRENTDIR}/nativeserialengine_win.cpp"
	  "${qserialdevice_CURRENTDIR}/nativeserialnotifier_win.cpp"
	)
else(WIN32)
  list(APPEND HEADERS
	  "${qserialdevice_CURRENTDIR}/ttylocker.h"
	  )
  list(APPEND SOURCES
	  "${qserialdevice_CURRENTDIR}/nativeserialengine_unix.cpp"
	  "${qserialdevice_CURRENTDIR}/ttylocker.cpp"
	  "${qserialdevice_CURRENTDIR}/nativeserialnotifier_unix.cpp"
	  )
endif(WIN32)
