# -----------------------
# Macroses
# -----------------------

MACRO (TODAY RESULT)
    IF (WIN32)
        EXECUTE_PROCESS(COMMAND "cmd" "/C" "date" "/T" OUTPUT_VARIABLE ${RESULT} OUTPUT_STRIP_TRAILING_WHITESPACE)
        # string(STRIP ${${RESULT}} ${RESULT})
        # string(REGEX REPLACE "(..).(..).(....).*" "\\3\\2\\1" ${RESULT} ${${RESULT}})
    ELSEIF(UNIX)
        EXECUTE_PROCESS(COMMAND "date" "+%d.%m.%Y" OUTPUT_VARIABLE ${RESULT} OUTPUT_STRIP_TRAILING_WHITESPACE)
        # string(REGEX REPLACE "(..).(..).(....).*" "\\3\\2\\1" ${RESULT} ${${RESULT}})
    ELSE (WIN32)
        MESSAGE(SEND_ERROR "date not implemented")
        SET(${RESULT} 000000)
    ENDIF (WIN32)
ENDMACRO (TODAY)

TODAY(DATE_VERSION)

write_file("version.h" "#define DATE_VERSION " \"${DATE_VERSION}\")