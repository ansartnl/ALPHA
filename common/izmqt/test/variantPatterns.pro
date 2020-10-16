#-------------------------------------------------
#
# Project created by QtCreator 2013-01-11T10:02:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = variantPatterns
TEMPLATE = app

DESTDIR = $$PWD/bin


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

include(texteditappender/texteditappender.pri)
include(../izmqt.pri)
include(../src/autoconnector/autoconnector.pri)
include(../src/autoconnector/asyncconnector.pri)
include(../../log4qt/src/Log4Qt.pri)
include(../../log/log.pri)
