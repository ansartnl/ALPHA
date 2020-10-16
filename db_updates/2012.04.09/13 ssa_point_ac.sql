CREATE OR REPLACE
TRIGGER SSA_POINT_AC 
      AFTER INSERT OR UPDATE OR DELETE ON SSA_POINT FOR EACH ROW
DECLARE
	ac_flag NUMBER;
	
	cursorssa zone_pkg.ssa_rec_type;
	curssa cursorssa%ROWTYPE;

	cursorrunway zone_pkg.runway_rec_type;
	currunway cursorrunway%ROWTYPE;
	
	cursorairport zone_pkg.airport_rec_type;
	curairport cursorairport%ROWTYPE;

	cursorpoint zone_pkg.point_rec_type;
	newpoint cursorpoint%ROWTYPE;		
	oldpoint cursorpoint%ROWTYPE;		

	BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			zone_pkg.get_cur_ssa(:old.ssa_id, cursorssa);
			FETCH cursorssa INTO curssa;
			CLOSE cursorssa;
			zone_pkg.get_cur_runway(curssa.runway_id, cursorrunway);
			FETCH cursorrunway INTO currunway;
			CLOSE cursorrunway;
			zone_pkg.get_cur_airport(currunway.airport_id, cursorairport);
			FETCH cursorairport INTO curairport;
			CLOSE cursorairport;
			
			zone_pkg.get_cur_point(:old.point_id, cursorpoint);
			FETCH cursorpoint INTO oldpoint;
			CLOSE cursorpoint;
			
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM SSA_POINT WHERE ' ||
				'SSA_ID = (SELECT ID FROM SSA WHERE RUNWAY_ID = (SELECT ID FROM RUNWAY WHERE NAME = ''' || currunway.NAME || ''' AND LATITUDE = ''' || currunway.LATITUDE || ''' AND LONGITUDE = ''' || currunway.LONGITUDE || ''' AND LENGTH = ''' || currunway.LENGTH || ''' AND AZIMUTH = ''' || currunway.AZIMUTH || ''' AND AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''') AND WIDTH = ''' || currunway.WIDTH || ''') AND NAME = ''' || curssa.NAME || ''' AND TYPE ' || NVL2(curssa.TYPE, '= ''' || curssa.TYPE || '''', 'IS NULL') || ' AND TRANS_ALT ' || NVL2(curssa.TRANS_ALT, '= ''' || curssa.TRANS_ALT || '''', 'IS NULL') || ') AND ' ||
				'POINT_ID ' || NVL2(DECODE(:old.POINT_ID, 0, NULL, :old.POINT_ID), '= (SELECT ID FROM POINT WHERE TYPE = ''' || oldpoint.TYPE || ''' AND NAME = ''' || oldpoint.NAME || ''' AND LATITUDE = ''' || oldpoint.LATITUDE || ''' AND LONGITUDE = ''' || oldpoint.LONGITUDE || ''')', '= 0') || ' AND ' ||
				'SN = ''' || :old.SN || ''' AND ' ||
				'FLEVEL ' || NVL2(:old.FLEVEL, '= ''' || :old.FLEVEL || '''', 'IS NULL') || ' AND ' ||
				'SPEED ' || NVL2(:old.SPEED, '= ''' || :old.SPEED || '''', 'IS NULL') || ' AND ' ||
				'TYPE = ''' || :old.TYPE || ''' AND ' ||
				'COURSE ' || NVL2(:old.COURSE, '= ''' || :old.COURSE || '''', 'IS NULL') || ' AND ' ||
				'TURN ' || NVL2(:old.TURN, '= ''' || :old.TURN || '''', 'IS NULL') || ' AND ' ||
				'ALTITUDE1 ' || NVL2(:old.ALTITUDE1, '= ''' || :old.ALTITUDE1 || '''', 'IS NULL') || ' AND ' ||
				'FLY_OVER ' || NVL2(:old.FLY_OVER, '= ''' || :old.FLY_OVER || '''', 'IS NULL')
			);
		ELSE
			zone_pkg.get_cur_ssa(:new.ssa_id, cursorssa);
			FETCH cursorssa INTO curssa;
			CLOSE cursorssa;
			zone_pkg.get_cur_runway(curssa.runway_id, cursorrunway);
			FETCH cursorrunway INTO currunway;
			CLOSE cursorrunway;
			zone_pkg.get_cur_airport(currunway.airport_id, cursorairport);
			FETCH cursorairport INTO curairport;
			CLOSE cursorairport;
			
			zone_pkg.get_cur_point(:new.point_id, cursorpoint);
			FETCH cursorpoint INTO newpoint;
			CLOSE cursorpoint;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO SSA_POINT(SSA_ID, POINT_ID, SN, FLEVEL, SPEED, TYPE, COURSE, TURN, ALTITUDE1, FLY_OVER) ' ||
				'VALUES(' ||
					'(SELECT ID FROM SSA WHERE RUNWAY_ID = (SELECT ID FROM RUNWAY WHERE NAME = ''' || currunway.NAME || ''' AND LATITUDE = ''' || currunway.LATITUDE || ''' AND LONGITUDE = ''' || currunway.LONGITUDE || ''' AND LENGTH = ''' || currunway.LENGTH || ''' AND AZIMUTH = ''' || currunway.AZIMUTH || ''' AND AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''') AND WIDTH = ''' || currunway.WIDTH || ''') AND NAME = ''' || curssa.NAME || ''' AND TYPE ' || NVL2(curssa.TYPE, '= ''' || curssa.TYPE || '''', 'IS NULL') || ' AND TRANS_ALT ' || NVL2(curssa.TRANS_ALT, '= ''' || curssa.TRANS_ALT || '''', 'IS NULL') || '), ' ||
					NVL2(DECODE(:new.POINT_ID, 0, NULL, :new.POINT_ID), '(SELECT ID FROM POINT WHERE TYPE = ''' || newpoint.TYPE || ''' AND NAME = ''' || newpoint.NAME || ''' AND LATITUDE = ''' || newpoint.LATITUDE || ''' AND LONGITUDE = ''' || newpoint.LONGITUDE || ''')', '0') || ', ' ||
					'''' || :new.SN || ''', ' ||
					'''' || :new.FLEVEL || ''', ' ||
					'''' || :new.SPEED || ''', ' ||
					'''' || :new.TYPE || ''', ' ||
					'''' || :new.COURSE || ''', ' ||
					'''' || :new.TURN || ''', ' ||
					'''' || :new.ALTITUDE1 || ''', ' ||
					'''' || :new.FLY_OVER || '''' ||
					')');
			--UPDATE
			ELSE
				zone_pkg.get_cur_point(:old.point_id, cursorpoint);
				FETCH cursorpoint INTO oldpoint;
				CLOSE cursorpoint;
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE SSA_POINT SET ' ||
					'POINT_ID = ' || NVL2(DECODE(:new.POINT_ID, 0, NULL, :new.POINT_ID), '(SELECT ID FROM POINT WHERE TYPE = ''' || newpoint.TYPE || ''' AND NAME = ''' || newpoint.NAME || ''' AND LATITUDE = ''' || newpoint.LATITUDE || ''' AND LONGITUDE = ''' || newpoint.LONGITUDE || ''')', '0') || ', ' ||
					'SN = ''' || :new.SN || ''', ' ||
					'FLEVEL = ''' || :new.FLEVEL || ''', ' ||
					'SPEED = ''' || :new.SPEED || ''', ' ||
					'TYPE = ''' || :new.TYPE || ''', ' ||
					'COURSE = ''' || :new.COURSE || ''', ' ||
					'TURN = ''' || :new.TURN || ''', ' ||
					'ALTITUDE1 = ''' || :new.ALTITUDE1 || ''', ' ||
					'FLY_OVER = ''' || :new.FLY_OVER || ''' ' ||
				'WHERE ' ||
					'SSA_ID = (SELECT ID FROM SSA WHERE RUNWAY_ID = (SELECT ID FROM RUNWAY WHERE NAME = ''' || currunway.NAME || ''' AND LATITUDE = ''' || currunway.LATITUDE || ''' AND LONGITUDE = ''' || currunway.LONGITUDE || ''' AND LENGTH = ''' || currunway.LENGTH || ''' AND AZIMUTH = ''' || currunway.AZIMUTH || ''' AND AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''') AND WIDTH = ''' || currunway.WIDTH || ''') AND NAME = ''' || curssa.NAME || ''' AND TYPE ' || NVL2(curssa.TYPE, '= ''' || curssa.TYPE || '''', 'IS NULL') || ' AND TRANS_ALT ' || NVL2(curssa.TRANS_ALT, '= ''' || curssa.TRANS_ALT || '''', 'IS NULL') || ') AND ' ||
					'POINT_ID ' || NVL2(DECODE(:old.POINT_ID, 0, NULL, :old.POINT_ID), '= (SELECT ID FROM POINT WHERE TYPE = ''' || oldpoint.TYPE || ''' AND NAME = ''' || oldpoint.NAME || ''' AND LATITUDE = ''' || oldpoint.LATITUDE || ''' AND LONGITUDE = ''' || oldpoint.LONGITUDE || ''')', '= 0') || ' AND ' ||
					'SN = ''' || :old.SN || ''' AND ' ||
					'FLEVEL ' || NVL2(:old.FLEVEL, '= ''' || :old.FLEVEL || '''', 'IS NULL') || ' AND ' ||
					'SPEED ' || NVL2(:old.SPEED, '= ''' || :old.SPEED || '''', 'IS NULL') || ' AND ' ||
					'TYPE = ''' || :old.TYPE || ''' AND ' ||
					'COURSE ' || NVL2(:old.COURSE, '= ''' || :old.COURSE || '''', 'IS NULL') || ' AND ' ||
					'TURN ' || NVL2(:old.TURN, '= ''' || :old.TURN || '''', 'IS NULL') || ' AND ' ||
					'ALTITUDE1 ' || NVL2(:old.ALTITUDE1, '= ''' || :old.ALTITUDE1 || '''', 'IS NULL') || ' AND ' ||
					'FLY_OVER ' || NVL2(:old.FLY_OVER, '= ''' || :old.FLY_OVER || '''', 'IS NULL')
				);
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER SSA_POINT_AC ENABLE;
