
  CREATE TABLE "AERO"."POINT_TYPE_M" 
   (	"ID" NUMBER NOT NULL ENABLE, 
	"NAME" VARCHAR2(20 BYTE) NOT NULL ENABLE, 
	 CONSTRAINT "POINT_TYPE_M_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "AERO"."POINT_TYPE_M"."ID" IS 'Primary key';
 
   COMMENT ON COLUMN "AERO"."POINT_TYPE_M"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "AERO"."POINT_TYPE_M"  IS 'Provide convertion from numberic type of point to textual';
 