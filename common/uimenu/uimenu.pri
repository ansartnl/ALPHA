QT += xml

INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

SOURCES += \
    $$PWD/UiItems.cpp \
    $$PWD/UiModel.cpp \
    $$PWD/UiBuilders.cpp \
    $$PWD/UiXmlConfig.cpp \
    $$PWD/RearrangeDlg.cpp \
    $$PWD/AddActionDlg.cpp \
    $$PWD/CellEditorDlg.cpp \
    $$PWD/RearrangeModels.cpp \
    $$PWD/ActionUiGroup.cpp

HEADERS += \
    $$PWD/AbstractUiModel.h \
    $$PWD/UiItems.h \
    $$PWD/UiModel.h \
    $$PWD/UiBuilders.h \
    $$PWD/UiXmlConfig.h \
    $$PWD/RearrangeDlg.h \
    $$PWD/AddActionDlg.h \
    $$PWD/CellEditorDlg.h \
    $$PWD/RearrangeModels.h \
    $$PWD/CommandDelegate.h \
    $$PWD/ActionUiGroup.h

RESOURCES += $$PWD/uimenu.qrc

FORMS += \
    $$PWD/RearrangeDlg.ui \
    $$PWD/AddActionDlg.ui \
    $$PWD/CellEditorDlg.ui
