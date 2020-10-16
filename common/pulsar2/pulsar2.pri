INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

HEADERS += $$PWD/Pulsar.h \
    $$PWD/PulsReceiverPrivate.h \
    $$PWD/PulsarPrivate.h \
    $$PWD/PulsReceiver.h \
    $$PWD/PulsarShareFunctions.h \
    $$PWD/PulsData.h

SOURCES += $$PWD/Pulsar.cpp \
    $$PWD/PulsReceiverPrivate.cpp \
    $$PWD/PulsarPrivate.cpp \
    $$PWD/PulsReceiver.cpp \
    $$PWD/PulsarShareFunctions.cpp \
    $$PWD/PulsData.cpp

include(../compare/compare.pri)
include(../buildvariation/buildvariation.pri)
