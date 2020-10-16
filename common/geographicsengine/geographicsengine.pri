QT       += core gui

INCLUDEPATH   += $$PWD $$PWD/src $$PWD/src/back $$PWD/src/front
DEPENDPATH    += $$PWD $$PWD/src $$PWD/src/back $$PWD/src/front

HEADERS       += \
    $$PWD/src/back/common/Algorithms.h \
    $$PWD/src/back/formular/FormularDefs.h \
    $$PWD/src/back/formular/ProcessingThread.h \
    $$PWD/src/back/formular/SeparateStrategy.h \
    $$PWD/src/back/formular/SpiralSeparation.h \
    $$PWD/src/back/core/MeteoContour.h \
    $$PWD/src/back/core/MeteoVector.h \
    $$PWD/src/back/core/Peleng.h \
    $$PWD/src/back/View.h \
    $$PWD/src/back/MacroDefs.h \
    $$PWD/src/front/common/UserSettings.h \
    $$PWD/src/front/common/ZOrderValue.h \
    $$PWD/src/front/core/airport/SSA.h \
    $$PWD/src/front/core/airport/SSALegs.h \
    $$PWD/src/front/core/GUIInterface.h \
    $$PWD/src/front/core/GUIObject.h \
    $$PWD/src/front/core/GUIAirport.h \
    $$PWD/src/front/core/GUICorridor.h \
    $$PWD/src/front/core/GUIGrid.h \
    $$PWD/src/front/core/GUIHolding.h \
    $$PWD/src/front/core/GUIMeteoContour.h \
    $$PWD/src/front/core/GUIMeteoVector.h \
    $$PWD/src/front/core/GUIPeleng.h \
    $$PWD/src/front/core/GUIPoint.h \
    $$PWD/src/front/core/GUIRestriction.h \
    $$PWD/src/front/core/GUIRoute.h \
    $$PWD/src/front/core/GUIScene.h \
    $$PWD/src/front/core/GUISector.h \
    $$PWD/src/front/core/GUISSA.h \
    $$PWD/src/front/core/GUIView.h \
    $$PWD/src/front/misc/XScaleRuler.h

SOURCES       += \
    $$PWD/src/back/common/Algorithms.cpp \
    $$PWD/src/back/formular/ProcessingThread.cpp \
    $$PWD/src/back/formular/SpiralSeparation.cpp \
    $$PWD/src/back/View.cpp \
    $$PWD/src/front/common/UserSettings.cpp \
    $$PWD/src/front/common/ZOrderValue.cpp \
    $$PWD/src/front/core/airport/SSA.cpp \
    $$PWD/src/front/core/airport/SSALegs.cpp \
    $$PWD/src/front/core/GUIObject.cpp \
    $$PWD/src/front/core/GUIAirport.cpp \
    $$PWD/src/front/core/GUICorridor.cpp \
    $$PWD/src/front/core/GUIGrid.cpp \
    $$PWD/src/front/core/GUIHolding.cpp \
    $$PWD/src/front/core/GUIMeteoContour.cpp \
    $$PWD/src/front/core/GUIMeteoVector.cpp \
    $$PWD/src/front/core/GUIPeleng.cpp \
    $$PWD/src/front/core/GUIPoint.cpp \
    $$PWD/src/front/core/GUIRestriction.cpp \
    $$PWD/src/front/core/GUIRoute.cpp \
    $$PWD/src/front/core/GUIScene.cpp \
    $$PWD/src/front/core/GUISector.cpp \
    $$PWD/src/front/core/GUISSA.cpp \
    $$PWD/src/front/core/GUIView.cpp \
    $$PWD/src/front/misc/XScaleRuler.cpp

RESOURCES += $$PWD/share/GeoGEResources.qrc
