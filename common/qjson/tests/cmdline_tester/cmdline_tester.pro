TEMPLATE = app
TARGET =
CONFIG += link_prl
DEPENDPATH += .

# Input
SOURCES += cmdline_tester.cpp cmdlineparser.cpp

include (../../qjson.pri)
