get_filename_component(geogrengine_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories(
	"${geogrengine_CURRENTDIR}/src"
	"${geogrengine_CURRENTDIR}/src/back"
	"${geogrengine_CURRENTDIR}/src/front"
	)

list(APPEND SOURCES 
	"${geogrengine_CURRENTDIR}/src/back/common/Algorithms.cpp"
	"${geogrengine_CURRENTDIR}/src/back/formular/ProcessingThread.cpp"
	"${geogrengine_CURRENTDIR}/src/back/formular/SpiralSeparation.cpp"
	"${geogrengine_CURRENTDIR}/src/back/View.cpp"
	"${geogrengine_CURRENTDIR}/src/front/common/UserSettings.cpp"
	"${geogrengine_CURRENTDIR}/src/front/common/ZOrderValue.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/airport/SSA.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/airport/SSALegs.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIObject.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIAirport.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUICorridor.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIGrid.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIHolding.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIMeteoContour.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIMeteoVector.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIPeleng.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIPoint.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIRestriction.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIRoute.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIScene.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUISector.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUISSA.cpp"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIView.cpp"
	"${geogrengine_CURRENTDIR}/src/front/misc/XScaleRuler.cpp"
	)

list(APPEND Q_HEADERS 
	"${geogrengine_CURRENTDIR}/src/back/formular/ProcessingThread.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIView.h"
	)

list(APPEND HEADERS 
	"${geogrengine_CURRENTDIR}/src/back/common/Algorithms.h"
	"${geogrengine_CURRENTDIR}/src/back/formular/FormularDefs.h"
	"${geogrengine_CURRENTDIR}/src/back/formular/SeparateStrategy.h"
	"${geogrengine_CURRENTDIR}/src/back/formular/SpiralSeparation.h"
	"${geogrengine_CURRENTDIR}/src/back/core/MeteoContour.h"
	"${geogrengine_CURRENTDIR}/src/back/core/MeteoVector.h"
	"${geogrengine_CURRENTDIR}/src/back/core/Peleng.h"
	"${geogrengine_CURRENTDIR}/src/back/View.h"
	"${geogrengine_CURRENTDIR}/src/back/MacroDefs.h"
	"${geogrengine_CURRENTDIR}/src/front/common/UserSettings.h"
	"${geogrengine_CURRENTDIR}/src/front/common/ZOrderValue.h"
	"${geogrengine_CURRENTDIR}/src/front/core/airport/SSA.h"
	"${geogrengine_CURRENTDIR}/src/front/core/airport/SSALegs.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIAirport.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUICorridor.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIGrid.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIHolding.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIInterface.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIMeteoContour.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIMeteoVector.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIObject.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIPeleng.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIPoint.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIRestriction.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIRoute.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUIScene.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUISector.h"
	"${geogrengine_CURRENTDIR}/src/front/core/GUISSA.h"
	"${geogrengine_CURRENTDIR}/src/front/misc/XScaleRuler.h"
	)

list(APPEND RESOURCES
	"${geogrengine_CURRENTDIR}/share/GeoGEResources.qrc"
	)
