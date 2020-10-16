CREATE TABLE VEHICLE 
(
  ID NUMBER NOT NULL 
, NAME VARCHAR2(100) NOT NULL 
, CONSTRAINT VEHICLE_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);


   CREATE SEQUENCE  "VEHICLE_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
 
CREATE TABLE VEHICLE_ROUTE 
(
  ID NUMBER NOT NULL 
, NAME VARCHAR2(100) NOT NULL 
, CONSTRAINT VEHICLE_ROUTE_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);


   CREATE SEQUENCE  "VEHICLE_ROUTE_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
 

CREATE TABLE VEHICLE_ROUTE_POINTS 
(
  ID NUMBER NOT NULL 
, LATITUDE FLOAT NOT NULL 
, LONGITUDE FLOAT NOT NULL 
, ROUTE_ID NUMBER NOT NULL 
, CONSTRAINT VEHICLE_ROUTE_POINTS_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);

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


   CREATE SEQUENCE  "VEHICLE_ROUTE_POINTS_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
 

CREATE TABLE VEHICLE_PLAN 
(
  ID NUMBER NOT NULL 
, NAME VARCHAR2(100) NOT NULL 
, VEHICLE_ID NUMBER NOT NULL 
, ROUTE_ID NUMBER NOT NULL 
, AIRPORT_ID NUMBER NOT NULL 
, SPEED NUMBER 
, CONSTRAINT VEHICLE_PLAN_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);

ALTER TABLE VEHICLE_PLAN
ADD CONSTRAINT VEHICLE_PLAN_AIRPORT_FK1 FOREIGN KEY
(
  AIRPORT_ID 
)
REFERENCES AIRPORT
(
  ID 
)
ON DELETE CASCADE ENABLE;

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


   CREATE SEQUENCE  "VEHICLE_PLAN_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
 
CREATE TABLE VEHICLE_EXERCISE 
(
  ID NUMBER NOT NULL 
, EXERCISE_ID NUMBER NOT NULL 
, VEHICLE_PLAN_ID NUMBER NOT NULL 
, CONSTRAINT VEHICLE_EXERCISE_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);

ALTER TABLE VEHICLE_EXERCISE
ADD CONSTRAINT VEHICLE_EXERCISE_EXERCISE_FK1 FOREIGN KEY
(
  EXERCISE_ID 
)
REFERENCES EXERCISE
(
  ID_EXERCISE 
)
ON DELETE CASCADE ENABLE;

ALTER TABLE VEHICLE_EXERCISE
ADD CONSTRAINT VEHICLE_EXERCISE_VEHICLE__FK1 FOREIGN KEY
(
  VEHICLE_PLAN_ID 
)
REFERENCES VEHICLE_PLAN
(
  ID 
)
ON DELETE CASCADE ENABLE;


   CREATE SEQUENCE  "VEHICLE_EXERCISE_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
 

