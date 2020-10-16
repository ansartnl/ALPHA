CREATE OR REPLACE
TRIGGER HOLDING_AC 
      AFTER INSERT OR UPDATE OR DELETE ON HOLDING FOR EACH ROW
DECLARE
	ac_flag NUMBER;
	
	cursorfir zone_pkg.zone_rec_type;
	curfir cursorfir%ROWTYPE;

	cursorpoint zone_pkg.point_rec_type;
	newpoint cursorpoint%ROWTYPE;		
	oldpoint cursorpoint%ROWTYPE;		

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			zone_pkg.get_cur_zone(:old.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			zone_pkg.get_cur_point(:old.point_id, cursorpoint);
			FETCH cursorpoint INTO oldpoint;
			CLOSE cursorpoint;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM HOLDING WHERE ' ||
				'POINT_ID = (SELECT ID FROM POINT WHERE TYPE = ''' || oldpoint.TYPE || ''' AND NAME = ''' || oldpoint.NAME || ''' AND LATITUDE = ''' || oldpoint.LATITUDE || ''' AND LONGITUDE = ''' || oldpoint.LONGITUDE || ''') AND ' ||
				'TYPE = ''' || :old.TYPE || ''' AND ' ||
				'IN_COURSE = ''' || :old.IN_COURSE || ''' AND ' ||
				'TURN_DIRECTION = ''' || :old.TURN_DIRECTION || ''' AND ' ||
				'LEG_LENGTH = ''' || :old.LEG_LENGTH || ''' AND ' ||
				'LEG_TIME = ''' || :old.LEG_TIME || ''' AND ' ||
				'SPEED = ''' || :old.SPEED || ''' AND ' ||
				'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')'
			);
		ELSE
			zone_pkg.get_cur_zone(:new.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			zone_pkg.get_cur_point(:new.point_id, cursorpoint);
			FETCH cursorpoint INTO newpoint;
			CLOSE cursorpoint;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO HOLDING(POINT_ID, TYPE, IN_COURSE, TURN_DIRECTION, LEG_LENGTH, LEG_TIME, SPEED, ZONE_ID) ' ||
				'VALUES(' ||
					'(SELECT ID FROM POINT WHERE TYPE = ''' || newpoint.TYPE || ''' AND NAME = ''' || newpoint.NAME || ''' AND LATITUDE = ''' || newpoint.LATITUDE || ''' AND LONGITUDE = ''' || newpoint.LONGITUDE || '''), ' ||
					'''' || :new.TYPE || ''', ' ||
					'''' || :new.IN_COURSE || ''', ' ||
					'''' || :new.TURN_DIRECTION || ''', ' ||
					'''' || :new.LEG_LENGTH || ''', ' ||
					'''' || :new.LEG_TIME || ''', ' ||
					'''' || :new.SPEED || ''', ' ||
					'(SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')' ||
					')');
			--UPDATE
			ELSE
				zone_pkg.get_cur_point(:old.point_id, cursorpoint);
				FETCH cursorpoint INTO oldpoint;
				CLOSE cursorpoint;
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE HOLDING SET ' ||
					'POINT_ID = (SELECT ID FROM POINT WHERE TYPE = ''' || newpoint.TYPE || ''' AND NAME = ''' || newpoint.NAME || ''' AND LATITUDE = ''' || newpoint.LATITUDE || ''' AND LONGITUDE = ''' || newpoint.LONGITUDE || '''), ' ||
					'TYPE = ''' || :new.TYPE || ''', ' ||
					'IN_COURSE = ''' || :new.IN_COURSE || ''', ' ||
					'TURN_DIRECTION = ''' || :new.TURN_DIRECTION || ''', ' ||
					'LEG_LENGTH = ''' || :new.LEG_LENGTH || ''', ' ||
					'LEG_TIME = ''' || :new.LEG_TIME || ''', ' ||
					'SPEED = ''' || :new.SPEED || ''' ' ||
				'WHERE ' ||
					'POINT_ID = (SELECT ID FROM POINT WHERE TYPE = ''' || oldpoint.TYPE || ''' AND NAME = ''' || oldpoint.NAME || ''' AND LATITUDE = ''' || oldpoint.LATITUDE || ''' AND LONGITUDE = ''' || oldpoint.LONGITUDE || ''') AND ' ||
					'TYPE = ''' || :old.TYPE || ''' AND ' ||
					'IN_COURSE = ''' || :old.IN_COURSE || ''' AND ' ||
					'TURN_DIRECTION = ''' || :old.TURN_DIRECTION || ''' AND ' ||
					'LEG_LENGTH = ''' || :old.LEG_LENGTH || ''' AND ' ||
					'LEG_TIME = ''' || :old.LEG_TIME || ''' AND ' ||
					'SPEED = ''' || :old.SPEED || ''' AND ' ||
					'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')'
				);
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER HOLDING_AC ENABLE;
