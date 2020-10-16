get_filename_component(redisconnectionsmanager_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${redisconnectionsmanager_CURRENTDIR}")

list(APPEND Q_HEADERS
        "${redisconnectionsmanager_CURRENTDIR}/redisdb.h"
        )

list(APPEND HEADERS
        "${redisconnectionsmanager_CURRENTDIR}/sectorredis.h"
        )

list(APPEND SOURCES
        "${redisconnectionsmanager_CURRENTDIR}/redisdb.cpp"
        "${redisconnectionsmanager_CURRENTDIR}/sectorredis.cpp"
        )