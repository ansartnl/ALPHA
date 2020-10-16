get_filename_component(dbsettings_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${dbsettings_CURRENTDIR}")

list(APPEND SOURCES "${dbsettings_CURRENTDIR}/DBSettings.cpp")

list(APPEND Q_HEADERS "${dbsettings_CURRENTDIR}/DBSettings.h")
