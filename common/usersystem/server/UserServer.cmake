get_filename_component(userserver_root_dir ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories(
  "${userserver_root_dir}/src"
  )

list(APPEND SOURCES
  "${userserver_root_dir}/src/UserServer.cpp"
  )

list(APPEND Q_HEADERS
  "${userserver_root_dir}/src/UserServer.h"
  )
