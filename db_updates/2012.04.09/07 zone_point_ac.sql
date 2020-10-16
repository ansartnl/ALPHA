CREATE OR REPLACE
TRIGGER ZONE_POINT_AC 
      AFTER INSERT OR UPDATE OR DELETE ON ZONE_POINT FOR EACH ROW
DECLARE
	ac_flag NUMBER;
	
	cursorfir zone_pkg.zone_rec_type;
	curfir cursorfir%ROWTYPE;
	
	cursorpoint zone_pkg.point_rec_type;
	curpoint cursorpoint%ROWTYPE;
		
BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			zone_pkg.get_cur_zone(:old.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			zone_pkg.get_cur_point(:old.point_id, cursorpoint);
			FETCH cursorpoint INTO curpoint;
			CLOSE cursorpoint;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM ZONE_POINT WHERE ' ||
				'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''') AND ' ||
				'POINT_ID = (SELECT ID FROM POINT WHERE TYPE = ''' || curpoint.TYPE || ''' AND NAME = ''' || curpoint.NAME || ''' AND LATITUDE = ''' || curpoint.LATITUDE || ''' AND LONGITUDE = ''' || curpoint.LONGITUDE || ''')');
		ELSE
			--INSERT
			IF INSERTING THEN
				zone_pkg.get_cur_zone(:new.zone_id, cursorfir);
				FETCH cursorfir INTO curfir;
				CLOSE cursorfir;
				zone_pkg.get_cur_point(:new.point_id, cursorpoint);
				FETCH cursorpoint INTO curpoint;
				CLOSE cursorpoint;
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO ZONE_POINT(ZONE_ID, POINT_ID) ' ||
				'VALUES(' ||
					'(SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || '''), ' ||
					'(SELECT ID FROM POINT WHERE TYPE = ''' || curpoint.TYPE || ''' AND NAME = ''' || curpoint.NAME || ''' AND LATITUDE = ''' || curpoint.LATITUDE || ''' AND LONGITUDE = ''' || curpoint.LONGITUDE || ''')' ||
					')');
			--UPDATE
			ELSE
				NULL;
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER ZONE_POINT_AC ENABLE;
