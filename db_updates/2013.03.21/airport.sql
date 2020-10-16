ALTER TABLE AIRPORT 
ADD (DECLINATION VARCHAR2(5 CHAR) );


  CREATE OR REPLACE TRIGGER "AERO"."AIRPORT_AC" 
      AFTER INSERT OR UPDATE OR DELETE ON AIRPORT FOR EACH ROW
DECLARE
	ac_flag NUMBER;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM AIRPORT WHERE ICAO = ''' || :old.ICAO || ''' AND NAME = ''' || :old.NAME || '''');
		ELSE
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO AIRPORT(ICAO, NAME, LATITUDE, LONGITUDE, ALTITUDE, DECLINATION) ' ||
					'VALUES(''' || :new.ICAO || ''', ''' || :new.NAME || ''', ''' || :new.LATITUDE || ''', ''' || :new.LONGITUDE || ''', ''' || :new.ALTITUDE || ''', ''' || :new.DECLINATION || ''')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE AIRPORT SET ' ||
					'ICAO = ''' || :new.ICAO || ''', ' ||
					'NAME = ''' || :new.NAME || ''', ' ||
					'LATITUDE = ''' || :new.LATITUDE || ''', ' ||
					'LONGITUDE = ''' || :new.LONGITUDE || ''', ' ||
          'ALTITUDE = ''' || :new.ALTITUDE || ''', ' ||
					'DECLINATION = ''' || :new.DECLINATION || ''' ' ||
					'WHERE ICAO = ''' || :old.ICAO || ''' AND NAME = ''' || :old.NAME || '''');
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER "AERO"."AIRPORT_AC" ENABLE;
 
