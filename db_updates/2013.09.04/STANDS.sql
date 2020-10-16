
  ALTER TABLE EXERCISE_PLANE 
  DROP COLUMN STAND;

  CREATE TABLE "STAND" 
   (	"ID" NUMBER NOT NULL ENABLE, 
	"AIRPORT_ID" NUMBER NOT NULL ENABLE, 
	"NAME" VARCHAR2(200 CHAR) NOT NULL ENABLE, 
	 CONSTRAINT "STAND_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE, 
	 CONSTRAINT "STAND_AIRPORT_FK1" FOREIGN KEY ("AIRPORT_ID")
	  REFERENCES "AIRPORT" ("ID") ON DELETE CASCADE ENABLE
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   CREATE SEQUENCE  "STAND_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
 

  CREATE OR REPLACE TRIGGER "STAND_TRG" BEFORE INSERT ON STAND 
FOR EACH ROW 
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    IF :NEW.ID IS NULL THEN
      SELECT STAND_SEQ.NEXTVAL INTO :NEW.ID FROM DUAL;
    END IF;
  END COLUMN_SEQUENCES;
END;
/
ALTER TRIGGER "STAND_TRG" ENABLE;
 


  CREATE TABLE "STAND_STATE" 
   (	"ID" NUMBER NOT NULL ENABLE, 
	"NAME" VARCHAR2(20 BYTE) NOT NULL ENABLE, 
	 CONSTRAINT "STAND_STATE_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 
REM INSERTING into STAND_STATE
Delete from STAND_STATE;
Insert into STAND_STATE (ID,NAME) values (0,'Free');
Insert into STAND_STATE (ID,NAME) values (1,'Occupied');
Insert into STAND_STATE (ID,NAME) values (2,'Closed');
Commit;



  CREATE TABLE "EXERCISE_STAND" 
   (	"ID" NUMBER NOT NULL ENABLE, 
	"STAND_ID" NUMBER NOT NULL ENABLE, 
	"EXERCISE_ID" NUMBER NOT NULL ENABLE, 
	"FPL_ID" NUMBER, 
	"STATE_ID" NUMBER DEFAULT 0, 
	 CONSTRAINT "EXERCISE_STAND_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE, 
	 CONSTRAINT "EXERCISE_STAND_EXERCISE_FK1" FOREIGN KEY ("EXERCISE_ID")
	  REFERENCES "EXERCISE" ("ID_EXERCISE") ON DELETE CASCADE ENABLE, 
	 CONSTRAINT "EXERCISE_STAND_FPL_FK1" FOREIGN KEY ("FPL_ID")
	  REFERENCES "FPL" ("ID") ON DELETE SET NULL ENABLE, 
	 CONSTRAINT "EXERCISE_STAND_STAND_FK1" FOREIGN KEY ("STAND_ID")
	  REFERENCES "STAND" ("ID") ON DELETE CASCADE ENABLE, 
	 CONSTRAINT "EXERCISE_STAND_STAND_STAT_FK1" FOREIGN KEY ("STATE_ID")
	  REFERENCES "STAND_STATE" ("ID") ON DELETE SET NULL ENABLE
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   CREATE SEQUENCE  "EXERCISE_STAND_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
 

  CREATE OR REPLACE TRIGGER "EXERCISE_STAND_TRG" BEFORE INSERT ON EXERCISE_STAND 
FOR EACH ROW 
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    IF :NEW.ID IS NULL THEN
      SELECT EXERCISE_STAND_SEQ.NEXTVAL INTO :NEW.ID FROM DUAL;
    END IF;
  END COLUMN_SEQUENCES;
END;
/
ALTER TRIGGER "EXERCISE_STAND_TRG" ENABLE;


  CREATE OR REPLACE TRIGGER "STAND_EXERCISE__TRG" AFTER INSERT ON STAND 
FOR EACH ROW 
DECLARE
  CURSOR stand_cur
  IS
	SELECT id_exercise FROM exercise;
BEGIN
  FOR stand_rec IN stand_cur
  LOOP
    INSERT INTO exercise_stand(stand_id, exercise_id)
	  VALUES(:new.id, stand_rec.id_exercise);
  END LOOP;
END;
/
ALTER TRIGGER "STAND_EXERCISE__TRG" ENABLE;

  CREATE OR REPLACE TRIGGER "EXERCISE_STAND__TRG" AFTER INSERT ON EXERCISE 
FOR EACH ROW 
DECLARE
  CURSOR stand_cur
  IS
	SELECT id FROM stand;
BEGIN
  FOR stand_rec IN stand_cur
  LOOP
    INSERT INTO exercise_stand(stand_id, exercise_id)
	  VALUES(stand_rec.id, :new.id_exercise);
  END LOOP;
END;
/
ALTER TRIGGER "EXERCISE_STAND__TRG" ENABLE;


  CREATE OR REPLACE FORCE VIEW "VW_FPL_WITH_NULL" ("ID", "AIRCRAFT_ID") AS 
   select id, aircraft_id 
   from fpl
UNION
   select null id, '(empty)' aircraft_id 
   from dual;

/*
  CREATE OR REPLACE FORCE VIEW "VW_STAND" ("ID", "STAND_ID", "STAND_NAME", "AIRPORT_ID", "AIRPORT_NAME", "EXERCISE_ID", "EXERCISE_NAME", "FPL_ID", "FPL_NAME", "STATE_ID", "STATE_NAME") AS 
  SELECT	se.id id, 
	se.stand_id, s.name stand_name, 
	s.airport_id airport_id, a.name airport_name, 
	se.exercise_id exercise_id, e.name exercise_name, 
	se.fpl_id fpl_id, f.aircraft_id fpl_name, 
	se.state_id state_id, ss.name state_name 

FROM	exercise_stand se 
	RIGHT OUTER JOIN stand s ON se.stand_id = s.id 
	LEFT  OUTER JOIN airport a ON s.airport_id = a.id 
	LEFT  OUTER JOIN exercise e ON se.exercise_id = e.id_exercise 
	LEFT  OUTER JOIN fpl f ON se.fpl_id = f.id 
	LEFT  OUTER JOIN stand_state ss ON se.state_id = ss.id;
*/
 
