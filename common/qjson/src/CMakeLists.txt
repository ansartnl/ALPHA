# add_custom_command (OUTPUT ${qjson_SOURCE_DIR}/lib/json_parser.cc
#                    PRE_BUILD
#                    COMMAND bison -t -o json_parser.cc -d json_parser.yy
#                    DEPENDS json_parser.yy
#                    WORKING_DIRECTORY ${qjson_SOURCE_DIR}/lib/
#                    )

set (qjson_SRCS parser.cpp qobjecthelper.cpp json_scanner.cpp json_parser.cc parserrunnable.cpp serializer.cpp serializerrunnable.cpp)
set (qjson_HEADERS parser.h parserrunnable.h qobjecthelper.h serializer.h serializerrunnable.h qjson_export.h parserrunnable.h serializerrunnable.h Parser QObjectHelper Serializer)

add_library (qjson SHARED ${qjson_SRCS} ${qjson_HEADERS})

set_property(TARGET qjson PROPERTY FOLDER "Common")

if(COMMAND configure_build)
	configure_build(qjson)
else()
	set_target_properties(qjson PROPERTIES
		DEBUG_POSTFIX "d"
	)
endif()

link_qt(qjson)

if(NOT android)
	set_target_properties(qjson PROPERTIES
	                      VERSION ${QJSON_LIB_MAJOR_VERSION}.${QJSON_LIB_MINOR_VERSION}.${QJSON_LIB_PATCH_VERSION}
                              SOVERSION ${QJSON_LIB_MAJOR_VERSION}
	                      )
endif()
set_target_properties(qjson PROPERTIES
                      DEFINE_SYMBOL QJSON_MAKEDLL
                      PUBLIC_HEADER "${qjson_HEADERS}"
                      FRAMEWORK ${OSX_FRAMEWORK}
                      )
					  
if(NOT QJSON_NOT_SET_OWN_DIRECTORY)
if(WIN32)
	set_target_properties(qjson PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIB_INSTALL_DIR}
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIB_INSTALL_DIR}
		LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_INSTALL_DIR}
		LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_INSTALL_DIR}
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_INSTALL_DIR}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_INSTALL_DIR}
	)
else(WIN32)
	set_target_properties(qjson PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${LIB_INSTALL_DIR}
		ARCHIVE_OUTPUT_DIRECTORY ${LIB_INSTALL_DIR}
		LIBRARY_OUTPUT_DIRECTORY ${LIB_INSTALL_DIR}
		LIBRARY_OUTPUT_DIRECTORY ${LIB_INSTALL_DIR}
		RUNTIME_OUTPUT_DIRECTORY ${BIN_INSTALL_DIR}
		RUNTIME_OUTPUT_DIRECTORY ${BIN_INSTALL_DIR}
	)
endif(WIN32)
endif()

if (MSVC)
#    set(BIN_INSTALLATION_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "path to install binaries" FORCE)    
	
	set_target_properties(${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_INSTALLATION_PATH}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_INSTALLATION_PATH}
		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_INSTALLATION_PATH}
	)
endif()

if(QJSON_SUBPROJECT)
	copy_files(qjson ${INCLUDE_INSTALLATION_PATH}/qjson ${CMAKE_CURRENT_SOURCE_DIR}/ ${qjson_HEADERS})
endif()
