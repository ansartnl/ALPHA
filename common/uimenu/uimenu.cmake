get_filename_component(uimenu_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(QT_USE_QTXML TRUE)

include_directories("${uimenu_CURRENTDIR}")

list(APPEND SOURCES 
    "${uimenu_CURRENTDIR}/UiItems.cpp"
    "${uimenu_CURRENTDIR}/UiModel.cpp"
    "${uimenu_CURRENTDIR}/UiBuilders.cpp"
    "${uimenu_CURRENTDIR}/UiXmlConfig.cpp"
    "${uimenu_CURRENTDIR}/RearrangeDlg.cpp"
    "${uimenu_CURRENTDIR}/AddActionDlg.cpp"
    "${uimenu_CURRENTDIR}/CellEditorDlg.cpp"
    "${uimenu_CURRENTDIR}/RearrangeModels.cpp"
    "${uimenu_CURRENTDIR}/ActionUiGroup.cpp"
	)

list(APPEND Q_HEADERS 
    "${uimenu_CURRENTDIR}/RearrangeDlg.h"
    "${uimenu_CURRENTDIR}/AddActionDlg.h"
    "${uimenu_CURRENTDIR}/CellEditorDlg.h"
    "${uimenu_CURRENTDIR}/ActionUiGroup.h"
	)

list(APPEND HEADERS 
    "${uimenu_CURRENTDIR}/UiItems.h"
    "${uimenu_CURRENTDIR}/AbstractUiModel.h"
    "${uimenu_CURRENTDIR}/UiModel.h"
    "${uimenu_CURRENTDIR}/UiBuilders.h"
    "${uimenu_CURRENTDIR}/UiXmlConfig.h"
    "${uimenu_CURRENTDIR}/RearrangeModels.h"
    "${uimenu_CURRENTDIR}/CommandDelegate.h"
	)

list(APPEND FORMS
    "${uimenu_CURRENTDIR}/RearrangeDlg.ui"
    "${uimenu_CURRENTDIR}/AddActionDlg.ui"
    "${uimenu_CURRENTDIR}/CellEditorDlg.ui"
	)
	
list(APPEND RESOURCES
	"${uimenu_CURRENTDIR}/uimenu.qrc"
	)
