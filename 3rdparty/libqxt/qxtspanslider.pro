DEFINES += QXT_STATIC

TEMPLATE        = lib
CONFIG          += dll staticlib

TARGET          = QxtSpanSlider

win32{
    DESTDIR         = $$PWD/lib/win32
} else {
    DESTDIR         = $$PWD/lib
}

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}

INCLUDEPATH     +=  $$PWD/src/gui \
                    $$PWD/src/core

HEADERS         +=  $$PWD/src/gui/qxtspanslider_p.h \
                    $$PWD/src/gui/qxtspanslider.h

SOURCES         +=  $$PWD/src/gui/qxtspanslider.cpp
