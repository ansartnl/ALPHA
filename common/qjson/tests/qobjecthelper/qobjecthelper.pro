CONFIG += qtestlib
CONFIG += link_prl
DEFINES += QMAKE_BUILD

QMAKE_CXXFLAGS += -Wno-ignored-qualifiers

HEADERS = person.h

SOURCES = testqobjecthelper.cpp \
          person.cpp

include (../../qjson.pri)
