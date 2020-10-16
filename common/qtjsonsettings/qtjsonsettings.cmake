get_filename_component(qtjsonsettings_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${qtjsonsettings_CURRENTDIR}")

# "${qtjsonsettings_CURRENTDIR}/jsonparser.cpp" included in json.cpp

list(APPEND SOURCES "${qtjsonsettings_CURRENTDIR}/qtjsonsettings.cpp"
    "${qtjsonsettings_CURRENTDIR}/json.cpp")
    
list(APPEND Q_HEADERS "${qtjsonsettings_CURRENTDIR}/qtjsonsettings.h")

list(APPEND HEADERS "${qtjsonsettings_CURRENTDIR}/json.h")

message("Hello, it's me. My name is qtjsonsettings. I can store your application settings in JSON file")