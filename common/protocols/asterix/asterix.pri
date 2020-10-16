INCLUDEPATH += $$PWD/src

SOURCES += $$PWD/src/asterixdata.cpp \
    $$PWD/src/asterixerrors.cpp \
    $$PWD/src/asterix.cpp \
    $$PWD/src/asterix001.cpp \
    $$PWD/src/asterix002.cpp \
    $$PWD/src/asterix004.cpp \
    $$PWD/src/asterix008.cpp \
    $$PWD/src/asterix020.cpp \
    $$PWD/src/asterix021.cpp \
    $$PWD/src/asterix034.cpp \
    $$PWD/src/asterix048.cpp \
    $$PWD/src/asterix062.cpp \
    $$PWD/src/asterix063.cpp \
# Eurocontrol standard
    $$PWD/src/eurocontrol/asterix020euro.cpp \
    $$PWD/src/eurocontrol/asterix253euro.cpp \
# Aerotechnica standard
    $$PWD/src/aerotechnica/asterix001aero.cpp \
    $$PWD/src/aerotechnica/asterix002aero.cpp \
    $$PWD/src/aerotechnica/asterix062aero.cpp \
    $$PWD/src/aerotechnica/asterix250aero.cpp \
    $$PWD/src/aerotechnica/asterix253aero.cpp \
    $$PWD/src/aerotechnica/asterix254aero.cpp \
# Peleng standard
    $$PWD/src/peleng/asterix062peleng.cpp \
    $$PWD/src/peleng/asterix241peleng.cpp \
    $$PWD/src/peleng/asterix253peleng.cpp

HEADERS += $$PWD/src/asterixdata.h \
    $$PWD/src/asterixexport.h \
    $$PWD/src/asterixerrors.h \
    $$PWD/src/asterix.h \
    $$PWD/src/asterixvariety.h \
    $$PWD/src/asterix001.h \
    $$PWD/src/asterix002.h \
    $$PWD/src/asterix004.h \
    $$PWD/src/asterix008.h \
    $$PWD/src/asterix020.h \
    $$PWD/src/asterix021.h \
    $$PWD/src/asterix034.h \
    $$PWD/src/asterix048.h \
    $$PWD/src/asterix062.h \
    $$PWD/src/asterix063.h \
# Eurocontrol standard
    $$PWD/src/eurocontrol/asterix001euro.h \
    $$PWD/src/eurocontrol/asterix002euro.h \
    $$PWD/src/eurocontrol/asterix004euro.h \
    $$PWD/src/eurocontrol/asterix008euro.h \
    $$PWD/src/eurocontrol/asterix020euro.h \
    $$PWD/src/eurocontrol/asterix021euro.h \
    $$PWD/src/eurocontrol/asterix034euro.h \
    $$PWD/src/eurocontrol/asterix048euro.h \
    $$PWD/src/eurocontrol/asterix062euro.h \
    $$PWD/src/eurocontrol/asterix063euro.h \
    $$PWD/src/eurocontrol/asterix253euro.h \
# Aerotechnica standard
    $$PWD/src/aerotechnica/asterix001aero.h \
    $$PWD/src/aerotechnica/asterix002aero.h \
    $$PWD/src/aerotechnica/asterix062aero.h \
    $$PWD/src/aerotechnica/asterix250aero.h \
    $$PWD/src/aerotechnica/asterix253aero.h \
    $$PWD/src/aerotechnica/asterix254aero.h \
# Peleng standard
    $$PWD/src/peleng/asterix062peleng.h \
    $$PWD/src/peleng/asterix241peleng.h \
    $$PWD/src/peleng/asterix253peleng.h

include(../../factory/factory.pri)
include(../share/share.pri)
