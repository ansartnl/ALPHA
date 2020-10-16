CREATE OR REPLACE
TRIGGER SSA_AC 
      AFTER INSERT OR UPDATE OR DELETE ON SSA FOR EACH ROW
DECLARE
	ac_flag NUMBER;
	
	cursorrunway zone_pkg.runway_rec_type;
	currunway cursorrunway%ROWTYPE;
	
	cursorairport zone_pkg.airport_rec_type;
	curairport cursorairport%ROWTYPE;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			zone_pkg.get_cur_runway(:old.runway_id, cursorrunway);
			FETCH cursorrunway INTO currunway;
			CLOSE cursorrunway;
			zone_pkg.get_cur_airport(currunway.airport_id, cursorairport);
			FETCH cursorairport INTO curairport;
			CLOSE cursorairport;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM SSA WHERE ' ||
				'RUNWAY_ID = (SELECT ID FROM RUNWAY WHERE NAME = ''' || currunway.NAME || ''' AND LATITUDE = ''' || currunway.LATITUDE || ''' AND LONGITUDE = ''' || currunway.LONGITUDE || ''' AND LENGTH = ''' || currunway.LENGTH || ''' AND AZIMUTH = ''' || currunway.AZIMUTH || ''' AND AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''') AND WIDTH = ''' || currunway.WIDTH || ''') AND ' ||
				'NAME = ''' || :old.NAME || ''' AND ' ||
				'TYPE ' || NVL2(:old.TYPE, '= ''' || :old.TYPE || '''', 'IS NULL') || ' AND ' ||
				'TRANS_ALT ' || NVL2(:old.TRANS_ALT, '= ''' || :old.TRANS_ALT || '''', 'IS NULL')
			);
		ELSE
			zone_pkg.get_cur_runway(:new.runway_id, cursorrunway);
			FETCH cursorrunway INTO currunway;
			CLOSE cursorrunway;
			zone_pkg.get_cur_airport(currunway.airport_id, cursorairport);
			FETCH cursorairport INTO curairport;
			CLOSE cursorairport;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO SSA(RUNWAY_ID, NAME, TYPE, TRANS_ALT) ' ||
				'VALUES(' ||
					'(SELECT ID FROM RUNWAY WHERE NAME = ''' || currunway.NAME || ''' AND LATITUDE = ''' || currunway.LATITUDE || ''' AND LONGITUDE = ''' || currunway.LONGITUDE || ''' AND LENGTH = ''' || currunway.LENGTH || ''' AND AZIMUTH = ''' || currunway.AZIMUTH || ''' AND AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''') AND WIDTH = ''' || currunway.WIDTH || '''), ' ||
					'''' || :new.NAME || ''', ' ||
					'''' || :new.TYPE || ''', ' ||
					'''' || :new.TRANS_ALT || '''' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE SSA SET ' ||
					'NAME = ''' || :new.NAME || ''', ' ||
					'TYPE = ''' || :new.TYPE || ''', ' ||
					'TRANS_ALT = ''' || :new.TRANS_ALT || ''' ' ||
				'WHERE ' ||
					'RUNWAY_ID = (SELECT ID FROM RUNWAY WHERE NAME = ''' || currunway.NAME || ''' AND LATITUDE = ''' || currunway.LATITUDE || ''' AND LONGITUDE = ''' || currunway.LONGITUDE || ''' AND LENGTH = ''' || currunway.LENGTH || ''' AND AZIMUTH = ''' || currunway.AZIMUTH || ''' AND AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''') AND WIDTH = ''' || currunway.WIDTH || ''') AND ' ||
					'NAME = ''' || :old.NAME || ''' AND ' ||
					'TYPE ' || NVL2(:old.TYPE, '= ''' || :old.TYPE || '''', 'IS NULL') || ' AND ' ||
					'TRANS_ALT ' || NVL2(:old.TRANS_ALT, '= ''' || :old.TRANS_ALT || '''', 'IS NULL')
				);
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER SSA_AC ENABLE;
