/////////////////////////////////////////
ALTER TABLE VEHICLE_EXERCISE 
DROP CONSTRAINT VEHICLE_EXERCISE_VEHICLE__FK1;
drop table VEHICLE_PLAN;
/
ALTER TABLE VEHICLE_ROUTE_POINTS 
DROP CONSTRAINT VEHICLE_ROUTE_POINTS_VEHI_FK1;
drop table VEHICLE_ROUTE;
delete VEHICLE_ROUTE_POINTS;
/

CREATE TABLE VEHICLE_ROUTE 
(
  ID NUMBER NOT NULL 
, NAME VARCHAR2(100) NOT NULL 
, AIRPORT_ID NUMBER NOT NULL 
, CONSTRAINT VEHICLE_ROUTE_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);
/
ALTER TABLE VEHICLE_ROUTE_POINTS
ADD CONSTRAINT VEHICLE_ROUTE_POINTS_VEHI_FK1 FOREIGN KEY
(
  ROUTE_ID 
)
REFERENCES VEHICLE_ROUTE
(
  ID 
)
ON DELETE CASCADE ENABLE;
/
ALTER TABLE VEHICLE_ROUTE
ADD CONSTRAINT VEHICLE_ROUTE_AIRPORT_FK1 FOREIGN KEY
(
  AIRPORT_ID 
)
REFERENCES AIRPORT
(
  ID 
)
ON DELETE CASCADE ENABLE;
/ 

CREATE TABLE VEHICLE_PLAN 
(
  ID NUMBER NOT NULL 
, NAME VARCHAR2(100) NOT NULL 
, VEHICLE_ID NUMBER NOT NULL 
, ROUTE_ID NUMBER NOT NULL 
, SPEED NUMBER 
, CONSTRAINT VEHICLE_PLAN_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);
/
ALTER TABLE VEHICLE_PLAN
ADD CONSTRAINT VEHICLE_PLAN_VEHICLE_FK1 FOREIGN KEY
(
  VEHICLE_ID 
)
REFERENCES VEHICLE
(
  ID 
)
ON DELETE CASCADE ENABLE;
/
ALTER TABLE VEHICLE_PLAN
ADD CONSTRAINT VEHICLE_PLAN_VEHICLE_ROUT_FK1 FOREIGN KEY
(
  ROUTE_ID 
)
REFERENCES VEHICLE_ROUTE
(
  ID 
)
ON DELETE CASCADE ENABLE;
/
delete VEHICLE_EXERCISE;
/
 ALTER TABLE VEHICLE_EXERCISE
ADD CONSTRAINT VEHICLE_EXERCISE_VEHICLE_FK1 FOREIGN KEY
(
  VEHICLE_PLAN_ID 
)
REFERENCES VEHICLE_PLAN
(
  ID 
)
ON DELETE CASCADE ENABLE;
/
ALTER TABLE VEHICLE_PLAN 
ADD (STARTTIME NUMBER DEFAULT 0 NOT NULL);
/
create or replace
TRIGGER VEHICLE_PLAN_TRG BEFORE INSERT ON VEHICLE_PLAN 
FOR EACH ROW 
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    IF :NEW.ID IS NULL THEN
      SELECT VEHICLE_PLAN_SEQ.NEXTVAL INTO :NEW.ID FROM DUAL;
    END IF;
  END COLUMN_SEQUENCES;
END;
/
create or replace
TRIGGER VEHICLE_ROUTE_TRG BEFORE INSERT ON VEHICLE_ROUTE 
FOR EACH ROW 
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    IF :NEW.ID IS NULL THEN
      SELECT VEHICLE_ROUTE_SEQ.NEXTVAL INTO :NEW.ID FROM DUAL;
    END IF;
  END COLUMN_SEQUENCES;
END;
/
