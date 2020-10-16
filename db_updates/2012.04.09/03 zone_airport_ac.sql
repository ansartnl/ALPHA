CREATE OR REPLACE
TRIGGER ZONE_AIRPORT_AC 
      AFTER INSERT OR UPDATE OR DELETE ON ZONE_AIRPORT FOR EACH ROW
DECLARE
	ac_flag NUMBER;
	
	cursorfir zone_pkg.zone_rec_type;
	curfir cursorfir%ROWTYPE;
	
	cursorairport zone_pkg.airport_rec_type;
	curairport cursorairport%ROWTYPE;
				
BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			zone_pkg.get_cur_zone(:old.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			zone_pkg.get_cur_airport(:old.airport_id, cursorairport);
			FETCH cursorairport INTO curairport;
			CLOSE cursorairport;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM ZONE_AIRPORT WHERE ' ||
				'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''') AND ' ||
				'AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''')');
		ELSE
			--INSERT
			IF INSERTING THEN
				zone_pkg.get_cur_zone(:new.zone_id, cursorfir);
				FETCH cursorfir INTO curfir;
				CLOSE cursorfir;
				zone_pkg.get_cur_airport(:new.airport_id, cursorairport);
				FETCH cursorairport INTO curairport;
				CLOSE cursorairport;
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO ZONE_AIRPORT(ZONE_ID, AIRPORT_ID) ' ||
				'VALUES(' ||
					'(SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || '''), ' ||
					'(SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''')' ||
					')');
			--UPDATE
			ELSE
				NULL;
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER ZONE_AIRPORT_AC ENABLE;
