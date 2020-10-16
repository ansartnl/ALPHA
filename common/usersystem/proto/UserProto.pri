INCLUDEPATH += $$PWD/src

DEPENDPATH += $$PWD/src

HEADERS += \
    $$PWD/src/usDeclare.h \
    $$PWD/src/usProto.h \
    $$PWD/src/usUser.h \
    $$PWD/src/usHelperFunctions.h \
    $$PWD/src/usGroup.h \
    $$PWD/src/usGroups.h \
    $$PWD/src/usTransport.h \
    $$PWD/src/usUsers.h

SOURCES += \
    $$PWD/src/usProto.cpp \
    $$PWD/src/usUser.cpp \
    $$PWD/src/usHelperFunctions.cpp \
    $$PWD/src/usGroup.cpp \
    $$PWD/src/usGroups.cpp \
    $$PWD/src/usTransport.cpp \
    $$PWD/src/usUsers.cpp

include(../../enumstreaming/enumstreaming.pri)
