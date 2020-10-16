get_filename_component(HTTPPING_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${HTTPPING_CURRENTDIR}")

list(APPEND Q_HEADERS "${HTTPPING_CURRENTDIR}/HttpPingGlobal.h")
list(APPEND Q_HEADERS "${HTTPPING_CURRENTDIR}/HttpPing.h")
list(APPEND Q_HEADERS "${HTTPPING_CURRENTDIR}/HttpPingPrivate.h")
list(APPEND Q_HEADERS "${HTTPPING_CURRENTDIR}/HttpPinger.h")

list(APPEND SOURCES "${HTTPPING_CURRENTDIR}/HttpPing.cpp")
list(APPEND SOURCES "${HTTPPING_CURRENTDIR}/HttpPingPrivate.cpp")
list(APPEND SOURCES "${HTTPPING_CURRENTDIR}/HttpPinger.cpp")