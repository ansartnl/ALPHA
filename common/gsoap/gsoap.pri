INCLUDEPATH     += $$PWD/gsoap $$PWD/gsoap/import $$PWD/gsoap/plugin
DEPENDPATH      += $$PWD/gsoap $$PWD/gsoap/import $$PWD/gsoap/plugin


linux-g++-32 {
    LIBS += -L$$PWD/lib/linux-x32 -lgsoap++
} else:linux-g++-64 {
    LIBS += -L$$PWD/lib/linux-x64 -lgsoap++
} else:linux-g++ {
    LIBS += -L$$PWD/lib/linux-x32 -lgsoap++
} else {
    greaterThan(QT_MAJOR_VERSION, 4) {
        LIBS += -L$$PWD/lib/win32-qt5 -lgsoap++ -lws2_32
    } else {
        LIBS += -L$$PWD/lib/win32 -lgsoap++ -lws2_32
    }
}
