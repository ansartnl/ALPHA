get_filename_component(dbaccess_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${dbaccess_CURRENTDIR}")

set(QT_USE_QTSQL TRUE)

list(APPEND SOURCES
	"${dbaccess_CURRENTDIR}/src/Database.cpp"
	"${dbaccess_CURRENTDIR}/src/DBMySQL.cpp"
	"${dbaccess_CURRENTDIR}/src/DBOCI.cpp"
	"${dbaccess_CURRENTDIR}/src/DBPostgreSQL.cpp"
	"${dbaccess_CURRENTDIR}/src/DBQuery.cpp"
	)
	
list(APPEND HEADERS 
	"${dbaccess_CURRENTDIR}/src/Database.h"
	"${dbaccess_CURRENTDIR}/src/DBMySQL.h"
	"${dbaccess_CURRENTDIR}/src/DBOCI.h"
	"${dbaccess_CURRENTDIR}/src/DBPostgreSQL.h"
	"${dbaccess_CURRENTDIR}/src/DBQuery.h"
	)
