
PROJECT                 = qtserialport
TEMPLATE                = lib

CONFIG                 -= debug_and_release
#CONFIG                 += debug
CONFIG                 += release
CONFIG                 += warn_on qt thread
QT                     -= gui

OBJECTS_DIR             = build/obj
MOC_DIR                 = build/moc
DEPENDDIR               = .
INCLUDEDIR              = .
HEADERS                 = qextserialbase.h \
                          qtserialport.h
SOURCES                 = qextserialbase.cpp \
                          qtserialport.cpp

unix:HEADERS           += posix_qextserialport.h
unix:SOURCES           += posix_qextserialport.cpp
unix:DEFINES           += _TTY_POSIX_

win32:HEADERS          += win_qextserialport.h
win32:SOURCES          += win_qextserialport.cpp
win32:DEFINES          += _TTY_WIN_

DESTDIR                 = build
TARGET                  = qtserialport
unix:VERSION            = 1.0.0
