--------------------------------------------------------
--  File created - Tuesday-February-08-2011   
--------------------------------------------------------
  DROP TABLE "AIRPORT" cascade constraints;
  DROP TABLE "BOUNDARY_POINT_TYPE" cascade constraints;
  DROP TABLE "HOLDING" cascade constraints;
  DROP TABLE "HOLDING_POINT_MASTER" cascade constraints;
  DROP TABLE "HOLDING_TURN" cascade constraints;
  DROP TABLE "HOLDING_TYPE" cascade constraints;
  DROP TABLE "POINT" cascade constraints;
  DROP TABLE "POINT_TYPE" cascade constraints;
  DROP TABLE "RESTRICTION" cascade constraints;
  DROP TABLE "RESTRICTION_POINT" cascade constraints;
  DROP TABLE "RESTRICTION_POINT_MASTER" cascade constraints;
  DROP TABLE "RESTRICTION_POINT_TYPE" cascade constraints;
  DROP TABLE "RESTRICTION_TYPE" cascade constraints;
  DROP TABLE "ROUTE" cascade constraints;
  DROP TABLE "ROUTE_POINT" cascade constraints;
  DROP TABLE "RUNWAY" cascade constraints;
  DROP TABLE "SSA" cascade constraints;
  DROP TABLE "SSA_POINT" cascade constraints;
  DROP TABLE "SSA_POINT_TURN" cascade constraints;
  DROP TABLE "SSA_POINT_TYPE" cascade constraints;
  DROP TABLE "SSA_TYPE" cascade constraints;
  DROP TABLE "ZONE" cascade constraints;
  DROP TABLE "ZONE_AIRPORT" cascade constraints;
  DROP TABLE "ZONE_BOUNDARY" cascade constraints;
  DROP TABLE "ZONE_POINT" cascade constraints;
  DROP TABLE "ZONE_TYPE" cascade constraints;
  DROP SEQUENCE "AIRPORT_ID_SEQ";
  DROP SEQUENCE "HOLDING_ID_SEQ";
  DROP SEQUENCE "HOLDING_POINT_MASTER_SEQ";
  DROP SEQUENCE "POINT_ID_SEQ";
  DROP SEQUENCE "RESTRICTION_ID_SEQ";
  DROP SEQUENCE "RESTRICTION_POINT_ID_SEQ";
  DROP SEQUENCE "RESTRICTION_POINT_MASTER_SEQ";
  DROP SEQUENCE "ROUTE_ID_SEQ";
  DROP SEQUENCE "ROUTE_POINT_ID_SEQ";
  DROP SEQUENCE "RUNWAY_ID_SEQ";
  DROP SEQUENCE "SSA_ID_SEQ";
  DROP SEQUENCE "SSA_POINT_ID_SEQ";
  DROP SEQUENCE "ZONE_AIRPORT_ID_SEQ";
  DROP SEQUENCE "ZONE_BOUNDARY_SEQ";
  DROP SEQUENCE "ZONE_ID_SEQ";
  DROP SEQUENCE "ZONE_POINT_ID_SEQ";
  DROP VIEW "POINT_VIEW";
--------------------------------------------------------
--  DDL for Sequence AIRPORT_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "AIRPORT_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence HOLDING_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "HOLDING_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence HOLDING_POINT_MASTER_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "HOLDING_POINT_MASTER_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence POINT_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "POINT_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence RESTRICTION_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "RESTRICTION_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence RESTRICTION_POINT_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "RESTRICTION_POINT_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence RESTRICTION_POINT_MASTER_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "RESTRICTION_POINT_MASTER_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence ROUTE_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "ROUTE_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence ROUTE_POINT_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "ROUTE_POINT_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence RUNWAY_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "RUNWAY_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence SSA_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "SSA_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence SSA_POINT_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "SSA_POINT_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence ZONE_AIRPORT_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "ZONE_AIRPORT_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence ZONE_BOUNDARY_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "ZONE_BOUNDARY_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence ZONE_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "ZONE_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Sequence ZONE_POINT_ID_SEQ
--------------------------------------------------------

   CREATE SEQUENCE  "ZONE_POINT_ID_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Table AIRPORT
