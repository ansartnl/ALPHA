DROP TABLE OLDI_CLIENTS;

CREATE TABLE OLDI_CLIENTS
  (
    "ID" NUMBER NOT NULL ENABLE,
    "CLIENT" VARCHAR2(8 BYTE) NOT NULL ENABLE,
    "STATUS" NUMBER(1,0) NOT NULL ENABLE,
    "SECTOR" VARCHAR2(100 BYTE) NOT NULL ENABLE,
	 CONSTRAINT "OLDI_CLIENTS_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;

ALTER TABLE OLDI_CLIENTS
ADD CONSTRAINT OLDI_CLIENTS_UK1 UNIQUE 
(
  SECTOR 
)
ENABLE;
  
  CREATE SEQUENCE  OLDI_CLIENTS_SEQ  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
  
CREATE OR REPLACE TRIGGER OLDI_CLIENTS_TRG BEFORE INSERT ON OLDI_CLIENTS 
FOR EACH ROW 
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    SELECT OLDI_CLIENTS_SEQ.NEXTVAL INTO :NEW.ID FROM DUAL;
  END COLUMN_SEQUENCES;
END;
/
ALTER TRIGGER OLDI_CLIENTS_TRG ENABLE;

CREATE OR REPLACE TRIGGER OLDI_CLIENTS_LT AFTER
  INSERT OR
  UPDATE OR
  DELETE ON OLDI_CLIENTS FOR EACH ROW BEGIN
  -- Touch the row
  UPDATE sys_tables_info
  SET lasttouch   = SYS_EXTRACT_UTC(SYSTIMESTAMP)
  WHERE tablename ='OLDI_CLIENTS';
END;
/
ALTER TRIGGER OLDI_CLIENTS_LT ENABLE;

CREATE OR REPLACE
TRIGGER OLDI_CLIENTS_REPLICATOR 
      AFTER INSERT OR UPDATE OR DELETE ON OLDI_CLIENTS FOR EACH ROW
--WHEN ( new.fpl_guid IS NOT NULL OR old.fpl_guid IS NOT NULL )
DECLARE
	curtablename CONSTANT REPLICATION_TABLE.table_name%TYPE := 'OLDI_CLIENTS';
	lastinsertid NUMBER;
	curguid RAW(16);
	replicationflag NUMBER;
	
BEGIN
	replicationflag := sys_context('CLIENTCONTEXT', 'REPLICATION_FLAG');
	--DELETE
	IF DELETING THEN
		SELECT (SELECT col_guid_value
		FROM REPLICATION_TABLE_LINKS 
		WHERE col_id_value = :old.id AND table_name = curtablename) INTO curguid FROM dual;
		
		IF curguid IS NOT NULL THEN
			IF replicationflag IS NULL OR replicationflag <> 777 THEN
				INSERT INTO REPLICATION_TABLE
				(oper_type, table_name, col_guid_value)
				VALUES
				('DELETE', curtablename, curguid)
				RETURNING id INTO lastinsertid;
			END IF;
			
			--DELETE FROM replication_table_links WHERE col_id_value = :old.id AND table_name = curtablename;
			DELETE FROM replication_table_links WHERE col_guid_value = curguid;
		END IF;
	ELSE
		--INSERT
		IF INSERTING THEN
			IF replicationflag IS NULL OR replicationflag <> 777 THEN
				INSERT INTO REPLICATION_TABLE_LINKS
				(table_name, col_id_name, col_id_value)
				VALUES
				(curtablename, 'ID', :new.id)
				RETURNING col_guid_value INTO curguid;
			ELSE
				curguid := sys_context('CLIENTCONTEXT', 'REPLICATION_GUID');
				IF curguid IS NOT NULL THEN
					INSERT INTO REPLICATION_TABLE_LINKS
					(table_name, col_id_name, col_id_value, col_guid_value)
					VALUES
					(curtablename, 'ID', :new.id, curguid);
				END IF;
			END IF;
				
			IF (replicationflag IS NULL OR replicationflag <> 777) AND curguid IS NOT NULL THEN
				INSERT INTO REPLICATION_TABLE
				(oper_type, table_name, col_guid_value)
				VALUES
				('INSERT', curtablename, curguid)
				RETURNING id INTO lastinsertid;
			END IF;
			
		--UPDATE
		ELSE
			IF replicationflag IS NULL OR replicationflag <> 777 THEN
				SELECT (SELECT col_guid_value
				FROM REPLICATION_TABLE_LINKS 
				WHERE col_id_value = :new.id AND table_name = curtablename) INTO curguid FROM dual;
				
				IF curguid IS NOT NULL THEN
					INSERT INTO REPLICATION_TABLE
					(oper_type, table_name, col_guid_value)
					VALUES
					('UPDATE', curtablename, curguid)
					RETURNING id INTO lastinsertid;
				END IF;
			END IF;
		END IF;
		
		IF (replicationflag IS NULL OR replicationflag <> 777) AND curguid IS NOT NULL THEN
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'CLIENT', '''' || :new.CLIENT || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'STATUS', '''' || :new.STATUS || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'SECTOR', '''' || :new.SECTOR || '''');
		END IF;
	END IF;
END;
/
ALTER TRIGGER OLDI_CLIENTS_REPLICATOR ENABLE;
