INCLUDEPATH   += $$PWD/src
DEPENDPATH    += $$PWD/src

HEADERS       += $$PWD/src/Database.h \
				$$PWD/src/DBMySQL.h \
				$$PWD/src/DBOCI.h \
				$$PWD/src/DBPostgreSQL.h \
				$$PWD/src/DBQuery.h

SOURCES       += $$PWD/src/Database.cpp \
				$$PWD/src/DBMySQL.cpp \
				$$PWD/src/DBOCI.cpp \
				$$PWD/src/DBPostgreSQL.cpp \
				$$PWD/src/DBQuery.cpp
