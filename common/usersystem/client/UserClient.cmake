get_filename_component(userclient_root_dir ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories(
  "${userclient_root_dir}/src"
  "${userclient_root_dir}/src/dialogs"
  )

list(APPEND SOURCES
  "${userclient_root_dir}/src/DialogManager.cpp"
  "${userclient_root_dir}/src/UserClient.cpp"
  "${userclient_root_dir}/src/dialogs/AddUserDialog.cpp"
  "${userclient_root_dir}/src/dialogs/ChangePasswordDialog.cpp"
  "${userclient_root_dir}/src/dialogs/LoginDialog.cpp"
  "${userclient_root_dir}/src/dialogs/ManageUsersDialog.cpp"
  )

list(APPEND Q_HEADERS
  "${userclient_root_dir}/src/DialogManager.h"
  "${userclient_root_dir}/src/UserClient.h"
  "${userclient_root_dir}/src/dialogs/AddUserDialog.h"
  "${userclient_root_dir}/src/dialogs/ChangePasswordDialog.h"
  "${userclient_root_dir}/src/dialogs/LoginDialog.h"
  "${userclient_root_dir}/src/dialogs/ManageUsersDialog.h"
  )

list(APPEND FORMS
  "${userclient_root_dir}/src/dialogs/AddUserDialog.ui"
  "${userclient_root_dir}/src/dialogs/ChangePasswordDialog.ui"
  "${userclient_root_dir}/src/dialogs/LoginDialog.ui"
  "${userclient_root_dir}/src/dialogs/ManageUsersDialog.ui"
  )