get_filename_component(usersystem_root_dir ${CMAKE_CURRENT_LIST_FILE} PATH)

include(${usersystem_root_dir}/proto/UserProto.cmake)
include(${usersystem_root_dir}/client/UserClient.cmake)
include(${usersystem_root_dir}/server/UserServer.cmake)
