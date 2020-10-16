get_filename_component(aftnws_login_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${aftnws_login_CURRENTDIR}")

list(APPEND Q_HEADERS "${aftnws_login_CURRENTDIR}/src/gui/dialog/logindialog.h")

list(APPEND SOURCES "${aftnws_login_CURRENTDIR}/src/gui/dialog/logindialog.cpp")

list(APPEND FORMS  "${aftnws_login_CURRENTDIR}/src/gui/dialog/logindialog.ui")
