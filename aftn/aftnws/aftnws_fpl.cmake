get_filename_component(aftnws_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${aftnws_CURRENTDIR}")

list(APPEND HEADERS "${aftnws_CURRENTDIR}/src/global/printutils.h")
list(APPEND HEADERS "${aftnws_CURRENTDIR}/src/core/fpl.h")
list(APPEND HEADERS "${aftnws_CURRENTDIR}/src/core/rpl.h")
list(APPEND HEADERS "${aftnws_CURRENTDIR}/src/core/notam.h")
list(APPEND HEADERS "${aftnws_CURRENTDIR}/src/data/AFTNDao.h")

list(APPEND Q_HEADERS "${aftnws_CURRENTDIR}/src/gui/browser/abstractbrowser.h")
list(APPEND Q_HEADERS "${aftnws_CURRENTDIR}/src/gui/dialog/abstractdialog.h")
list(APPEND Q_HEADERS "${aftnws_CURRENTDIR}/src/gui/editor/fpleditor.h")
list(APPEND Q_HEADERS "${aftnws_CURRENTDIR}/src/kernel/models/proxy/sectionsproxymodel.h")
list(APPEND Q_HEADERS "${aftnws_CURRENTDIR}/src/kernel/views/qtsqltableview.h")
list(APPEND Q_HEADERS "${aftnws_CURRENTDIR}/src/kernel/views/qtheaderview.h")
list(APPEND Q_HEADERS "${aftnws_CURRENTDIR}/src/gui/custom/customtextedit.h")
list(APPEND Q_HEADERS "${aftnws_CURRENTDIR}/src/global/translations.h")
list(APPEND Q_HEADERS "${aftnws_CURRENTDIR}/src/gui/browser/fplbrowser.h")

list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/gui/browser/abstractbrowser.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/global/printutils.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/gui/dialog/abstractdialog.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/core/fpl.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/core/rpl.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/core/notam.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/data/AFTNDao.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/gui/editor/fpleditor.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/kernel/models/proxy/sectionsproxymodel.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/kernel/views/qtsqltableview.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/kernel/views/qtheaderview.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/gui/custom/customtextedit.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/global/translations.cpp")
list(APPEND SOURCES "${aftnws_CURRENTDIR}/src/gui/browser/fplbrowser.cpp")

list(APPEND FORMS  "${aftnws_CURRENTDIR}/src/gui/editor/fpleditor.ui")
list(APPEND FORMS  "${aftnws_CURRENTDIR}/src/gui/browser/fplbrowser.ui")
