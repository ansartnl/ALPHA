INCLUDEPATH += $$PWD/include

LIBS += -L$$PWD/lib

win32 {
    LIBS += -lOpenAL32
} else {
    LIBS += -lopenal
}
