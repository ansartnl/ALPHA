get_filename_component(dbconnectionsmanager_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${dbconnectionsmanager_CURRENTDIR}")

list(APPEND Q_HEADERS
        "${dbconnectionsmanager_CURRENTDIR}/dbconnectionsmanager.h"
        "${dbconnectionsmanager_CURRENTDIR}/databasesubscriber.h"
        )

list(APPEND SOURCES
        "${dbconnectionsmanager_CURRENTDIR}/dbconnectionsmanager.cpp"
        "${dbconnectionsmanager_CURRENTDIR}/databasesubscriber.cpp"
        )
