INCLUDEPATH   += $$PWD/src
DEPENDPATH    += $$PWD/src

INCLUDEPATH   += $$PWD/src/eurocontrol
DEPENDPATH    += $$PWD/src/eurocontrol

HEADERS       += asterixdata.h \
                asterixexport.h \
                asterixerrors.h \
                asterix.h \
                asterix001.h \
                asterix002.h \
                asterix008.h \
                asterix020.h \
                asterix021.h \
                asterix034.h \
                asterix048.h \
                asterix062.h \
                asterix063.h \
# Eurocontrol standard
                eurocontrol/asterix001euro.h \
                eurocontrol/asterix002euro.h \
                eurocontrol/asterix008euro.h \
                eurocontrol/asterix020euro.h \
                eurocontrol/asterix021euro.h \
                eurocontrol/asterix034euro.h \
                eurocontrol/asterix048euro.h \
                eurocontrol/asterix062euro.h \
                eurocontrol/asterix063euro.h \
                eurocontrol/asterix253euro.h \
# Aerotechnica standard
                aerotechnica/asterix001aero.h \
                aerotechnica/asterix002aero.h \
                aerotechnica/asterix062aero.h \
                aerotechnica/asterix250aero.h \
                aerotechnica/asterix253aero.h \
                aerotechnica/asterix254aero.h \
# Peleng standard
                src/peleng/asterix062peleng.h \
                src/peleng/asterix241peleng.h \
                src/peleng/asterix253peleng.h
				

LIBS += -L$$PWD/bin
CONFIG(debug, debug|release) : LIBS += -lasterix_debug
CONFIG(release, release|debug) : LIBS += -lasterix

include($$PWD/../../factory/factory.pri)
