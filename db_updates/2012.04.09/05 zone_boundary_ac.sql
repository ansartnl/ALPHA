CREATE OR REPLACE
TRIGGER ZONE_BOUNDARY_AC 
      AFTER INSERT OR UPDATE OR DELETE ON ZONE_BOUNDARY FOR EACH ROW
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
			('DELETE FROM ZONE_BOUNDARY WHERE ' ||
				'TYPE = ''' || :old.TYPE || ''' AND ' ||
				'SN = ''' || :old.SN || ''' AND ' ||
				'LATITUDE = ''' || :old.LATITUDE || ''' AND ' ||
				'LONGITUDE = ''' || :old.LONGITUDE || ''' AND ' ||
				'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''') AND ' ||
				'ARC_LATITUDE = ''' || :old.ARC_LATITUDE || ''' AND ' ||
				'ARC_LONGITUDE = ''' || :old.ARC_LONGITUDE || ''' AND ' ||
				'ARC_DISTANCE = ''' || :old.ARC_DISTANCE || '''');
		ELSE
			zone_pkg.get_cur_zone(:new.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO ZONE_BOUNDARY(TYPE, SN, LATITUDE, LONGITUDE, ZONE_ID, ARC_LATITUDE, ARC_LONGITUDE, ARC_DISTANCE) ' ||
				'VALUES(' ||
					'''' || :new.TYPE || ''', ' ||
					'''' || :new.SN || ''', ' ||
					'''' || :new.LATITUDE || ''', ' ||
					'''' || :new.LONGITUDE || ''', ' ||
					'(SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || '''), ' ||
					'''' || :new.ARC_LATITUDE || ''', ' ||
					'''' || :new.ARC_LONGITUDE || ''', ' ||
					'''' || :new.ARC_DISTANCE || '''' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE ZONE_BOUNDARY SET ' ||
					'TYPE = ''' || :new.TYPE || ''', ' ||
					'SN = ''' || :new.SN || ''', ' ||
					'LATITUDE = ''' || :new.LATITUDE || ''', ' ||
					'LONGITUDE = ''' || :new.LONGITUDE || ''', ' ||
					'ARC_LATITUDE = ''' || :new.ARC_LATITUDE || ''', ' ||
					'ARC_LONGITUDE = ''' || :new.ARC_LONGITUDE || ''', ' ||
					'ARC_DISTANCE = ''' || :new.ARC_DISTANCE || ''' ' ||
				'WHERE ' ||
					'TYPE = ''' || :old.TYPE || ''' AND ' ||
					'SN = ''' || :old.SN || ''' AND ' ||
					'LATITUDE = ''' || :old.LATITUDE || ''' AND ' ||
					'LONGITUDE = ''' || :old.LONGITUDE || ''' AND ' ||
					'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''') AND ' ||
					'ARC_LATITUDE = ''' || :old.ARC_LATITUDE || ''' AND ' ||
					'ARC_LONGITUDE = ''' || :old.ARC_LONGITUDE || ''' AND ' ||
					'ARC_DISTANCE = ''' || :old.ARC_DISTANCE || '''');
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER ZONE_BOUNDARY_AC ENABLE;
