get_filename_component(geo_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${geo_CURRENTDIR}")

list(APPEND SOURCES "${geo_CURRENTDIR}/GeoPoint.cpp"
  "${geo_CURRENTDIR}/Vincenty.cpp"
)

list(APPEND HEADERS "${geo_CURRENTDIR}/GeoPoint.h"
  "${geo_CURRENTDIR}/Vincenty.h")