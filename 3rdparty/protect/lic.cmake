get_filename_component(LIC_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${LIC_CURRENTDIR}/lic")
    
list(APPEND HEADERS "${LIC_CURRENTDIR}/lic/lic.h")
list(APPEND SOURCES "${LIC_CURRENTDIR}/lic/lic.cpp")
