INCLUDEPATH += $$PWD/src

LIBS += -L$$PWD/lib

win32 {
    CONFIG(debug, debug|release) : LIBS += -lqwtd
    CONFIG(release, debug|release) : LIBS += -lqwt
} else {
    LIBS += -lqwt
}
