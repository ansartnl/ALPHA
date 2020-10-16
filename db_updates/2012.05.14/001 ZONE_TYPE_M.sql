
  CREATE TABLE "AERO"."ZONE_TYPE_M" 
   (	"ID" NUMBER(10,0) NOT NULL ENABLE, 
	"NAME" VARCHAR2(80 BYTE) NOT NULL ENABLE, 
	 CONSTRAINT "ZONE_TYPE_M_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 

   COMMENT ON COLUMN "AERO"."ZONE_TYPE_M"."ID" IS 'Primaty key';
 
   COMMENT ON COLUMN "AERO"."ZONE_TYPE_M"."NAME" IS 'Textual description';
 
   COMMENT ON TABLE "AERO"."ZONE_TYPE_M"  IS 'Provide textual desctiption for ZONE.TYPE id';
 
