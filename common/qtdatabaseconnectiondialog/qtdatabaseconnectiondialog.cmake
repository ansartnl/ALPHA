get_filename_component(qtdatabaseconnectiondialog_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qtdatabaseconnectiondialog_CURRENTDIR}")

list(APPEND SOURCES "${qtdatabaseconnectiondialog_CURRENTDIR}/qtdatabaseconnectiondialog.cpp"
  "${qtdatabaseconnectiondialog_CURRENTDIR}/qtdatabaseconnector.cpp")

list(APPEND Q_HEADERS "${qtdatabaseconnectiondialog_CURRENTDIR}/qtdatabaseconnectiondialog.h"
  "${qtdatabaseconnectiondialog_CURRENTDIR}/qtdatabaseconnector.h")

list(APPEND FORMS "${qtdatabaseconnectiondialog_CURRENTDIR}/qtdatabaseconnectiondialog.ui")