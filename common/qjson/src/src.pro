QJSON_BASE = $$PWD/..
QJSON_SRCBASE = .

TEMPLATE = lib
QT -= gui
TARGET = qjson
CONFIG += create_prl

greaterThan(QT_MAJOR_VERSION, 4) {
    QMAKE_CXXFLAGS += -std=c++11
}

isEmpty( DESTDIR ) {
    DESTDIR = $$QJSON_BASE/bin
}

VERSION = 0.8.1

windows: {
  DEFINES += QJSON_MAKEDLL
}

macx: CONFIG += lib_bundle

QJSON_CPP = $$QJSON_SRCBASE
INCLUDEPATH += $$QJSON_CPP

PRIVATE_HEADERS += \
  json_parser.hh \
  json_scanner.h \
  location.hh \
  parser_p.h \
  position.hh \
  qjson_debug.h \
  stack.hh

PUBLIC_HEADERS += \
  parser.h \
  parserrunnable.h \
  qobjecthelper.h \
  serializer.h \
  serializerrunnable.h \
  qjson_export.h

HEADERS += $$PRIVATE_HEADERS $$PUBLIC_HEADERS

SOURCES += \
  json_parser.cc \
  json_scanner.cpp \
  parser.cpp \
  parserrunnable.cpp \
  qobjecthelper.cpp \
  serializer.cpp \
  serializerrunnable.cpp
