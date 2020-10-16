get_filename_component(OLDI_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${OLDI_CURRENTDIR}")
    
list(APPEND HEADERS "${OLDI_CURRENTDIR}/OLDIMessages.h")
list(APPEND HEADERS "${OLDI_CURRENTDIR}/OLDIClients.h")

list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIProc.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMessage.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesABI.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesACT.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesCOD.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesCOF.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesERR.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesFPL.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesHOP.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesINF.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesLAM.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesMAC.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesMAS.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesROF.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesSTS.h")
list(APPEND Q_HEADERS "${OLDI_CURRENTDIR}/OLDIMesURZ.h")

list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIProc.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMessage.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesABI.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesACT.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesCOD.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesCOF.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesERR.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesFPL.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesHOP.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesINF.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesLAM.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesMAC.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesMAS.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesROF.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesSTS.cpp")
list(APPEND SOURCES "${OLDI_CURRENTDIR}/OLDIMesURZ.cpp")

message("Hello, it's me. My name is OLDI.")

include("${OLDI_CURRENTDIR}/../utils/utils.cmake")
include("${OLDI_CURRENTDIR}/../qtautotransaction/qtautotransaction.cmake")
include("${OLDI_CURRENTDIR}/../dbaccess/dbaccess.cmake")
