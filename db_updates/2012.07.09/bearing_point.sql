  DROP TABLE "BEARING_POINT" cascade constraints;
  
  CREATE TABLE "BEARING_POINT" 
   (	"ID" NUMBER NOT NULL ENABLE, 
	"NAME" VARCHAR2(256 BYTE), 
	"FIR_NAME" VARCHAR2(20 BYTE) NOT NULL ENABLE, 
	"RDF_LATITUDE" FLOAT(126) NOT NULL ENABLE, 
	"RDF_LONGITUDE" FLOAT(126) NOT NULL ENABLE, 
	"SAC" NUMBER, 
	"SIC" NUMBER, 
	"CHANNEL" NUMBER, 
	 CONSTRAINT "BEARING_POINT_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 
 
  CREATE SEQUENCE  "BEARING_POINT_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
 

  CREATE OR REPLACE TRIGGER "BEARING_POINT_TRG" BEFORE INSERT ON BEARING_POINT 
FOR EACH ROW 
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    SELECT BEARING_POINT_SEQ.NEXTVAL INTO :NEW.ID FROM DUAL;
  END COLUMN_SEQUENCES;
END;
/
ALTER TRIGGER "BEARING_POINT_TRG" ENABLE;
 
  CREATE OR REPLACE TRIGGER "BEARING_POINT_LT" 
    AFTER INSERT OR UPDATE OR DELETE ON BEARING_POINT
    FOR EACH ROW
    BEGIN
    -- Touch the row
      update sys_tables_info set lasttouch = SYS_EXTRACT_UTC(SYSTIMESTAMP) where tablename ='BEARING_POINT';
    END;
/
ALTER TRIGGER "BEARING_POINT_LT" ENABLE;

REM INSERTING into BEARING_POINT
Insert into BEARING_POINT (FIR_NAME,RDF_LATITUDE,RDF_LONGITUDE,SAC,SIC) values ('UDDD',30,50,1,1);
Insert into BEARING_POINT (FIR_NAME,RDF_LATITUDE,RDF_LONGITUDE,SAC,SIC) values ('UMMV',53,28,2,2);
Insert into BEARING_POINT (FIR_NAME,RDF_LATITUDE,RDF_LONGITUDE,SAC,SIC) values ('UTTR',41.26,69.2916666666667,3,3);
commit;
