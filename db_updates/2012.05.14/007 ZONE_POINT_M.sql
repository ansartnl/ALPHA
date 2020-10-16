CREATE TABLE ZONE_POINT_M 
(
  ID NUMBER(10, 0) NOT NULL 
, ZONE_ID NUMBER(10, 0) NOT NULL 
, POINT_ID NUMBER(10, 0) NOT NULL 
, CONSTRAINT ZONE_POINT_M_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);

ALTER TABLE ZONE_POINT_M
ADD CONSTRAINT FK_SECTORPOINTS_POINT_M_ID FOREIGN KEY
(
  POINT_ID 
)
REFERENCES POINT_M
(
  ID 
)
ON DELETE CASCADE ENABLE;

ALTER TABLE ZONE_POINT_M
ADD CONSTRAINT FK_SECTORPOINTS_ZONE_M_ID FOREIGN KEY
(
  ZONE_ID 
)
REFERENCES ZONE_M
(
  ID 
)
ON DELETE CASCADE ENABLE;

COMMENT ON TABLE ZONE_POINT_M IS 'Many-to-many lick describing belong POINT_M to ZONE_M. Do not foreget recalculate this table afret change in zones or points.';

COMMENT ON COLUMN ZONE_POINT_M.ID IS 'Primary key';

COMMENT ON COLUMN ZONE_POINT_M.ZONE_ID IS 'Foreign key to ZONE.ID';

COMMENT ON COLUMN ZONE_POINT_M.POINT_ID IS 'Foreign key to POINT.ID';

CREATE SEQUENCE ZONE_POINT_M_ID_SEQ INCREMENT BY 1 MAXVALUE 999999999999999999999999 MINVALUE 1 CACHE 20;

CREATE OR REPLACE
TRIGGER "AERO"."ZONE_POINT_M_ID_TRG" BEFORE INSERT ON ZONE_POINT_M
FOR EACH ROW
BEGIN
  IF :new.id IS null THEN
    SELECT  ZONE_POINT_M_ID_SEQ.NEXTVAL INTO :new.id FROM DUAL;
  end if;
END;
