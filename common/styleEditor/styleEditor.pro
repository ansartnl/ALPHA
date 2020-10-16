#-------------------------------------------------
#
# Project created by QtCreator 2012-06-13T13:57:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = styleEditor

CONFIG(debug, debug|release) : TARGET = $$join(TARGET,,,_debug)

TEMPLATE = app

DESTDIR = $$PWD/bin
 


SOURCES  += main.cpp\
            MainWindow.cpp \
            QtColorButton.cpp \
            ColorWidget.cpp

HEADERS  += MainWindow.h \
            QtColorButton.h \
            ColorWidget.h

FORMS    += MainWindow.ui \
            ColorWidget.ui

include (ExtendedStyle.pri)
