--------------------------------------------------------
--  File created - понедельник-Январь-16-2012   
--------------------------------------------------------

  DROP TABLE "RESTRICTION_USER_SYNC" cascade constraints;
  DROP TABLE "RESTRICTION_USER" cascade constraints;
  
--------------------------------------------------------
--  DDL for Table RESTRICTION_USER
--------------------------------------------------------
	
  CREATE TABLE "RESTRICTION_USER" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(30 BYTE), 
	"ZONE_ID" NUMBER(10,0), 
	"H_MIN" NUMBER(10,0), 
	"H_MAX" NUMBER(10,0), 
	"TIME_START" TIMESTAMP (6), 
	"TIME_END" TIMESTAMP (6), 
	"TYPE" NUMBER(2,0),
	"GUID" VARCHAR2(128)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "RESTRICTION_USER"."ID" IS 'Первичный и уникальный ключ совместно с таблицей RESTRICTION';
 
   COMMENT ON COLUMN "RESTRICTION_USER"."NAME" IS 'Имя зоны';
 
   COMMENT ON COLUMN "RESTRICTION_USER"."ZONE_ID" IS 'Идентификатор ФИР-а';
 
   COMMENT ON COLUMN "RESTRICTION_USER"."H_MIN" IS 'Нижняя граница зоны';
 
   COMMENT ON COLUMN "RESTRICTION_USER"."H_MAX" IS 'Верхняя граница зоны';
 
   COMMENT ON COLUMN "RESTRICTION_USER"."TIME_START" IS 'Начальное время появления зоны';
 
   COMMENT ON COLUMN "RESTRICTION_USER"."TIME_END" IS 'Время исчезновения зоны';
 
   COMMENT ON COLUMN "RESTRICTION_USER"."TYPE" IS 'Тип зоны ограничения из таблицы RESTRICTION_USER_TYPE';
   
   COMMENT ON COLUMN "RESTRICTION_USER"."GUID" IS 'Уникальный идентификатор';
--------------------------------------------------------
--  DDL for Table RESTRICTION_USER_SYNC
--------------------------------------------------------

  CREATE TABLE "RESTRICTION_USER_SYNC" 
   (	"ID" NUMBER(10,0), 
	"TYPE" NUMBER(1,0) DEFAULT 1, 
	"LASTTOUCH" TIMESTAMP (6) DEFAULT SYSTIMESTAMP
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "RESTRICTION_USER_SYNC"."ID" IS 'Identifier of Restriction user zone';
 
   COMMENT ON COLUMN "RESTRICTION_USER_SYNC"."TYPE" IS 'Type of an action: delete = 0, insert(or update) = 1';
 
   COMMENT ON COLUMN "RESTRICTION_USER_SYNC"."LASTTOUCH" IS 'Last touch (insert, update, delete data from restriction_user table) timestamp';
--------------------------------------------------------
--  DDL for Index RESTRICTION_USER_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RESTRICTION_USER_PK" ON "RESTRICTION_USER" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index RESTRICTION_USER_SYNC_INDEX1
--------------------------------------------------------

  CREATE UNIQUE INDEX "RESTRICTION_USER_SYNC_INDEX1" ON "RESTRICTION_USER_SYNC" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  Constraints for Table RESTRICTION_USER
--------------------------------------------------------

  ALTER TABLE "RESTRICTION_USER" ADD CONSTRAINT "RESTRICTION_USER_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "RESTRICTION_USER" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_USER" MODIFY ("ZONE_ID" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_USER" MODIFY ("TYPE" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RESTRICTION_USER_SYNC
--------------------------------------------------------

  ALTER TABLE "RESTRICTION_USER_SYNC" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_USER_SYNC" MODIFY ("TYPE" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_USER_SYNC" MODIFY ("LASTTOUCH" NOT NULL ENABLE);
--------------------------------------------------------
--  Ref Constraints for Table RESTRICTION_USER
--------------------------------------------------------

  ALTER TABLE "RESTRICTION_USER" ADD CONSTRAINT "RESTRICTION_USER_RESTRICT_FK1" FOREIGN KEY ("TYPE")
	  REFERENCES "RESTRICTION_USER_TYPE" ("ID") ENABLE;
 
  ALTER TABLE "RESTRICTION_USER" ADD CONSTRAINT "RESTRICTION_USER_ZONE_FK1" FOREIGN KEY ("ZONE_ID")
	  REFERENCES "ZONE" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  DDL for Trigger RESTRICTION_USER_CLEAN_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RESTRICTION_USER_CLEAN_TRG" AFTER INSERT OR UPDATE OR DELETE ON RESTRICTION_USER
FOR EACH ROW

BEGIN
  BEGIN
    delete from restriction_user_sync where lasttouch < systimestamp - interval '6' hour;
  EXCEPTION
  WHEN OTHERS THEN
    null;
  END;
END;
/
ALTER TRIGGER "RESTRICTION_USER_CLEAN_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RESTRICTION_USER_MODIFY_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RESTRICTION_USER_MODIFY_TRG" AFTER INSERT OR UPDATE ON RESTRICTION_USER
FOR EACH ROW

BEGIN
  BEGIN
    insert into restriction_user_sync (id, type) values(:new.id, 1);
  EXCEPTION
  WHEN DUP_VAL_ON_INDEX THEN
    update restriction_user_sync SET lasttouch = systimestamp where id = :old.id;
  WHEN OTHERS THEN
    null;
  END;
END;
/
ALTER TRIGGER "RESTRICTION_USER_MODIFY_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RESTRICTION_USER_DELETE_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RESTRICTION_USER_DELETE_TRG" BEFORE DELETE ON RESTRICTION_USER
FOR EACH ROW

BEGIN
  BEGIN
    insert into restriction_user_sync (id, type) values(:old.id, 0);
  EXCEPTION
  WHEN DUP_VAL_ON_INDEX THEN
    update restriction_user_sync SET type = 0, lasttouch = systimestamp where id = :old.id;
  WHEN OTHERS THEN
    null;
  END;
END;
/
ALTER TRIGGER "RESTRICTION_USER_DELETE_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RESTRICTION_USER_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RESTRICTION_USER_ID_TRG" BEFORE INSERT ON RESTRICTION_USER
FOR EACH ROW

BEGIN
  if :new.id is null then
    SELECT  restriction_id_SEQ.NEXTVAL INTO :new.id FROM DUAL;
  end if;
END;
/
ALTER TRIGGER "RESTRICTION_USER_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RESTRICTION_USER_LT
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RESTRICTION_USER_LT" 
    AFTER INSERT OR UPDATE OR DELETE ON RESTRICTION_USER
    FOR EACH ROW
    BEGIN
    -- Touch the row
      update sys_tables_info set lasttouch = SYS_EXTRACT_UTC(SYSTIMESTAMP) where tablename ='RESTRICTION_USER';
    END;
/
ALTER TRIGGER "RESTRICTION_USER_LT" ENABLE;
