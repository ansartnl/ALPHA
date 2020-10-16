INCLUDEPATH += $$PWD
HEADERS += $$PWD/qextserialbase.h \
    $$PWD/qtserialport.h
SOURCES += $$PWD/qextserialbase.cpp \
    $$PWD/qtserialport.cpp
unix:HEADERS += $$PWD/posix_qextserialport.h
unix:SOURCES += $$PWD/posix_qextserialport.cpp
unix:DEFINES += _TTY_POSIX_
win32:HEADERS += $$PWD/win_qextserialport.h
win32:SOURCES += $$PWD/win_qextserialport.cpp
win32:DEFINES += _TTY_WIN_
