#-------------------------------------------------
#
# Project created by QtCreator 2013-02-22T15:03:48
#
#-------------------------------------------------

QT       += core gui
CONFIG(debug, debug|release) : TARGET = $$join(TARGET,,,_debug)

DESTDIR = $$PWD/bin

TARGET = recoverytool
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/models/FileListModel.cpp \
    src/models/FileListFilterModel.cpp \
    src/CopyDialog.cpp \
    src/FileCopy.cpp

HEADERS  += src/mainwindow.h \
    src/main.h \
    src/models/FileListModel.h \
    src/models/FileListFilterModel.h \
    src/CopyDialog.h \
    src/FileCopy.h

FORMS    += src/mainwindow.ui \
    src/copydialog.ui

include(../initmain/initmain.pri)
include(../qtxmlsettings/qtxmlsettings.pri)
include(../qtlanguagemenu/qtlanguagemenu.pri)

TRANSLATIONS = share/translations/RecoveryTool_ru.ts
