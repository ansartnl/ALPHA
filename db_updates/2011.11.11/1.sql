DROP TABLE RESTRICTION_USER_TYPE CASCADE CONSTRAINTS;
DROP TABLE RESTRICTION_USER CASCADE CONSTRAINTS;

CREATE TABLE "RESTRICTION_USER_TYPE"
  (
    "ID"   NUMBER(2,0) NOT NULL ENABLE,
    "NAME" VARCHAR2(15 BYTE) NOT NULL ENABLE,
    CONSTRAINT "RESTRICTION_USER_TYPE_PK" PRIMARY KEY ("ID") USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645 PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT) TABLESPACE "USERS" ENABLE
  )
  PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING STORAGE
  (
    INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645 PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT
  )
  TABLESPACE "USERS" ;

REM INSERTING into RESTRICTION_USER_TYPE
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (79,'Obstacle');
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (82,'Restriction');
commit;
  
CREATE TABLE "RESTRICTION_USER"
  (
    "ID"      NUMBER(10,0) NOT NULL ENABLE,
    "NAME"    VARCHAR2(30 BYTE),
    "ZONE_ID" NUMBER(10,0) NOT NULL ENABLE,
    "H_MIN"   NUMBER(10,0),
    "H_MAX"   NUMBER(10,0),
    "TIME_START" TIMESTAMP (6),
    "TIME_END" TIMESTAMP (6),
    "TYPE" NUMBER(2,0) NOT NULL ENABLE,
    CONSTRAINT "RESTRICTION_USER_PK" PRIMARY KEY ("ID") USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645 PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT) TABLESPACE "USERS" ENABLE,
    CONSTRAINT "RESTRICTION_USER_ZONE_FK1" FOREIGN KEY ("ZONE_ID") REFERENCES "ZONE" ("ID") ON
  DELETE CASCADE ENABLE,
    CONSTRAINT "RESTRICTION_USER_RESTRICT_FK1" FOREIGN KEY ("TYPE") REFERENCES "RESTRICTION_USER_TYPE" ("ID") ENABLE
  )
  PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING STORAGE
  (
    INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645 PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT
  )
  TABLESPACE "USERS" ;
COMMENT ON COLUMN "RESTRICTION_USER"."ID"
IS
  '��������� � ���������� ���� ��������� � �������� RESTRICTION';
  COMMENT ON COLUMN "RESTRICTION_USER"."NAME"
IS
  '��� ����';
  COMMENT ON COLUMN "RESTRICTION_USER"."ZONE_ID"
IS
  '������������� ���-�';
  COMMENT ON COLUMN "RESTRICTION_USER"."H_MIN"
IS
  '������ ������� ����';
  COMMENT ON COLUMN "RESTRICTION_USER"."H_MAX"
IS
  '������� ������� ����';
  COMMENT ON COLUMN "RESTRICTION_USER"."TIME_START"
IS
  '��������� ����� ��������� ����';
  COMMENT ON COLUMN "RESTRICTION_USER"."TIME_END"
IS
  '����� ������������ ����';
  COMMENT ON COLUMN "RESTRICTION_USER"."TYPE"
IS
  '��� ���� ����������� �� ������� RESTRICTION_USER_TYPE';
CREATE OR REPLACE TRIGGER "RESTRICTION_USER_MODIFY_TRG" AFTER
  INSERT OR
  UPDATE ON RESTRICTION_USER FOR EACH ROW BEGIN DECLARE temp NUMBER;
  /* Dummy temporary variable to do nothing in exception statement*/
BEGIN
  INSERT INTO restriction_user_sync
    (id, type
    ) VALUES
    (:new.id, 1
    );
EXCEPTION
WHEN DUP_VAL_ON_INDEX THEN
  temp := 1;
WHEN OTHERS THEN
  temp := 1;
END;
END;
/
ALTER TRIGGER "RESTRICTION_USER_MODIFY_TRG" ENABLE;
CREATE OR REPLACE TRIGGER "RESTRICTION_USER_DELETE_TRG" BEFORE
  DELETE ON RESTRICTION_USER FOR EACH ROW BEGIN DECLARE temp NUMBER;
  id_current NUMBER;
  BEGIN
    INSERT INTO restriction_user_sync
      (id, type
      ) VALUES
      (:old.id, 0
      );
  EXCEPTION
  WHEN DUP_VAL_ON_INDEX THEN
    UPDATE restriction_user_sync SET type = 0 WHERE id = :old.id;
  WHEN OTHERS THEN
    temp := 1;
  END;
END;
/
ALTER TRIGGER "RESTRICTION_USER_DELETE_TRG" ENABLE;
CREATE OR REPLACE TRIGGER "RESTRICTION_USER_ID_TRG" BEFORE
  INSERT ON RESTRICTION_USER FOR EACH ROW BEGIN IF :new.id IS NULL THEN
  SELECT restriction_id_SEQ.NEXTVAL INTO :new.id FROM DUAL;
END IF;
END;
/
ALTER TRIGGER "RESTRICTION_USER_ID_TRG" ENABLE;
CREATE OR REPLACE TRIGGER "RESTRICTION_USER_LT" AFTER
  INSERT OR
  UPDATE OR
  DELETE ON RESTRICTION_USER FOR EACH ROW BEGIN
  -- Touch the row
  UPDATE sys_tables_info
  SET lasttouch   = SYS_EXTRACT_UTC(SYSTIMESTAMP)
  WHERE tablename ='RESTRICTION_USER';
END;
/
ALTER TRIGGER "RESTRICTION_USER_LT" ENABLE;