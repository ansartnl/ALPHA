INCLUDEPATH     += $$PWD/include
DEPENDPATH      += $$PWD/include

VERSION = 0.8.1

win32 {
    LIBS += -L$$DESTDIR -L$$PWD/bin -lqjson
} macx {
    QMAKE_LFLAGS += -F$$PWD/bin
    LIBS += -framework qjson
} unix:!macx {
    LIBS += -L$$DESTDIR -L$$PWD/bin -lqjson
}

PRE_TARGETDEPS += $$DESTDIR $$PWD/bin
