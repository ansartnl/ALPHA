get_filename_component(userproto_root_dir ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories(
  "${userproto_root_dir}/src"
  )

list(APPEND SOURCES
  "${userproto_root_dir}/src/usGroup.cpp"
  "${userproto_root_dir}/src/usGroups.cpp"
  "${userproto_root_dir}/src/usHelperFunctions.cpp"
  "${userproto_root_dir}/src/usProto.cpp"
  "${userproto_root_dir}/src/usTransport.cpp"
  "${userproto_root_dir}/src/usUser.cpp"
  "${userproto_root_dir}/src/usUsers.cpp"
  )

list(APPEND Q_HEADERS
  "${userproto_root_dir}/src/usTransport.h"
  )

list(APPEND HEADERS
  "${userproto_root_dir}/src/usDeclare.h"
  "${userproto_root_dir}/src/usGroup.h"
  "${userproto_root_dir}/src/usGroups.h"
  "${userproto_root_dir}/src/usHelperFunctions.h"
  "${userproto_root_dir}/src/usProto.h"
  "${userproto_root_dir}/src/usUser.h"
  "${userproto_root_dir}/src/usUsers.h"
  )

include(${userproto_root_dir}/../../enumstreaming/enumstreaming.cmake)  
