
  CREATE TABLE FPL_IN_EXERCISE 
   (	"ID" NUMBER NOT NULL ENABLE, 
	"FIR_NAME" VARCHAR2(4 CHAR) DEFAULT 'UMMV' NOT NULL ENABLE, 
	"AIRCRAFT_ID" VARCHAR2(7 CHAR) NOT NULL ENABLE, 
	"TAIRCRAFT" VARCHAR2(4 CHAR), 
	"FRULES" VARCHAR2(1 CHAR), 
	"TFLIGHT" VARCHAR2(1 CHAR), 
	"NUM" NUMBER(2,0), 
	"WTC" VARCHAR2(1 CHAR), 
	"EQUIPMENT" VARCHAR2(222 CHAR), 
	"ADEP" VARCHAR2(4 CHAR) NOT NULL ENABLE, 
	"TIME" TIMESTAMP (6), 
	"SPEED" VARCHAR2(5 CHAR) NOT NULL ENABLE, 
	"FLEVEL" VARCHAR2(5 CHAR) NOT NULL ENABLE, 
	"ROUTE" VARCHAR2(4000 CHAR), 
	"ADES" VARCHAR2(4 CHAR) NOT NULL ENABLE, 
	"EET" VARCHAR2(5 CHAR), 
	"ALT1" VARCHAR2(4 CHAR), 
	"ALT2" VARCHAR2(4 CHAR), 
	"OTHER" VARCHAR2(4000 CHAR), 
	"TRTIME" TIMESTAMP (6) DEFAULT SYS_EXTRACT_UTC(SYSTIMESTAMP) NOT NULL ENABLE, 
	"EXFPL" CHAR(1 CHAR), 
	"VISIBILITY" NUMBER(1,0), 
	"MESSAGE_ID" NUMBER, 
	"ENTRY_FIR_EET" VARCHAR2(5 CHAR)
   );