--------------------------------------------------------

  CREATE TABLE "AIRPORT" 
   (	"ID" NUMBER(10,0), 
	"ICAO" VARCHAR2(5 CHAR), 
	"NAME" VARCHAR2(5 CHAR), 
	"LATITUDE" FLOAT(126), 
	"LONGITUDE" FLOAT(126), 
	"ALTITUDE" FLOAT(126) DEFAULT NULL
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "AIRPORT"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "AIRPORT"."ICAO" IS 'Geographical zone code IATA';
 
   COMMENT ON COLUMN "AIRPORT"."NAME" IS 'ICAO name';
 
   COMMENT ON COLUMN "AIRPORT"."LATITUDE" IS 'the latitude part of airport coordinates';
 
   COMMENT ON COLUMN "AIRPORT"."LONGITUDE" IS 'the longitude part of airport coordinates';
 
   COMMENT ON COLUMN "AIRPORT"."ALTITUDE" IS 'the altitude part of airport coordinates';
 
   COMMENT ON TABLE "AIRPORT"  IS 'This is airport table';
--------------------------------------------------------
--  DDL for Table BOUNDARY_POINT_TYPE
--------------------------------------------------------

  CREATE TABLE "BOUNDARY_POINT_TYPE" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(100 BYTE)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "BOUNDARY_POINT_TYPE"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "BOUNDARY_POINT_TYPE"."NAME" IS 'Textual descriprion';
 
   COMMENT ON TABLE "BOUNDARY_POINT_TYPE"  IS '?????? ??????? ???????? ????????? ??????? ???????? ????? ????? ????????? ?????.
This table provides textual descriprion for numberic type of boundary points.';
--------------------------------------------------------
--  DDL for Table HOLDING
--------------------------------------------------------

  CREATE TABLE "HOLDING" 
   (	"ID" NUMBER(10,0), 
	"POINT_ID" NUMBER(10,0), 
	"TYPE" NUMBER(10,0), 
	"IN_COURSE" FLOAT(126), 
	"TURN_DIRECTION" NUMBER(10,0), 
	"LEG_LENGTH" FLOAT(126), 
	"LEG_TIME" NUMBER(10,0), 
	"SPEED" FLOAT(126), 
	"ZONE_ID" NUMBER(10,0)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "HOLDING"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "HOLDING"."POINT_ID" IS 'The entry point of the holding zone';
 
   COMMENT ON COLUMN "HOLDING"."TYPE" IS 'Type of holding zone. For textual descriprion see HOLDING_TYPE table';
 
   COMMENT ON COLUMN "HOLDING"."IN_COURSE" IS 'Entering course';
 
   COMMENT ON COLUMN "HOLDING"."TURN_DIRECTION" IS 'Turn direction after entering holding zone';
 
   COMMENT ON COLUMN "HOLDING"."LEG_LENGTH" IS 'The length of the holding zone';
 
   COMMENT ON COLUMN "HOLDING"."LEG_TIME" IS 'The other variant leng assignment is LEG_TIME*SPEED';
 
   COMMENT ON COLUMN "HOLDING"."SPEED" IS 'The other variant leng assignment is LEG_TIME*SPEED';
 
   COMMENT ON COLUMN "HOLDING"."ZONE_ID" IS 'The zone id which holding is belonged';
 
   COMMENT ON TABLE "HOLDING"  IS 'Describe the holding zone.';
--------------------------------------------------------
--  DDL for Table HOLDING_POINT_MASTER
--------------------------------------------------------

  CREATE TABLE "HOLDING_POINT_MASTER" 
   (	"ID" NUMBER(10,0), 
	"HOLDING_ID" NUMBER(10,0), 
	"LATITUDE" FLOAT(126), 
	"LONGITUDE" FLOAT(126), 
	"SN" NUMBER(10,0)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "HOLDING_POINT_MASTER"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "HOLDING_POINT_MASTER"."HOLDING_ID" IS 'The id of holdinf zone';
 
   COMMENT ON COLUMN "HOLDING_POINT_MASTER"."LATITUDE" IS 'The latitude coordinate part of the boundaty point';
 
   COMMENT ON COLUMN "HOLDING_POINT_MASTER"."LONGITUDE" IS 'The longitude coordinate part of the boundaty point';
 
   COMMENT ON COLUMN "HOLDING_POINT_MASTER"."SN" IS 'The suquintal number of the point';
 
   COMMENT ON TABLE "HOLDING_POINT_MASTER"  IS 'Polygonal aproximation of the holding zone. Contains points with linked with line.';
--------------------------------------------------------
--  DDL for Table HOLDING_TURN
--------------------------------------------------------

  CREATE TABLE "HOLDING_TURN" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(100 BYTE)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "HOLDING_TURN"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "HOLDING_TURN"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "HOLDING_TURN"  IS 'Provide convertion from numberic type of type of turning inside holding zone to textual';
--------------------------------------------------------
--  DDL for Table HOLDING_TYPE
--------------------------------------------------------

  CREATE TABLE "HOLDING_TYPE" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(100 BYTE)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "HOLDING_TYPE"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "HOLDING_TYPE"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "HOLDING_TYPE"  IS 'Provide convertion from numberic type of holding zone to textual';
--------------------------------------------------------
--  DDL for Table POINT
--------------------------------------------------------

  CREATE TABLE "POINT" 
   (	"ID" NUMBER(10,0), 
	"TYPE" NUMBER(10,0), 
	"NAME" VARCHAR2(10 CHAR), 
	"LATITUDE" FLOAT(126), 
	"LONGITUDE" FLOAT(126), 
	"ICAO" VARCHAR2(5 CHAR), 
	"AIRPORT" VARCHAR2(5 CHAR)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "POINT"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "POINT"."TYPE" IS 'Type of the point. For textual description see POINT_TYPE table';
 
   COMMENT ON COLUMN "POINT"."NAME" IS 'The short name (as usual less then 7 letters) of the point.';
 
   COMMENT ON COLUMN "POINT"."LATITUDE" IS 'The latitude part of the coordinates';
 
   COMMENT ON COLUMN "POINT"."LONGITUDE" IS 'The longitude part of the coordinates';
 
   COMMENT ON COLUMN "POINT"."ICAO" IS 'The ICAO index';
 
   COMMENT ON COLUMN "POINT"."AIRPORT" IS 'I pretty dont know what is this =(';
 
   COMMENT ON TABLE "POINT"  IS 'Monument point. It could belong to zone.';
--------------------------------------------------------
--  DDL for Table POINT_TYPE
--------------------------------------------------------

  CREATE TABLE "POINT_TYPE" 
   (	"ID" NUMBER, 
	"NAME" VARCHAR2(20 BYTE)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "POINT_TYPE"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "POINT_TYPE"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "POINT_TYPE"  IS 'Provide convertion from numberic type of point to textual';
--------------------------------------------------------
--  DDL for Table RESTRICTION
--------------------------------------------------------

  CREATE TABLE "RESTRICTION" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(30 CHAR), 
	"TYPE" NUMBER(10,0), 
	"DESIGNATION" VARCHAR2(10 CHAR), 
	"ZONE_ID" NUMBER(10,0), 
	"MULTI_CODE" VARCHAR2(1 CHAR), 
	"H_MIN" NUMBER(10,0), 
	"H_MAX" NUMBER(10,0), 
	"SCHEDULE_MODE" NUMBER(10,0)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "RESTRICTION"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "RESTRICTION"."NAME" IS 'Name of restriction zone';
 
   COMMENT ON COLUMN "RESTRICTION"."TYPE" IS 'Type of restriction zone';
 
   COMMENT ON COLUMN "RESTRICTION"."ZONE_ID" IS 'FIR id which restriction zone is belonged';
 
   COMMENT ON COLUMN "RESTRICTION"."H_MIN" IS 'Bottom border of restriction zone';
 
   COMMENT ON COLUMN "RESTRICTION"."H_MAX" IS 'Top border of restriction zone';
 
   COMMENT ON TABLE "RESTRICTION"  IS 'Zones of restriction';
--------------------------------------------------------
--  DDL for Table RESTRICTION_POINT
--------------------------------------------------------

  CREATE TABLE "RESTRICTION_POINT" 
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
 

   COMMENT ON COLUMN "RESTRICTION_POINT"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "RESTRICTION_POINT"."SN" IS 'Sequence number';
 
   COMMENT ON COLUMN "RESTRICTION_POINT"."TYPE" IS 'Type of boundery point';
 
   COMMENT ON COLUMN "RESTRICTION_POINT"."LATITUDE" IS 'Latitude coordinate part';
 
   COMMENT ON COLUMN "RESTRICTION_POINT"."LONGITUDE" IS 'Longitude coordinate part';
 
   COMMENT ON COLUMN "RESTRICTION_POINT"."ARC_LATITUDE" IS 'Latitude coordinate part for arcs';
 
   COMMENT ON COLUMN "RESTRICTION_POINT"."ARC_LONGITUDE" IS 'longitude coordinate part for arcs';
 
   COMMENT ON COLUMN "RESTRICTION_POINT"."ARC_DISTANCE" IS 'Distance for arcs';
 
   COMMENT ON COLUMN "RESTRICTION_POINT"."RESTRICTION_ID" IS 'Reference to RESTRICTION.ID';
 
   COMMENT ON TABLE "RESTRICTION_POINT"  IS 'Boundary points of restriction zone';
--------------------------------------------------------
--  DDL for Table RESTRICTION_POINT_MASTER
--------------------------------------------------------

  CREATE TABLE "RESTRICTION_POINT_MASTER" 
   (	"ID" NUMBER(10,0), 
	"RESTRICTION_ID" NUMBER(10,0), 
	"LATITUDE" FLOAT(126), 
	"LONGITUDE" FLOAT(126), 
	"SN" NUMBER(10,0)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "RESTRICTION_POINT_MASTER"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "RESTRICTION_POINT_MASTER"."RESTRICTION_ID" IS 'Reference to RESTRICTION.ID';
 
   COMMENT ON COLUMN "RESTRICTION_POINT_MASTER"."LATITUDE" IS 'Latitude coordinate part of a point';
 
   COMMENT ON COLUMN "RESTRICTION_POINT_MASTER"."LONGITUDE" IS 'Longitude coordinate part of a point';
 
   COMMENT ON COLUMN "RESTRICTION_POINT_MASTER"."SN" IS 'Sequence number of points';
 
   COMMENT ON TABLE "RESTRICTION_POINT_MASTER"  IS 'Provide line aproximation for restriction zones';
--------------------------------------------------------
--  DDL for Table RESTRICTION_POINT_TYPE
--------------------------------------------------------

  CREATE TABLE "RESTRICTION_POINT_TYPE" 
   (	"ID" NUMBER, 
	"NAME" VARCHAR2(100 BYTE)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "RESTRICTION_POINT_TYPE"."ID" IS 'Primaty key';
 
   COMMENT ON COLUMN "RESTRICTION_POINT_TYPE"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "RESTRICTION_POINT_TYPE"  IS 'Provide convertion from numberic type boundary point of restriction zone to textual reprisentation';
--------------------------------------------------------
--  DDL for Table RESTRICTION_TYPE
--------------------------------------------------------

  CREATE TABLE "RESTRICTION_TYPE" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(100 BYTE)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "RESTRICTION_TYPE"."ID" IS 'Primaty key';
 
   COMMENT ON COLUMN "RESTRICTION_TYPE"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "RESTRICTION_TYPE"  IS 'Provide convertion from numberic type of restriction zone to textual reprisentation';
--------------------------------------------------------
--  DDL for Table ROUTE
--------------------------------------------------------

  CREATE TABLE "ROUTE" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(20 CHAR), 
	"ZONE_ID" NUMBER(10,0)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "ROUTE"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "ROUTE"."NAME" IS 'Zone name';
 
   COMMENT ON COLUMN "ROUTE"."ZONE_ID" IS 'Pointer to the FIR id which route is belonged';
 
   COMMENT ON TABLE "ROUTE"  IS 'Reprisent routed - the set of POINTs';
--------------------------------------------------------
--  DDL for Table ROUTE_POINT
--------------------------------------------------------

  CREATE TABLE "ROUTE_POINT" 
   (	"ID" NUMBER(10,0), 
	"SN" NUMBER(10,0), 
	"POINT_ID" NUMBER(10,0), 
	"ROUTE_ID" NUMBER(10,0), 
	"END_FLAG" NUMBER(3,0) DEFAULT '0'
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "ROUTE_POINT"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "ROUTE_POINT"."SN" IS 'Sequence number';
 
   COMMENT ON COLUMN "ROUTE_POINT"."POINT_ID" IS 'Reference to POINT.ID';
 
   COMMENT ON COLUMN "ROUTE_POINT"."ROUTE_ID" IS 'Reference to ROUTE.ID';
 
   COMMENT ON TABLE "ROUTE_POINT"  IS 'The sequence of poince for current route';
--------------------------------------------------------
--  DDL for Table RUNWAY
--------------------------------------------------------

  CREATE TABLE "RUNWAY" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(10 CHAR), 
	"LATITUDE" FLOAT(126), 
	"LONGITUDE" FLOAT(126), 
	"LENGTH" NUMBER(10,0) DEFAULT '0', 
	"AZIMUTH" FLOAT(126), 
	"AIRPORT_ID" NUMBER(10,0), 
	"WIDTH" FLOAT(3) DEFAULT 100
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "RUNWAY"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "RUNWAY"."NAME" IS 'Name of runway';
 
   COMMENT ON COLUMN "RUNWAY"."LATITUDE" IS 'Latitude coordinate part';
 
   COMMENT ON COLUMN "RUNWAY"."LONGITUDE" IS 'Longitude coordinate part';
 
   COMMENT ON COLUMN "RUNWAY"."LENGTH" IS 'Length of the runway';
 
   COMMENT ON COLUMN "RUNWAY"."AZIMUTH" IS 'Azimuth turning of runway';
 
   COMMENT ON COLUMN "RUNWAY"."AIRPORT_ID" IS 'Reference to AIRPORT.ID';
 
   COMMENT ON TABLE "RUNWAY"  IS 'Runway of an airport';
--------------------------------------------------------
--  DDL for Table SSA
--------------------------------------------------------

  CREATE TABLE "SSA" 
   (	"ID" NUMBER(10,0), 
	"RUNWAY_ID" NUMBER(10,0), 
	"NAME" VARCHAR2(10 CHAR), 
	"TYPE" NUMBER(10,0) DEFAULT NULL, 
	"TRANS_ALT" FLOAT(126)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "SSA"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "SSA"."RUNWAY_ID" IS 'Reference to RUNWAY.ID';
 
   COMMENT ON COLUMN "SSA"."NAME" IS 'Name of ssa way';
 
   COMMENT ON COLUMN "SSA"."TYPE" IS 'Type of ssa way. For textual represintation see SSA_TYPE table.';
 
   COMMENT ON TABLE "SSA"  IS 'Seed Star and aproach for the runway';
--------------------------------------------------------
--  DDL for Table SSA_POINT
--------------------------------------------------------

  CREATE TABLE "SSA_POINT" 
   (	"ID" NUMBER(10,0), 
	"SSA_ID" NUMBER(10,0), 
	"POINT_ID" NUMBER(10,0), 
	"SN" NUMBER(10,0), 
	"FLEVEL" FLOAT(126), 
	"SPEED" FLOAT(126), 
	"TYPE" NUMBER(10,0) DEFAULT NULL, 
	"COURSE" FLOAT(126), 
	"TURN" NUMBER(10,0), 
	"ALTITUDE1" FLOAT(126), 
	"FLY_OVER" NUMBER(1,0) DEFAULT 0
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "SSA_POINT"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "SSA_POINT"."SSA_ID" IS 'Reference to SSA.ID';
 
   COMMENT ON COLUMN "SSA_POINT"."SN" IS 'Sequnce number of the point';
 
   COMMENT ON COLUMN "SSA_POINT"."TYPE" IS 'Type of the point. See SSA_POINT_TYPE table';
 
   COMMENT ON COLUMN "SSA_POINT"."TURN" IS 'Turn type of the point. See SSA_POINT_TURN table';
 
   COMMENT ON TABLE "SSA_POINT"  IS 'Squence of points for SSA ways';
--------------------------------------------------------
--  DDL for Table SSA_POINT_TURN
--------------------------------------------------------

  CREATE TABLE "SSA_POINT_TURN" 
   (	"ID" NUMBER, 
	"NAME" VARCHAR2(40 BYTE)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "SSA_POINT_TURN"."ID" IS 'Primaty key';
 
   COMMENT ON COLUMN "SSA_POINT_TURN"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "SSA_POINT_TURN"  IS 'Provide convertion from numberic type of ssa turning to textual reprisentation';
--------------------------------------------------------
--  DDL for Table SSA_POINT_TYPE
--------------------------------------------------------

  CREATE TABLE "SSA_POINT_TYPE" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(40 BYTE)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "SSA_POINT_TYPE"."ID" IS 'Primaty key';
 
   COMMENT ON COLUMN "SSA_POINT_TYPE"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "SSA_POINT_TYPE"  IS 'Provide convertion from numberic type of ssa point to textual reprisentation';
--------------------------------------------------------
--  DDL for Table SSA_TYPE
--------------------------------------------------------

  CREATE TABLE "SSA_TYPE" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(40 BYTE)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "SSA_TYPE"."ID" IS 'Primaty key';
 
   COMMENT ON COLUMN "SSA_TYPE"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "SSA_TYPE"  IS 'Provide convertion from numberic type of ssa to textual reprisentation';
--------------------------------------------------------
--  DDL for Table ZONE
--------------------------------------------------------

  CREATE TABLE "ZONE" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(45 CHAR), 
	"TYPE" NUMBER(10,0), 
	"ID_NAME" VARCHAR2(10 CHAR), 
	"PARENT_ID" NUMBER(10,0) DEFAULT NULL, 
	"LOW_LIMIT" NUMBER(10,0) DEFAULT 0, 
	"UP_LIMIT" NUMBER(10,0) DEFAULT 0, 
	"SHORT_NAME" VARCHAR2(100 CHAR), 
	"LABEL_NAME" VARCHAR2(100 CHAR),
	FREQUENCY FLOAT(126)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "ZONE"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "ZONE"."NAME" IS 'The full name of zone.';
 
   COMMENT ON COLUMN "ZONE"."TYPE" IS 'Type of zone (FIR/SECTOR/etc). There is descriprion in the ZONE_TYPE table.';
 
   COMMENT ON COLUMN "ZONE"."ID_NAME" IS 'The unique textual id of zone. As usual contains 4 capital letters';
 
   COMMENT ON COLUMN "ZONE"."PARENT_ID" IS 'Pointer to the zone which include current zone. This is actual for FIR-Sector including. If zone is not included the value is null.';
 
   COMMENT ON COLUMN "ZONE"."LOW_LIMIT" IS 'Lowest limit for sectors';
 
   COMMENT ON COLUMN "ZONE"."UP_LIMIT" IS 'Higest limit for sectors';
 
   COMMENT ON TABLE "ZONE"  IS '???????? ?????? ? ????? ? ????????.
Contains FIRs and sectors. I hope we will separate sectors and firs... one day...';
--------------------------------------------------------
--  DDL for Table ZONE_AIRPORT
--------------------------------------------------------

  CREATE TABLE "ZONE_AIRPORT" 
   (	"ID" NUMBER(10,0), 
	"ZONE_ID" NUMBER(10,0), 
	"AIRPORT_ID" NUMBER(10,0)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "ZONE_AIRPORT"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "ZONE_AIRPORT"."ZONE_ID" IS 'Foreign key to ZONE.ID';
 
   COMMENT ON COLUMN "ZONE_AIRPORT"."AIRPORT_ID" IS 'Foreign key to AIRPORT.ID';
 
   COMMENT ON TABLE "ZONE_AIRPORT"  IS 'Provide many-to-many link belonging AIRPORT to ZONE. Do not foregot recalculate this table after changing AIRPORT or ZONE table.';
--------------------------------------------------------
--  DDL for Table ZONE_BOUNDARY
--------------------------------------------------------

  CREATE TABLE "ZONE_BOUNDARY" 
   (	"ID" NUMBER(10,0), 
	"TYPE" NUMBER(10,0), 
	"SN" NUMBER(10,0), 
	"LATITUDE" FLOAT(126) DEFAULT 0, 
	"LONGITUDE" FLOAT(126) DEFAULT 0, 
	"ZONE_ID" NUMBER(10,0), 
	"ARC_LATITUDE" FLOAT(126) DEFAULT 0, 
	"ARC_LONGITUDE" FLOAT(126) DEFAULT 0, 
	"ARC_DISTANCE" FLOAT(126) DEFAULT 0
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "ZONE_BOUNDARY"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "ZONE_BOUNDARY"."TYPE" IS 'Type of the line. For textual description see BOUNDARY_POINT_TYPE';
 
   COMMENT ON COLUMN "ZONE_BOUNDARY"."SN" IS 'The sequence number of the boundary point.';
 
   COMMENT ON COLUMN "ZONE_BOUNDARY"."LATITUDE" IS 'The latitude part of coordiants';
 
   COMMENT ON COLUMN "ZONE_BOUNDARY"."LONGITUDE" IS 'The longitude part of coordiants';
 
   COMMENT ON COLUMN "ZONE_BOUNDARY"."ZONE_ID" IS 'The reference to zone which point is belonged';
 
   COMMENT ON COLUMN "ZONE_BOUNDARY"."ARC_LATITUDE" IS 'The latitute perameter for arcs';
 
   COMMENT ON COLUMN "ZONE_BOUNDARY"."ARC_LONGITUDE" IS 'The longitude perameter for arcs';
 
   COMMENT ON COLUMN "ZONE_BOUNDARY"."ARC_DISTANCE" IS 'The radius of circle for arcs';
 
   COMMENT ON TABLE "ZONE_BOUNDARY"  IS 'Provide boundary for zones.';
--------------------------------------------------------
--  DDL for Table ZONE_POINT
--------------------------------------------------------

  CREATE TABLE "ZONE_POINT" 
   (	"ID" NUMBER(10,0), 
	"ZONE_ID" NUMBER(10,0), 
	"POINT_ID" NUMBER(10,0)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "ZONE_POINT"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "ZONE_POINT"."ZONE_ID" IS 'Foreign key to ZONE.ID';
 
   COMMENT ON COLUMN "ZONE_POINT"."POINT_ID" IS 'Foreign key to POINT.ID';
 
   COMMENT ON TABLE "ZONE_POINT"  IS 'Many-to-many lick describing belong POINT to ZONE. Do not foreget recalculate this table afret change in zones or points.';
--------------------------------------------------------
--  DDL for Table ZONE_TYPE
--------------------------------------------------------

  CREATE TABLE "ZONE_TYPE" 
   (	"ID" NUMBER(10,0), 
	"NAME" VARCHAR2(80 BYTE)
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "ZONE_TYPE"."ID" IS 'Primaty key';
 
   COMMENT ON COLUMN "ZONE_TYPE"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "ZONE_TYPE"  IS 'Provide textual desctiption for ZONE.TYPE id';

---------------------------------------------------
--   DATA FOR TABLE AIRPORT
--   FILTER = none used
---------------------------------------------------
REM INSERTING into AIRPORT

---------------------------------------------------
--   END DATA FOR TABLE AIRPORT
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE BOUNDARY_POINT_TYPE
--   FILTER = none used
---------------------------------------------------
REM INSERTING into BOUNDARY_POINT_TYPE
Insert into BOUNDARY_POINT_TYPE (ID,NAME) values (71,'Line');
Insert into BOUNDARY_POINT_TYPE (ID,NAME) values (67,'Circle');
Insert into BOUNDARY_POINT_TYPE (ID,NAME) values (76,'LeftArc');
Insert into BOUNDARY_POINT_TYPE (ID,NAME) values (82,'RightArc');

---------------------------------------------------
--   END DATA FOR TABLE BOUNDARY_POINT_TYPE
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE HOLDING
--   FILTER = none used
---------------------------------------------------
REM INSERTING into HOLDING

---------------------------------------------------
--   END DATA FOR TABLE HOLDING
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE HOLDING_POINT_MASTER
--   FILTER = none used
---------------------------------------------------
REM INSERTING into HOLDING_POINT_MASTER

---------------------------------------------------
--   END DATA FOR TABLE HOLDING_POINT_MASTER
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE HOLDING_TURN
--   FILTER = none used
---------------------------------------------------
REM INSERTING into HOLDING_TURN
Insert into HOLDING_TURN (ID,NAME) values (76,'Left');
Insert into HOLDING_TURN (ID,NAME) values (82,'Right');

---------------------------------------------------
--   END DATA FOR TABLE HOLDING_TURN
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE HOLDING_TYPE
--   FILTER = none used
---------------------------------------------------
REM INSERTING into HOLDING_TYPE
Insert into HOLDING_TYPE (ID,NAME) values (48,'Undef');
Insert into HOLDING_TYPE (ID,NAME) values (49,'HighAlt');
Insert into HOLDING_TYPE (ID,NAME) values (50,'LowAlt');
Insert into HOLDING_TYPE (ID,NAME) values (51,'SID');
Insert into HOLDING_TYPE (ID,NAME) values (52,'STAR');
Insert into HOLDING_TYPE (ID,NAME) values (53,'Appr');
Insert into HOLDING_TYPE (ID,NAME) values (54,'MisAppr');
Insert into HOLDING_TYPE (ID,NAME) values (55,'Undef2');

---------------------------------------------------
--   END DATA FOR TABLE HOLDING_TYPE
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE POINT
--   FILTER = none used
---------------------------------------------------
REM INSERTING into POINT

---------------------------------------------------
--   END DATA FOR TABLE POINT
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE POINT_TYPE
--   FILTER = none used
---------------------------------------------------
REM INSERTING into POINT_TYPE
Insert into POINT_TYPE (ID,NAME) values (0,'Unknown');
Insert into POINT_TYPE (ID,NAME) values (1,'VHF');
Insert into POINT_TYPE (ID,NAME) values (2,'NDB');
Insert into POINT_TYPE (ID,NAME) values (3,'Waypoint');
Insert into POINT_TYPE (ID,NAME) values (6,'Airport waypoint');
Insert into POINT_TYPE (ID,NAME) values (7,'Airport NDB');

---------------------------------------------------
--   END DATA FOR TABLE POINT_TYPE
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE RESTRICTION
--   FILTER = none used
---------------------------------------------------
REM INSERTING into RESTRICTION

---------------------------------------------------
--   END DATA FOR TABLE RESTRICTION
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE RESTRICTION_POINT
--   FILTER = none used
---------------------------------------------------
REM INSERTING into RESTRICTION_POINT

---------------------------------------------------
--   END DATA FOR TABLE RESTRICTION_POINT
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE RESTRICTION_POINT_MASTER
--   FILTER = none used
---------------------------------------------------
REM INSERTING into RESTRICTION_POINT_MASTER

---------------------------------------------------
--   END DATA FOR TABLE RESTRICTION_POINT_MASTER
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE RESTRICTION_POINT_TYPE
--   FILTER = none used
---------------------------------------------------
REM INSERTING into RESTRICTION_POINT_TYPE
Insert into RESTRICTION_POINT_TYPE (ID,NAME) values (71,'Line');
Insert into RESTRICTION_POINT_TYPE (ID,NAME) values (67,'Circle');
Insert into RESTRICTION_POINT_TYPE (ID,NAME) values (76,'LeftArc');
Insert into RESTRICTION_POINT_TYPE (ID,NAME) values (82,'RightArc');

---------------------------------------------------
--   END DATA FOR TABLE RESTRICTION_POINT_TYPE
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE RESTRICTION_TYPE
--   FILTER = none used
---------------------------------------------------
REM INSERTING into RESTRICTION_TYPE
Insert into RESTRICTION_TYPE (ID,NAME) values (65,'Alert');
Insert into RESTRICTION_TYPE (ID,NAME) values (67,'Caution');
Insert into RESTRICTION_TYPE (ID,NAME) values (68,'Danger');
Insert into RESTRICTION_TYPE (ID,NAME) values (77,'Military');
Insert into RESTRICTION_TYPE (ID,NAME) values (80,'Prohibited');
Insert into RESTRICTION_TYPE (ID,NAME) values (82,'Restricted');
Insert into RESTRICTION_TYPE (ID,NAME) values (84,'Training');
Insert into RESTRICTION_TYPE (ID,NAME) values (87,'Warning');
Insert into RESTRICTION_TYPE (ID,NAME) values (85,'Unknown');

---------------------------------------------------
--   END DATA FOR TABLE RESTRICTION_TYPE
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE ROUTE
--   FILTER = none used
---------------------------------------------------
REM INSERTING into ROUTE

---------------------------------------------------
--   END DATA FOR TABLE ROUTE
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE ROUTE_POINT
--   FILTER = none used
---------------------------------------------------
REM INSERTING into ROUTE_POINT

---------------------------------------------------
--   END DATA FOR TABLE ROUTE_POINT
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE RUNWAY
--   FILTER = none used
---------------------------------------------------
REM INSERTING into RUNWAY

---------------------------------------------------
--   END DATA FOR TABLE RUNWAY
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE SSA
--   FILTER = none used
---------------------------------------------------
REM INSERTING into SSA

---------------------------------------------------
--   END DATA FOR TABLE SSA
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE SSA_POINT
--   FILTER = none used
---------------------------------------------------
REM INSERTING into SSA_POINT

---------------------------------------------------
--   END DATA FOR TABLE SSA_POINT
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE SSA_POINT_TURN
--   FILTER = none used
---------------------------------------------------
REM INSERTING into SSA_POINT_TURN
Insert into SSA_POINT_TURN (ID,NAME) values (32,'Unknown');
Insert into SSA_POINT_TURN (ID,NAME) values (76,'Left');
Insert into SSA_POINT_TURN (ID,NAME) values (82,'Right');

---------------------------------------------------
--   END DATA FOR TABLE SSA_POINT_TURN
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE SSA_POINT_TYPE
--   FILTER = none used
---------------------------------------------------
REM INSERTING into SSA_POINT_TYPE
Insert into SSA_POINT_TYPE (ID,NAME) values (0,'Unknown');
Insert into SSA_POINT_TYPE (ID,NAME) values (1,'IF');
Insert into SSA_POINT_TYPE (ID,NAME) values (2,'TF');
Insert into SSA_POINT_TYPE (ID,NAME) values (3,'CF');
Insert into SSA_POINT_TYPE (ID,NAME) values (4,'DF');
Insert into SSA_POINT_TYPE (ID,NAME) values (5,'CA');
Insert into SSA_POINT_TYPE (ID,NAME) values (6,'CI');

---------------------------------------------------
--   END DATA FOR TABLE SSA_POINT_TYPE
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE SSA_TYPE
--   FILTER = none used
---------------------------------------------------
REM INSERTING into SSA_TYPE
Insert into SSA_TYPE (ID,NAME) values (0,'Unknown');
Insert into SSA_TYPE (ID,NAME) values (1,'SID');
Insert into SSA_TYPE (ID,NAME) values (2,'STAR');
Insert into SSA_TYPE (ID,NAME) values (3,'Approach');

---------------------------------------------------
--   END DATA FOR TABLE SSA_TYPE
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE ZONE
--   FILTER = none used
---------------------------------------------------
REM INSERTING into ZONE

---------------------------------------------------
--   END DATA FOR TABLE ZONE
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE ZONE_AIRPORT
--   FILTER = none used
---------------------------------------------------
REM INSERTING into ZONE_AIRPORT

---------------------------------------------------
--   END DATA FOR TABLE ZONE_AIRPORT
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE ZONE_BOUNDARY
--   FILTER = none used
---------------------------------------------------
REM INSERTING into ZONE_BOUNDARY

---------------------------------------------------
--   END DATA FOR TABLE ZONE_BOUNDARY
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE ZONE_POINT
--   FILTER = none used
---------------------------------------------------
REM INSERTING into ZONE_POINT

---------------------------------------------------
--   END DATA FOR TABLE ZONE_POINT
---------------------------------------------------


---------------------------------------------------
--   DATA FOR TABLE ZONE_TYPE
--   FILTER = none used
---------------------------------------------------
REM INSERTING into ZONE_TYPE
Insert into ZONE_TYPE (ID,NAME) values (1,'FIR/UIR');
Insert into ZONE_TYPE (ID,NAME) values (2,'Sector');

---------------------------------------------------
--   END DATA FOR TABLE ZONE_TYPE
---------------------------------------------------

--------------------------------------------------------
--  Constraints for Table ZONE_TYPE
--------------------------------------------------------

  ALTER TABLE "ZONE_TYPE" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE_TYPE" MODIFY ("NAME" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE_TYPE" ADD CONSTRAINT "ZONE_TYPE_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
--------------------------------------------------------
--  Constraints for Table HOLDING_TYPE
--------------------------------------------------------

  ALTER TABLE "HOLDING_TYPE" ADD CONSTRAINT "HOLDING_TYPE_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "HOLDING_TYPE" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING_TYPE" MODIFY ("NAME" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table HOLDING_TURN
--------------------------------------------------------

  ALTER TABLE "HOLDING_TURN" ADD CONSTRAINT "HOLDING_TURN_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "HOLDING_TURN" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING_TURN" MODIFY ("NAME" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table BOUNDARY_POINT_TYPE
--------------------------------------------------------

  ALTER TABLE "BOUNDARY_POINT_TYPE" ADD CONSTRAINT "BOUNDARY_POINT_TYPE_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "BOUNDARY_POINT_TYPE" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "BOUNDARY_POINT_TYPE" MODIFY ("NAME" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table SSA_POINT_TYPE
--------------------------------------------------------

  ALTER TABLE "SSA_POINT_TYPE" ADD CONSTRAINT "SSA_POINT_TYPE_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "SSA_POINT_TYPE" MODIFY ("ID" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RESTRICTION_POINT
--------------------------------------------------------

  ALTER TABLE "RESTRICTION_POINT" ADD CONSTRAINT "PRIMARY_10" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "RESTRICTION_POINT" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT" MODIFY ("SN" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT" MODIFY ("TYPE" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT" MODIFY ("LATITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT" MODIFY ("LONGITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT" MODIFY ("ARC_LATITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT" MODIFY ("ARC_LONGITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT" MODIFY ("ARC_DISTANCE" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT" MODIFY ("RESTRICTION_ID" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table ZONE
--------------------------------------------------------

  ALTER TABLE "ZONE" ADD CONSTRAINT "PRIMARY_13" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "ZONE" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE" MODIFY ("TYPE" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE" MODIFY ("ID_NAME" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE" MODIFY ("LOW_LIMIT" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE" MODIFY ("UP_LIMIT" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE" ADD CONSTRAINT "ZONE_UK_ID_NAME" UNIQUE ("ID_NAME", "NAME", "TYPE", "LOW_LIMIT", "UP_LIMIT", "PARENT_ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
--------------------------------------------------------
--  Constraints for Table SSA_POINT_TURN
--------------------------------------------------------

  ALTER TABLE "SSA_POINT_TURN" ADD CONSTRAINT "SSA_POINT_TURN_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "SSA_POINT_TURN" MODIFY ("ID" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table AIRPORT
--------------------------------------------------------

  ALTER TABLE "AIRPORT" ADD CONSTRAINT "PRIMARY_2" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "AIRPORT" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "AIRPORT" MODIFY ("ICAO" NOT NULL ENABLE);
 
  ALTER TABLE "AIRPORT" MODIFY ("NAME" NOT NULL ENABLE);
 
  ALTER TABLE "AIRPORT" MODIFY ("LATITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "AIRPORT" MODIFY ("LONGITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "AIRPORT" MODIFY ("ALTITUDE" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table POINT_TYPE
--------------------------------------------------------

  ALTER TABLE "POINT_TYPE" ADD CONSTRAINT "POINT_TYPE_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "POINT_TYPE" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "POINT_TYPE" MODIFY ("NAME" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table SSA_POINT
--------------------------------------------------------

  ALTER TABLE "SSA_POINT" ADD CONSTRAINT "PRIMARY_18" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "SSA_POINT" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "SSA_POINT" MODIFY ("SSA_ID" NOT NULL ENABLE);
 
  ALTER TABLE "SSA_POINT" MODIFY ("SN" NOT NULL ENABLE);
 
  ALTER TABLE "SSA_POINT" MODIFY ("TYPE" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table POINT
--------------------------------------------------------

  ALTER TABLE "POINT" ADD CONSTRAINT "PRIMARY_7" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "POINT" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "POINT" MODIFY ("TYPE" NOT NULL ENABLE);
 
  ALTER TABLE "POINT" MODIFY ("NAME" NOT NULL ENABLE);
 
  ALTER TABLE "POINT" MODIFY ("LATITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "POINT" MODIFY ("LONGITUDE" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RUNWAY
--------------------------------------------------------

  ALTER TABLE "RUNWAY" ADD CONSTRAINT "PRIMARY_12" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "RUNWAY" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "RUNWAY" MODIFY ("NAME" NOT NULL ENABLE);
 
  ALTER TABLE "RUNWAY" MODIFY ("LATITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "RUNWAY" MODIFY ("LONGITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "RUNWAY" MODIFY ("LENGTH" NOT NULL ENABLE);
 
  ALTER TABLE "RUNWAY" MODIFY ("AZIMUTH" NOT NULL ENABLE);
 
  ALTER TABLE "RUNWAY" MODIFY ("AIRPORT_ID" NOT NULL ENABLE);
 
  ALTER TABLE "RUNWAY" MODIFY ("WIDTH" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table SSA
--------------------------------------------------------

  ALTER TABLE "SSA" ADD CONSTRAINT "PRIMARY_8" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "SSA" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "SSA" MODIFY ("RUNWAY_ID" NOT NULL ENABLE);
 
  ALTER TABLE "SSA" MODIFY ("NAME" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RESTRICTION_TYPE
--------------------------------------------------------

  ALTER TABLE "RESTRICTION_TYPE" ADD CONSTRAINT "RESTRICTION_TYPE_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "RESTRICTION_TYPE" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_TYPE" MODIFY ("NAME" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table ROUTE_POINT
--------------------------------------------------------

  ALTER TABLE "ROUTE_POINT" ADD CONSTRAINT "PRIMARY_16" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "ROUTE_POINT" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "ROUTE_POINT" MODIFY ("SN" NOT NULL ENABLE);
 
  ALTER TABLE "ROUTE_POINT" MODIFY ("POINT_ID" NOT NULL ENABLE);
 
  ALTER TABLE "ROUTE_POINT" MODIFY ("ROUTE_ID" NOT NULL ENABLE);
 
  ALTER TABLE "ROUTE_POINT" MODIFY ("END_FLAG" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table ROUTE
--------------------------------------------------------

  ALTER TABLE "ROUTE" ADD CONSTRAINT "PRIMARY_15" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "ROUTE" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "ROUTE" MODIFY ("NAME" NOT NULL ENABLE);
 
  ALTER TABLE "ROUTE" MODIFY ("ZONE_ID" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table ZONE_POINT
--------------------------------------------------------

  ALTER TABLE "ZONE_POINT" ADD CONSTRAINT "PRIMARY_17" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "ZONE_POINT" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE_POINT" MODIFY ("ZONE_ID" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE_POINT" MODIFY ("POINT_ID" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RESTRICTION_POINT_TYPE
--------------------------------------------------------

  ALTER TABLE "RESTRICTION_POINT_TYPE" ADD CONSTRAINT "RESTRICTION_POINT_TYPE_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "RESTRICTION_POINT_TYPE" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT_TYPE" MODIFY ("NAME" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table HOLDING_POINT_MASTER
--------------------------------------------------------

  ALTER TABLE "HOLDING_POINT_MASTER" ADD CONSTRAINT "HOLDING_POINT_MASTER_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "HOLDING_POINT_MASTER" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING_POINT_MASTER" MODIFY ("HOLDING_ID" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING_POINT_MASTER" MODIFY ("LATITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING_POINT_MASTER" MODIFY ("LONGITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING_POINT_MASTER" MODIFY ("SN" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table SSA_TYPE
--------------------------------------------------------

  ALTER TABLE "SSA_TYPE" ADD CONSTRAINT "SSA_TYPE_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "SSA_TYPE" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "SSA_TYPE" MODIFY ("NAME" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table ZONE_BOUNDARY
--------------------------------------------------------

  ALTER TABLE "ZONE_BOUNDARY" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE_BOUNDARY" MODIFY ("TYPE" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE_BOUNDARY" MODIFY ("SN" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE_BOUNDARY" MODIFY ("ZONE_ID" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE_BOUNDARY" ADD CONSTRAINT "ZONE_BOUNDARY_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
--------------------------------------------------------
--  Constraints for Table RESTRICTION
--------------------------------------------------------

  ALTER TABLE "RESTRICTION" ADD CONSTRAINT "PRIMARY_3" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "RESTRICTION" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION" MODIFY ("TYPE" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION" MODIFY ("DESIGNATION" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION" MODIFY ("ZONE_ID" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table ZONE_AIRPORT
--------------------------------------------------------

  ALTER TABLE "ZONE_AIRPORT" ADD CONSTRAINT "PRIMARY_5" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "ZONE_AIRPORT" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE_AIRPORT" MODIFY ("ZONE_ID" NOT NULL ENABLE);
 
  ALTER TABLE "ZONE_AIRPORT" MODIFY ("AIRPORT_ID" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RESTRICTION_POINT_MASTER
--------------------------------------------------------

  ALTER TABLE "RESTRICTION_POINT_MASTER" ADD CONSTRAINT "RESTRICTION_POINT_MASTER_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "RESTRICTION_POINT_MASTER" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT_MASTER" MODIFY ("RESTRICTION_ID" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT_MASTER" MODIFY ("LATITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT_MASTER" MODIFY ("LONGITUDE" NOT NULL ENABLE);
 
  ALTER TABLE "RESTRICTION_POINT_MASTER" MODIFY ("SN" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table HOLDING
--------------------------------------------------------

  ALTER TABLE "HOLDING" ADD CONSTRAINT "PRIMARY_1" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE;
 
  ALTER TABLE "HOLDING" MODIFY ("ID" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING" MODIFY ("POINT_ID" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING" MODIFY ("TYPE" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING" MODIFY ("IN_COURSE" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING" MODIFY ("TURN_DIRECTION" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING" MODIFY ("LEG_LENGTH" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING" MODIFY ("LEG_TIME" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING" MODIFY ("SPEED" NOT NULL ENABLE);
 
  ALTER TABLE "HOLDING" MODIFY ("ZONE_ID" NOT NULL ENABLE);
--------------------------------------------------------
--  DDL for Index RECTRICTION_POINT_TYPE_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RECTRICTION_POINT_TYPE_PK" ON "BOUNDARY_POINT_TYPE" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index ZONE_UK_ID_NAME
--------------------------------------------------------

  CREATE UNIQUE INDEX "ZONE_UK_ID_NAME" ON "ZONE" ("ID_NAME", "NAME", "TYPE", "LOW_LIMIT", "UP_LIMIT", "PARENT_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index SSA_POINT_TURN_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "SSA_POINT_TURN_PK" ON "SSA_POINT_TURN" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index ZONE_TYPE_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "ZONE_TYPE_PK" ON "ZONE_TYPE" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_17
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_17" ON "ZONE_POINT" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_ROUTES_ZONEID
--------------------------------------------------------

  CREATE INDEX "FK_ROUTES_ZONEID" ON "ROUTE" ("ZONE_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_7
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_7" ON "POINT" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index HOLDING_TURN_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "HOLDING_TURN_PK" ON "HOLDING_TURN" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index HOLDING_TYPE_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "HOLDING_TYPE_PK" ON "HOLDING_TYPE" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index RESTRICTION_POINT_TYPE_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RESTRICTION_POINT_TYPE_PK" ON "RESTRICTION_POINT_TYPE" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_SECTORPOINTS_POINT_ID
--------------------------------------------------------

  CREATE INDEX "FK_SECTORPOINTS_POINT_ID" ON "ZONE_POINT" ("POINT_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_16
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_16" ON "ROUTE_POINT" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_ROUTEPOINTS_POINT_ID
--------------------------------------------------------

  CREATE INDEX "FK_ROUTEPOINTS_POINT_ID" ON "ROUTE_POINT" ("POINT_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_1
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_1" ON "HOLDING" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index ZONE_BOUNDARY_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "ZONE_BOUNDARY_PK" ON "ZONE_BOUNDARY" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_ROUTEPOINTS_ROUTID
--------------------------------------------------------

  CREATE INDEX "FK_ROUTEPOINTS_ROUTID" ON "ROUTE_POINT" ("ROUTE_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_SECTORPOINTS_ZONE_ID
--------------------------------------------------------

  CREATE INDEX "FK_SECTORPOINTS_ZONE_ID" ON "ZONE_POINT" ("ZONE_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_RESTRICTION_POINT_RESTRICTI
--------------------------------------------------------

  CREATE INDEX "FK_RESTRICTION_POINT_RESTRICTI" ON "RESTRICTION_POINT" ("RESTRICTION_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_HOLDING_ZONE_ID
--------------------------------------------------------

  CREATE INDEX "FK_HOLDING_ZONE_ID" ON "HOLDING" ("ZONE_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index RESTRICTION_POINT_MASTER_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RESTRICTION_POINT_MASTER_PK" ON "RESTRICTION_POINT_MASTER" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_ZONEAIRPORTS_AIRPORT_ID
--------------------------------------------------------

  CREATE INDEX "FK_ZONEAIRPORTS_AIRPORT_ID" ON "ZONE_AIRPORT" ("AIRPORT_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index SSA_POINT_TYPE_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "SSA_POINT_TYPE_PK" ON "SSA_POINT_TYPE" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index HOLDING_POINT_MASTER_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "HOLDING_POINT_MASTER_PK" ON "HOLDING_POINT_MASTER" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index RESTRICTION_TYPE_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RESTRICTION_TYPE_PK" ON "RESTRICTION_TYPE" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index SSA_TYPE_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "SSA_TYPE_PK" ON "SSA_TYPE" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index INDEX_UNIQUE
--------------------------------------------------------

  CREATE INDEX "INDEX_UNIQUE" ON "AIRPORT" ("ICAO", "NAME") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_SSAPOINTS_POINT_ID
--------------------------------------------------------

  CREATE INDEX "FK_SSAPOINTS_POINT_ID" ON "SSA_POINT" ("POINT_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_3
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_3" ON "RESTRICTION" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index POINT_TYPE_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "POINT_TYPE_PK" ON "POINT_TYPE" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_13
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_13" ON "ZONE" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_12
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_12" ON "RUNWAY" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_15
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_15" ON "ROUTE" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_RUNWAYS_AIRPORT_ID
--------------------------------------------------------

  CREATE INDEX "FK_RUNWAYS_AIRPORT_ID" ON "RUNWAY" ("AIRPORT_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_SSAPOINTS_SSA_ID
--------------------------------------------------------

  CREATE INDEX "FK_SSAPOINTS_SSA_ID" ON "SSA_POINT" ("SSA_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_5
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_5" ON "ZONE_AIRPORT" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_RESTRICTION_ZONE_ID
--------------------------------------------------------

  CREATE INDEX "FK_RESTRICTION_ZONE_ID" ON "RESTRICTION" ("ZONE_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_HOLDING_POINT_ID
--------------------------------------------------------

  CREATE INDEX "FK_HOLDING_POINT_ID" ON "HOLDING" ("POINT_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_2
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_2" ON "AIRPORT" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index INDEX_UNIQ_1
--------------------------------------------------------

  CREATE INDEX "INDEX_UNIQ_1" ON "ZONE" ("NAME", "ID_NAME") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_SSA_RUNWAY_ID
--------------------------------------------------------

  CREATE INDEX "FK_SSA_RUNWAY_ID" ON "SSA" ("RUNWAY_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_8
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_8" ON "SSA" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_10
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_10" ON "RESTRICTION_POINT" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index FK_ZONEAIRPORTS_ZONE_ID
--------------------------------------------------------

  CREATE INDEX "FK_ZONEAIRPORTS_ZONE_ID" ON "ZONE_AIRPORT" ("ZONE_ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
--------------------------------------------------------
--  DDL for Index PRIMARY_18
--------------------------------------------------------

  CREATE UNIQUE INDEX "PRIMARY_18" ON "SSA_POINT" ("ID") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;


--------------------------------------------------------
--  Ref Constraints for Table HOLDING
--------------------------------------------------------

  ALTER TABLE "HOLDING" ADD CONSTRAINT "FK_HOLDING_POINT_ID" FOREIGN KEY ("POINT_ID")
	  REFERENCES "POINT" ("ID") ON DELETE CASCADE ENABLE;
 
  ALTER TABLE "HOLDING" ADD CONSTRAINT "FK_HOLDING_TURN_ID" FOREIGN KEY ("TURN_DIRECTION")
	  REFERENCES "HOLDING_TURN" ("ID") ENABLE;
 
  ALTER TABLE "HOLDING" ADD CONSTRAINT "FK_HOLDING_TYPE_ID" FOREIGN KEY ("TYPE")
	  REFERENCES "HOLDING_TYPE" ("ID") ENABLE;
 
  ALTER TABLE "HOLDING" ADD CONSTRAINT "FK_HOLDING_ZONE_ID" FOREIGN KEY ("ZONE_ID")
	  REFERENCES "ZONE" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table HOLDING_POINT_MASTER
--------------------------------------------------------

  ALTER TABLE "HOLDING_POINT_MASTER" ADD CONSTRAINT "HOLDING_POINT_MASTER_HOLD_FK1" FOREIGN KEY ("HOLDING_ID")
	  REFERENCES "HOLDING" ("ID") ON DELETE CASCADE ENABLE;


--------------------------------------------------------
--  Ref Constraints for Table POINT
--------------------------------------------------------

  ALTER TABLE "POINT" ADD CONSTRAINT "FK_POINT_TYPE_ID" FOREIGN KEY ("TYPE")
	  REFERENCES "POINT_TYPE" ("ID") ENABLE;

--------------------------------------------------------
--  Ref Constraints for Table RESTRICTION
--------------------------------------------------------

  ALTER TABLE "RESTRICTION" ADD CONSTRAINT "FK_RESTRICTION_TYPE_ID" FOREIGN KEY ("TYPE")
	  REFERENCES "RESTRICTION_TYPE" ("ID") ENABLE;
 
  ALTER TABLE "RESTRICTION" ADD CONSTRAINT "FK_RESTRICTION_ZONE_ID" FOREIGN KEY ("ZONE_ID")
	  REFERENCES "ZONE" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table RESTRICTION_POINT
--------------------------------------------------------

  ALTER TABLE "RESTRICTION_POINT" ADD CONSTRAINT "FK_RESTRICTION_POINT_TYPE_ID" FOREIGN KEY ("TYPE")
	  REFERENCES "RESTRICTION_POINT_TYPE" ("ID") ENABLE;
 
  ALTER TABLE "RESTRICTION_POINT" ADD CONSTRAINT "RESTRICTION_POINT_RESTRIC_FK1" FOREIGN KEY ("RESTRICTION_ID")
	  REFERENCES "RESTRICTION" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table RESTRICTION_POINT_MASTER
--------------------------------------------------------

  ALTER TABLE "RESTRICTION_POINT_MASTER" ADD CONSTRAINT "RESTRICTION_POINT_MASTER__FK1" FOREIGN KEY ("RESTRICTION_ID")
	  REFERENCES "RESTRICTION" ("ID") ON DELETE CASCADE ENABLE;


--------------------------------------------------------
--  Ref Constraints for Table ROUTE
--------------------------------------------------------

  ALTER TABLE "ROUTE" ADD CONSTRAINT "FK_ROUTES_ZONEID" FOREIGN KEY ("ZONE_ID")
	  REFERENCES "ZONE" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table ROUTE_POINT
--------------------------------------------------------

  ALTER TABLE "ROUTE_POINT" ADD CONSTRAINT "FK_ROUTEPOINTS_POINT_ID" FOREIGN KEY ("POINT_ID")
	  REFERENCES "POINT" ("ID") ON DELETE CASCADE ENABLE;
 
  ALTER TABLE "ROUTE_POINT" ADD CONSTRAINT "FK_ROUTEPOINTS_ROUTID" FOREIGN KEY ("ROUTE_ID")
	  REFERENCES "ROUTE" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table RUNWAY
--------------------------------------------------------

  ALTER TABLE "RUNWAY" ADD CONSTRAINT "FK_RUNWAYS_AIRPORT_ID" FOREIGN KEY ("AIRPORT_ID")
	  REFERENCES "AIRPORT" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table SSA
--------------------------------------------------------

  ALTER TABLE "SSA" ADD CONSTRAINT "FK_SSA_RUNWAY_ID" FOREIGN KEY ("RUNWAY_ID")
	  REFERENCES "RUNWAY" ("ID") ON DELETE CASCADE ENABLE;
 
  ALTER TABLE "SSA" ADD CONSTRAINT "FK_SSA_TYPE_ID" FOREIGN KEY ("TYPE")
	  REFERENCES "SSA_TYPE" ("ID") ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table SSA_POINT
--------------------------------------------------------

  ALTER TABLE "SSA_POINT" ADD CONSTRAINT "FK_SSAPOINTS_SSA_ID" FOREIGN KEY ("SSA_ID")
	  REFERENCES "SSA" ("ID") ON DELETE CASCADE ENABLE;
 
  ALTER TABLE "SSA_POINT" ADD CONSTRAINT "FK_SSA_POINT_TURN_ID" FOREIGN KEY ("TURN")
	  REFERENCES "SSA_POINT_TURN" ("ID") ENABLE;
 
  ALTER TABLE "SSA_POINT" ADD CONSTRAINT "FK_SSA_POINT_TYPE_ID" FOREIGN KEY ("TYPE")
	  REFERENCES "SSA_POINT_TYPE" ("ID") ENABLE;



--------------------------------------------------------
--  Ref Constraints for Table ZONE
--------------------------------------------------------

  ALTER TABLE "ZONE" ADD CONSTRAINT "ZONE_ZONE_TYPE_FK1" FOREIGN KEY ("TYPE")
	  REFERENCES "ZONE_TYPE" ("ID") ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table ZONE_AIRPORT
--------------------------------------------------------

  ALTER TABLE "ZONE_AIRPORT" ADD CONSTRAINT "FK_ZONEAIRPORTS_AIRPORT_ID" FOREIGN KEY ("AIRPORT_ID")
	  REFERENCES "AIRPORT" ("ID") ON DELETE CASCADE ENABLE;
 
  ALTER TABLE "ZONE_AIRPORT" ADD CONSTRAINT "FK_ZONEAIRPORTS_ZONE_ID" FOREIGN KEY ("ZONE_ID")
	  REFERENCES "ZONE" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table ZONE_BOUNDARY
--------------------------------------------------------

  ALTER TABLE "ZONE_BOUNDARY" ADD CONSTRAINT "FK_BOUNDARY_POINT_TYPE_ID" FOREIGN KEY ("TYPE")
	  REFERENCES "BOUNDARY_POINT_TYPE" ("ID") ENABLE;
 
  ALTER TABLE "ZONE_BOUNDARY" ADD CONSTRAINT "ZONE_BOUNDARY_ZONE_ID" FOREIGN KEY ("ZONE_ID")
	  REFERENCES "ZONE" ("ID") ON DELETE CASCADE ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table ZONE_POINT
--------------------------------------------------------

  ALTER TABLE "ZONE_POINT" ADD CONSTRAINT "FK_SECTORPOINTS_POINT_ID" FOREIGN KEY ("POINT_ID")
	  REFERENCES "POINT" ("ID") ON DELETE CASCADE ENABLE;
 
  ALTER TABLE "ZONE_POINT" ADD CONSTRAINT "FK_SECTORPOINTS_ZONE_ID" FOREIGN KEY ("ZONE_ID")
	  REFERENCES "ZONE" ("ID") ON DELETE CASCADE ENABLE;

--------------------------------------------------------
--  DDL for Trigger AIRPORT_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "AIRPORT_ID_TRG" BEFORE INSERT OR UPDATE ON airport
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  airport_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM airport;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT airport_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "AIRPORT_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger HOLDING_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "HOLDING_ID_TRG" BEFORE INSERT OR UPDATE ON holding
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  holding_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM holding;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT holding_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "HOLDING_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger HOLDING_POINT_MASTER_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "HOLDING_POINT_MASTER_TRG" BEFORE INSERT ON HOLDING_POINT_MASTER
FOR EACH ROW 
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    SELECT HOLDING_POINT_MASTER_SEQ.NEXTVAL INTO :NEW.ID FROM DUAL;
  END COLUMN_SEQUENCES;
END;
/
ALTER TRIGGER "HOLDING_POINT_MASTER_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger POINT_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "POINT_ID_TRG" BEFORE INSERT OR UPDATE ON point
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  point_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM point;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT point_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "POINT_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RESTRICTION_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RESTRICTION_ID_TRG" BEFORE INSERT OR UPDATE ON RESTRICTION
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  restriction_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM restriction;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT restriction_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "RESTRICTION_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RESTRICTION_POINT_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RESTRICTION_POINT_ID_TRG" BEFORE INSERT OR UPDATE ON restriction_point
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  restriction_point_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM restriction_point;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT restriction_point_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "RESTRICTION_POINT_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RESTRICTION_POINT_MASTER_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RESTRICTION_POINT_MASTER_TRG" BEFORE INSERT ON RESTRICTION_POINT_MASTER
FOR EACH ROW 
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    SELECT RESTRICTION_POINT_MASTER_SEQ.NEXTVAL INTO :NEW.ID FROM DUAL;
  END COLUMN_SEQUENCES;
END;
/
ALTER TRIGGER "RESTRICTION_POINT_MASTER_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ROUTE_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "ROUTE_ID_TRG" BEFORE INSERT OR UPDATE ON route
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  route_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM route;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT route_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "ROUTE_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ROUTE_POINT_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "ROUTE_POINT_ID_TRG" BEFORE INSERT OR UPDATE ON route_point
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  route_point_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM route_point;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT route_point_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "ROUTE_POINT_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RUNWAY_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RUNWAY_ID_TRG" BEFORE INSERT OR UPDATE ON runway
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  runway_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM runway;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT runway_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "RUNWAY_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger SSA_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "SSA_ID_TRG" BEFORE INSERT OR UPDATE ON ssa
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  ssa_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM ssa;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT ssa_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "SSA_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger SSA_POINT_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "SSA_POINT_ID_TRG" BEFORE INSERT OR UPDATE ON ssa_point
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  ssa_point_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM ssa_point;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT ssa_point_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "SSA_POINT_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ZONE_AFTER_DELETE_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "ZONE_AFTER_DELETE_TRG" AFTER
  DELETE ON zone
  DECLARE
    c number;
  BEGIN
    IF DELETING THEN
     select count(*) into c from zone a where a.parent_id is not null and not exists (select * from zone b where a.parent_id = b.id);
      if c<>0 then
        delete from zone a where a.parent_id is not null and not exists (select * from zone b where a.id = b.parent_id) ;
      end if;
    END IF;
  END;
/
ALTER TRIGGER "ZONE_AFTER_DELETE_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ZONE_AIRPORT_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "ZONE_AIRPORT_ID_TRG" BEFORE INSERT OR UPDATE ON zone_airport
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  zone_airport_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM zone_airport;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT zone_airport_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "ZONE_AIRPORT_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ZONE_BOUNDARY_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "ZONE_BOUNDARY_TRG" BEFORE INSERT OR UPDATE ON zone_boundary
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  zone_boundary_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM zone;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT zone_boundary_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "ZONE_BOUNDARY_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ZONE_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "ZONE_ID_TRG" BEFORE INSERT OR UPDATE ON zone
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  zone_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM zone;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT zone_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "ZONE_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ZONE_POINT_ID_TRG
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "ZONE_POINT_ID_TRG" BEFORE INSERT OR UPDATE ON zone_point
FOR EACH ROW
DECLARE 
v_newVal NUMBER(12) := 0;
v_incval NUMBER(12) := 0;
BEGIN
  IF INSERTING AND :new.id IS NULL THEN
    SELECT  zone_point_id_SEQ.NEXTVAL INTO v_newVal FROM DUAL;
    -- If this is the first time this table have been inserted into (sequence == 1)
    IF v_newVal = 1 THEN 
      --get the max indentity value from the table
      SELECT NVL(max(id),0) INTO v_newVal FROM zone_point;
      v_newVal := v_newVal + 1;
      --set the sequence to that value
      LOOP
           EXIT WHEN v_incval>=v_newVal;
           SELECT zone_point_id_SEQ.nextval INTO v_incval FROM dual;
      END LOOP;
    END IF;
    --used to emulate LAST_INSERT_ID()
    --mysql_utilities.identity := v_newVal; 
   -- assign the value from the sequence to emulate the identity column
   :new.id := v_newVal;
  END IF;
END;
/
ALTER TRIGGER "ZONE_POINT_ID_TRG" ENABLE;
--------------------------------------------------------
--  DDL for View POINT_VIEW
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "POINT_VIEW" ("ID", "ICAO", "TYPE", "NAME", "LATITUDE", "LONGITUDE", "AIRPORT") AS 
  select id, icao, type, name, latitude, longitude, airport 
from point
UNION 
select 0 id, '' ICAO, 100 type, '_NULL_' name , 0 latitude, 0 longitude, '' airport 
FROM dual;
