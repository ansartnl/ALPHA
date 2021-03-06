
  DROP TABLE "LOGIN";

  CREATE GLOBAL TEMPORARY TABLE "LOGIN" 
   (	"USER_ID" NUMBER, 
	"USER_NAME" VARCHAR2(32 CHAR), 
	"LOGGED_IN" TIMESTAMP (6) DEFAULT SYS_EXTRACT_UTC(SYSTIMESTAMP) NOT NULL ENABLE, 
	"REPLICATE" CHAR(1 CHAR) DEFAULT 1 NOT NULL ENABLE
   ) ON COMMIT PRESERVE ROWS ;
 

   COMMENT ON COLUMN "LOGIN"."USER_ID" IS 'User identifier';
 
   COMMENT ON COLUMN "LOGIN"."USER_NAME" IS 'User name';
 
   COMMENT ON COLUMN "LOGIN"."LOGGED_IN" IS 'When user logged in';
 
   COMMENT ON TABLE "LOGIN"  IS 'Temporary table for storing login user information';
 
  
  CREATE TABLE "LOGIN_LOG"
   (	"USER_ID" NUMBER, 
	"USER_NAME" VARCHAR2(64 CHAR), 
	"LOGGED_TIME" TIMESTAMP (6) DEFAULT SYS_EXTRACT_UTC(SYSTIMESTAMP) NOT NULL ENABLE, 
	"LOGGED_TYPE" CHAR(1 CHAR) DEFAULT 1 NOT NULL ENABLE
	)
  PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING STORAGE
  (
    INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645 PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT
  )
  TABLESPACE "USERS" ; 

   COMMENT ON COLUMN "LOGIN_LOG"."USER_ID" IS 'User identifier';
 
   COMMENT ON COLUMN "LOGIN_LOG"."USER_NAME" IS 'User name';
 
   COMMENT ON COLUMN "LOGIN_LOG"."LOGGED_TIME" IS 'When user logged in/out';
   
   COMMENT ON COLUMN "LOGIN_LOG"."LOGGED_TYPE" IS 'Type in/out';
 
   COMMENT ON TABLE "LOGIN_LOG"  IS 'Table for storing login user information';
 