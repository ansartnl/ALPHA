get_filename_component(fplparser_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${fplparser_CURRENTDIR}")

list(APPEND SOURCES "${fplparser_CURRENTDIR}/FplParser.cpp")
    
list(APPEND HEADERS "${fplparser_CURRENTDIR}/FplParser.h")

message("Welcome to fplparser. I hope it is usefull")