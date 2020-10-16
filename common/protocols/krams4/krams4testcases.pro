# -------------------------------------------------
# Project created by QtCreator 2010-10-15T18:06:13
# -------------------------------------------------
TARGET = Krams4UnitTest
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}

DESTDIR = $$PWD/bin
CONFIG += console
CONFIG += qtestlib
CONFIG -= app_bundle
TEMPLATE = app
INCLUDEPATH += src \
    tests

SOURCES += tests/main.cpp \
    tests/kramstest.cpp

HEADERS += tests/kramstest.h

include(krams4export.pri)
