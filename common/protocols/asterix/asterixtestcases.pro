# -------------------------------------------------
# Project created by QtCreator 2010-10-15T18:06:13
# -------------------------------------------------
TARGET = AsterixUnitTest
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
    tests/asterixtest.cpp \
    tests/asterix008ut.cpp
HEADERS += tests/asterixtest.h \
    tests/asterixFuncs.h \
    tests/helperFuncs.h \

include(asterixexport.pri)
