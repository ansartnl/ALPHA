--------------------------------------------------------
--  File created - �������-��������-15-2011   
--------------------------------------------------------
  DROP TABLE  "RESTRICTION_USER" cascade constraints;
  DROP TABLE  "RESTRICTION_POINT_USER" cascade constraints;
--------------------------------------------------------
--  DDL for Table RESTRICTION_USER
--------------------------------------------------------

  CREATE TABLE  "RESTRICTION_USER" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(30 BYTE), 
	"ZONE_ID" NUMBER(10,0), 
	"H_MIN" NUMBER(10,0), 
	"H_MAX" NUMBER(10,0), 
	"TIME_START" TIMESTAMP (6), 
	"TIME_END" TIMESTAMP (6)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN  "RESTRICTION_USER"."ID" IS '��������� � ���������� ���� ��������� � �������� RESTRICTION';
 
   COMMENT ON COLUMN  "RESTRICTION_USER"."NAME" IS '��� ����';
 
   COMMENT ON COLUMN  "RESTRICTION_USER"."ZONE_ID" IS '������������� ���-�';
 
   COMMENT ON COLUMN  "RESTRICTION_USER"."H_MIN" IS '������ ������� ����';
 
   COMMENT ON COLUMN  "RESTRICTION_USER"."H_MAX" IS '������� ������� ����';
 
   COMMENT ON COLUMN  "RESTRICTION_USER"."TIME_START" IS '��������� ����� ��������� ����';
 
   COMMENT ON COLUMN  "RESTRICTION_USER"."TIME_END" IS '����� ������������ ����';
--------------------------------------------------------
--  DDL for Table RESTRICTION_POINT_USER
--------------------------------------------------------

  CREATE TABLE  "RESTRICTION_POINT_USER" 
   (	"ID" NUMBER(10,0), 
	"SN" NUMBER(10,0), 
	"TYPE" NUMBER(10,0), 
	"LATITUDE" FLOAT(126), 
	"LONGITUDE" FLOAT(126), 
	"ARC_LATITUDE" FLOAT(126), 
	"ARC_LONGITUDE" FLOAT(126), 
	"ARC_DISTANCE" FLOAT(126), 
	"RESTRICTION_ID" NUMBER(10,0)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index RESTRICTION_USER_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX  "RESTRICTION_USER_PK" ON  "RESTRICTION_USER" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index RESTRICTION_POINT_USER_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX  "RESTRICTION_POINT_USER_PK" ON  "RESTRICTION_POINT_USER" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index RESTRICTION_POINT_USER_INDEX1
--------------------------------------------------------

  CREATE INDEX  "RESTRICTION_POINT_USER_INDEX1" ON  "RESTRICTION_POINT_USER" ("RESTRICTION_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  Constraints for Table RESTRICTION_USER
--------------------------------------------------------

  ALTER TABLE  "RESTRICTION_USER" ADD CONSTRAINT "RESTRICTION_USER_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE  "RESTRICTION_USER" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE  "RESTRICTION_USER" MODIFY ("ZONE_ID" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RESTRICTION_POINT_USER
--------------------------------------------------------

  ALTER TABLE  "RESTRICTION_POINT_USER" ADD CONSTRAINT "RESTRICTION_POINT_USER_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE  "RESTRICTION_POINT_USER" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE  "RESTRICTION_POINT_USER" MODIFY ("SN" NOT NULL ENABLE);
 
  ALTER TABLE  "RESTRICTION_POINT_USER" MODIFY ("TYPE" NOT NULL ENABLE);
 
  ALTER TABLE  "RESTRICTION_POINT_USER" MODIFY ("LATITUDE" NOT NULL ENABLE);
 
  ALTER TABLE  "RESTRICTION_POINT_USER" MODIFY ("LONGITUDE" NOT NULL ENABLE);
 
  ALTER TABLE  "RESTRICTION_POINT_USER" MODIFY ("ARC_LATITUDE" NOT NULL ENABLE);
 
  ALTER TABLE  "RESTRICTION_POINT_USER" MODIFY ("ARC_LONGITUDE" NOT NULL ENABLE);
 
  ALTER TABLE  "RESTRICTION_POINT_USER" MODIFY ("ARC_DISTANCE" NOT NULL ENABLE);
 
  ALTER TABLE  "RESTRICTION_POINT_USER" MODIFY ("RESTRICTION_ID" NOT NULL ENABLE);
--------------------------------------------------------
--  Ref Constraints for Table RESTRICTION_USER
--------------------------------------------------------

  ALTER TABLE  "RESTRICTION_USER" ADD CONSTRAINT "RESTRICTION_USER_ZONE_FK1" FOREIGN KEY ("ZONE_ID")
	  REFERENCES  "ZONE" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table RESTRICTION_POINT_USER
--------------------------------------------------------

  ALTER TABLE  "RESTRICTION_POINT_USER" ADD CONSTRAINT "RESTRICTION_POINT_USER_RE_FK1" FOREIGN KEY ("TYPE")
	  REFERENCES  "RESTRICTION_POINT_TYPE" ("ID") ON DELETE CASCADE ENABLE;
 
  ALTER TABLE  "RESTRICTION_POINT_USER" ADD CONSTRAINT "RESTRICTION_POINT_USER_RE_FK2" FOREIGN KEY ("RESTRICTION_ID")
	  REFERENCES  "RESTRICTION_USER" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  DDL for Trigger RESTRICTION_USER_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER  "RESTRICTION_USER_ID_TRG" BEFORE INSERT ON RESTRICTION_USER
FOR EACH ROW

BEGIN
  if :new.id is null then
    SELECT  restriction_id_SEQ.NEXTVAL INTO :new.id FROM DUAL;
  end if;
END;
/
ALTER TRIGGER  "RESTRICTION_USER_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RESTRICTION_POINT_USER_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER  "RESTRICTION_POINT_USER_ID_TRG" BEFORE INSERT ON restriction_point_user
FOR EACH ROW
BEGIN
  if :new.id is null then
    SELECT  restriction_point_id_SEQ.NEXTVAL INTO :new.id FROM DUAL;
  end if;
END;
/
ALTER TRIGGER  "RESTRICTION_POINT_USER_ID_TRG" ENABLE;