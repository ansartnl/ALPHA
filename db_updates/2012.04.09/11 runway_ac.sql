CREATE OR REPLACE
TRIGGER RUNWAY_AC 
      AFTER INSERT OR UPDATE OR DELETE ON RUNWAY FOR EACH ROW
DECLARE
	ac_flag NUMBER;
	
	cursorairport zone_pkg.airport_rec_type;
	curairport cursorairport%ROWTYPE;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			zone_pkg.get_cur_airport(:old.airport_id, cursorairport);
			FETCH cursorairport INTO curairport;
			CLOSE cursorairport;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM RUNWAY WHERE ' ||
				'NAME = ''' || :old.NAME || ''' AND ' ||
				'LATITUDE = ''' || :old.LATITUDE || ''' AND ' ||
				'LONGITUDE = ''' || :old.LONGITUDE || ''' AND ' ||
				'LENGTH = ''' || :old.LENGTH || ''' AND ' ||
				'AZIMUTH = ''' || :old.AZIMUTH || ''' AND ' ||
				'AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''') AND ' ||
				'WIDTH = ''' || :old.WIDTH || ''''
			);
		ELSE
			zone_pkg.get_cur_airport(:new.airport_id, cursorairport);
			FETCH cursorairport INTO curairport;
			CLOSE cursorairport;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO RUNWAY(NAME, LATITUDE, LONGITUDE, LENGTH, AZIMUTH, AIRPORT_ID, WIDTH) ' ||
				'VALUES(' ||
					'''' || :new.NAME || ''', ' ||
					'''' || :new.LATITUDE || ''', ' ||
					'''' || :new.LONGITUDE || ''', ' ||
					'''' || :new.LENGTH || ''', ' ||
					'''' || :new.AZIMUTH || ''', ' ||
					'(SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || '''), ' ||
					'''' || :new.WIDTH || '''' ||
					')');
			--UPDATE
			ELSE
				zone_pkg.get_cur_airport(:old.airport_id, cursorairport);
				FETCH cursorairport INTO curairport;
				CLOSE cursorairport;
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE RUNWAY SET ' ||
					'NAME = ''' || :new.NAME || ''', ' ||
					'LATITUDE = ''' || :new.LATITUDE || ''', ' ||
					'LONGITUDE = ''' || :new.LONGITUDE || ''', ' ||
					'LENGTH = ''' || :new.LENGTH || ''', ' ||
					'AZIMUTH = ''' || :new.AZIMUTH || ''', ' ||
					'WIDTH = ''' || :new.WIDTH || ''' ' ||
				'WHERE ' ||
					'NAME = ''' || :old.NAME || ''' AND ' ||
					'LATITUDE = ''' || :old.LATITUDE || ''' AND ' ||
					'LONGITUDE = ''' || :old.LONGITUDE || ''' AND ' ||
					'LENGTH = ''' || :old.LENGTH || ''' AND ' ||
					'AZIMUTH = ''' || :old.AZIMUTH || ''' AND ' ||
					'AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''') AND ' ||
					'WIDTH = ''' || :old.WIDTH || ''''
				);
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER RUNWAY_AC ENABLE;
