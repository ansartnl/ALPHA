INCLUDEPATH += $$PWD

INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

win32 {
	HEADERS += $$PWD/xgetopt.h
	SOURCES += $$PWD/xgetopt.cpp
}

HEADERS += \
    $$PWD/CmdLineArguments.h

SOURCES += \
    $$PWD/CmdLineArguments.cpp
