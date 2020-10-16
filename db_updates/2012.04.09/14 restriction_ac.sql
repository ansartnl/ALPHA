CREATE OR REPLACE
TRIGGER RESTRICTION_AC 
      AFTER INSERT OR UPDATE OR DELETE ON RESTRICTION FOR EACH ROW
DECLARE
	ac_flag NUMBER;
	
	cursorfir zone_pkg.zone_rec_type;
	curfir cursorfir%ROWTYPE;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			zone_pkg.get_cur_zone(:old.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM RESTRICTION WHERE ' ||
				'NAME ' || NVL2(:old.NAME, '= ''' || :old.NAME || '''', 'IS NULL') || ' AND ' ||
				'TYPE = ''' || :old.TYPE || ''' AND ' ||
				'DESIGNATION = ''' || :old.DESIGNATION || ''' AND ' ||
				'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')'
			);
		ELSE
			zone_pkg.get_cur_zone(:new.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO RESTRICTION(NAME, TYPE, DESIGNATION, ZONE_ID, MULTI_CODE, H_MIN, H_MAX, SCHEDULE_MODE) ' ||
				'VALUES(' ||
					'''' || :new.NAME || ''', ' ||
					'''' || :new.TYPE || ''', ' ||
					'''' || :new.DESIGNATION || ''', ' ||
					'(SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || '''), ' ||
					'''' || :new.MULTI_CODE || ''', ' ||
					'''' || :new.H_MIN || ''', ' ||
					'''' || :new.H_MAX || ''', ' ||
					'''' || :new.SCHEDULE_MODE || '''' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE RESTRICTION SET ' ||
					'NAME = ''' || :new.NAME || ''', ' ||
					'TYPE = ''' || :new.TYPE || ''', ' ||
					'DESIGNATION = ''' || :new.DESIGNATION || ''', ' ||
					'MULTI_CODE = ''' || :new.MULTI_CODE || ''', ' ||
					'H_MIN = ''' || :new.H_MIN || ''', ' ||
					'H_MAX = ''' || :new.H_MAX || ''', ' ||
					'SCHEDULE_MODE = ''' || :new.SCHEDULE_MODE || ''' ' ||
				'WHERE ' ||
					'NAME ' || NVL2(:old.NAME, '= ''' || :old.NAME || '''', 'IS NULL') || ' AND ' ||
					'TYPE = ''' || :old.TYPE || ''' AND ' ||
					'DESIGNATION = ''' || :old.DESIGNATION || ''' AND ' ||
					'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')'
				);
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER RESTRICTION_AC ENABLE;
