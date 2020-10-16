CREATE OR REPLACE
TRIGGER ZONE_AC 
      AFTER INSERT OR UPDATE OR DELETE ON ZONE FOR EACH ROW
DECLARE
	ac_flag NUMBER;
	
	cursorfir zone_pkg.zone_rec_type;
	curfir cursorfir%ROWTYPE;
				
BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM ZONE WHERE ' ||
				'ID_NAME = ''' || :old.ID_NAME || ''' AND ' ||
				'TYPE = ''' || :old.TYPE || ''' AND ' ||
				'NAME = ''' || :old.NAME || '''');
		ELSE
			zone_pkg.get_cur_zone(:new.parent_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO ZONE(NAME, TYPE, ID_NAME, PARENT_ID, LOW_LIMIT, UP_LIMIT, SHORT_NAME, LABEL_NAME, FREQUENCY) ' ||
				'VALUES(' ||
					'''' || :new.NAME || ''', ' ||
					'''' || :new.TYPE || ''', ' ||
					'''' || :new.ID_NAME || ''', ' ||
					'(SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || '''), ' ||
					'''' || :new.LOW_LIMIT || ''', ' ||
					'''' || :new.UP_LIMIT || ''', ' ||
					'''' || :new.SHORT_NAME || ''', ' ||
					'''' || :new.LABEL_NAME || ''', ' ||
					'''' || :new.FREQUENCY || '''' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE ZONE SET ' ||
					'NAME = ''' || :new.NAME || ''', ' ||
					'TYPE = ''' || :new.TYPE || ''', ' ||
					'ID_NAME = ''' || :new.ID_NAME || ''', ' ||
					'PARENT_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || '''), ' ||
					'LOW_LIMIT = ''' || :new.LOW_LIMIT || ''', ' ||
					'UP_LIMIT = ''' || :new.UP_LIMIT || ''', ' ||
					'SHORT_NAME = ''' || :new.SHORT_NAME || ''', ' ||
					'LABEL_NAME = ''' || :new.LABEL_NAME || ''', ' ||
					'FREQUENCY = ''' || :new.FREQUENCY || ''' ' ||
				'WHERE ID_NAME = ''' || :old.ID_NAME || ''' AND TYPE = ''' || :old.TYPE || ''' AND NAME = ''' || :old.NAME || '''');
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER ZONE_AC ENABLE;
