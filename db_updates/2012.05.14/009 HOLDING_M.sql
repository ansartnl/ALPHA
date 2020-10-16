CREATE TABLE HOLDING_M 
(
  ID NUMBER(10, 0) NOT NULL 
, POINT_ID NUMBER(10, 0) NOT NULL 
, TYPE NUMBER(10, 0) NOT NULL 
, IN_COURSE FLOAT(126) NOT NULL 
, TURN_DIRECTION NUMBER(10, 0) NOT NULL 
, LEG_LENGTH FLOAT(126) NOT NULL 
, LEG_TIME NUMBER(10, 0) NOT NULL 
, SPEED FLOAT(126) NOT NULL 
, ZONE_ID NUMBER(10, 0) NOT NULL 
, CONSTRAINT HOLDING_M_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);

ALTER TABLE HOLDING_M
ADD CONSTRAINT FK_HOLDING_POINT_M_ID FOREIGN KEY
(
  POINT_ID 
)
REFERENCES POINT
(
  ID 
)
ON DELETE CASCADE ENABLE;

ALTER TABLE HOLDING_M
ADD CONSTRAINT FK_HOLDING_TURN_M_ID FOREIGN KEY
(
  TURN_DIRECTION 
)
REFERENCES HOLDING_TURN
(
  ID 
)
ENABLE;

ALTER TABLE HOLDING_M
ADD CONSTRAINT FK_HOLDING_TYPE_M_ID FOREIGN KEY
(
  TYPE 
)
REFERENCES HOLDING_TYPE_M
(
  ID 
)
ENABLE;

ALTER TABLE HOLDING_M
ADD CONSTRAINT FK_HOLDING_ZONE_M_ID FOREIGN KEY
(
  ZONE_ID 
)
REFERENCES ZONE_M
(
  ID 
)
ON DELETE CASCADE ENABLE;

COMMENT ON TABLE HOLDING_M IS 'Describe the holding zone.';

COMMENT ON COLUMN HOLDING_M.ID IS 'Primary key';

COMMENT ON COLUMN HOLDING_M.POINT_ID IS 'The entry point of the holding zone';

COMMENT ON COLUMN HOLDING_M.TYPE IS 'Type of holding zone. For textual descriprion see HOLDING_TYPE table';

COMMENT ON COLUMN HOLDING_M.IN_COURSE IS 'Entering course';

COMMENT ON COLUMN HOLDING_M.TURN_DIRECTION IS 'Turn direction after entering holding zone';

COMMENT ON COLUMN HOLDING_M.LEG_LENGTH IS 'The length of the holding zone';

COMMENT ON COLUMN HOLDING_M.LEG_TIME IS 'The other variant leng assignment is LEG_TIME*SPEED';

COMMENT ON COLUMN HOLDING_M.SPEED IS 'The other variant leng assignment is LEG_TIME*SPEED';

COMMENT ON COLUMN HOLDING_M.ZONE_ID IS 'The zone id which holding is belonged';


  CREATE OR REPLACE TRIGGER "AERO"."HOLDING_M_AC2" 
  BEFORE INSERT OR UPDATE OR DELETE ON HOLDING_M FOR EACH ROW
DECLARE
  ac_flag NUMBER;
  rec_exist NUMBER;

BEGIN
  ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
  IF ac_flag = 777 THEN
    --DELETE
    IF DELETING THEN
      null;
    --INSERT or UPDATE
    ELSE
      SELECT COUNT(*) INTO rec_exist FROM HOLDING_TYPE_M WHERE id = :new.type;
      IF rec_exist = 0 THEN
        INSERT INTO HOLDING_TYPE_M (SELECT * FROM HOLDING_TYPE WHERE id = :new.type);
      END IF;
      SELECT COUNT(*) INTO rec_exist FROM ZONE_M WHERE id = :new.zone_id;
      IF rec_exist = 0 THEN
        INSERT INTO ZONE_M (SELECT * FROM ZONE WHERE id = :new.zone_id);
      END IF;
      SELECT COUNT(*) INTO rec_exist FROM POINT_M WHERE id = :new.point_id;
      IF rec_exist = 0 THEN
        INSERT INTO POINT_M (SELECT * FROM POINT WHERE id = :new.point_id);
      END IF;
      SELECT COUNT(*) INTO rec_exist FROM ZONE_POINT_M
      WHERE point_id = :new.point_id AND zone_id = :new.zone_id;
      IF rec_exist = 0 THEN
        INSERT INTO ZONE_POINT_M (zone_id, point_id) VALUES (:new.zone_id, :new.point_id);
      END IF; 
    END IF;
  END IF;
END;
/
ALTER TRIGGER "AERO"."HOLDING_M_AC2" ENABLE;
