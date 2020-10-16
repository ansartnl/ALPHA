CREATE OR REPLACE
TRIGGER FPL_REPLICATOR 
      AFTER INSERT OR UPDATE OR DELETE ON FPL FOR EACH ROW
--WHEN ( new.fpl_guid IS NOT NULL OR old.fpl_guid IS NOT NULL )
DECLARE
	curtablename CONSTANT REPLICATION_TABLE.table_name%TYPE := 'FPL';
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
			VALUES (lastinsertid, 'FIR_NAME', '''' || :new.FIR_NAME || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'AIRCRAFT_ID', '''' || :new.AIRCRAFT_ID || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'TAIRCRAFT', '''' || :new.TAIRCRAFT || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'FRULES', '''' || :new.FRULES || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'TFLIGHT', '''' || :new.TFLIGHT || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'NUM', '''' || :new.NUM || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'WTC', '''' || :new.WTC || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'EQUIPMENT', '''' || :new.EQUIPMENT || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'ADEP', '''' || :new.ADEP || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'TIME', 'to_timestamp(''' || to_char(:new.TIME, 'YYYY.MM.DD HH24:MI:SS.FF') || ''', ''YYYY.MM.DD HH24:MI:SS.FF'')');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'SPEED', '''' || :new.SPEED || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'FLEVEL', '''' || :new.FLEVEL || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'ROUTE', '''' || :new.ROUTE || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'ADES', '''' || :new.ADES || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'EET', '''' || :new.EET || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'ALT1', '''' || :new.ALT1 || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'ALT2', '''' || :new.ALT2 || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'OTHER', '''' || :new.OTHER || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'TRTIME', 'sys_extract_utc(systimestamp)');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'EXFPL', '''' || :new.EXFPL || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'VISIBILITY', '''' || :new.VISIBILITY || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'MESSAGE_ID', '''' || :new.MESSAGE_ID || '''');
			INSERT INTO REPLICATION_TABLE_VALUES (oper_id, col_name, col_value)
			VALUES (lastinsertid, 'ENTRY_FIR_EET', '''' || :new.ENTRY_FIR_EET || '''');
		END IF;
	END IF;
END;
/
ALTER TRIGGER FPL_REPLICATOR ENABLE;