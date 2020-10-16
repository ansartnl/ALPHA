get_filename_component(qtatimer_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qtatimer_CURRENTDIR}")

list(APPEND SOURCES "${qtatimer_CURRENTDIR}/QtATimer.cpp")

list(APPEND Q_HEADERS "${qtatimer_CURRENTDIR}/QtATimer.h")