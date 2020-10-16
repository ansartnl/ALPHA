CREATE OR REPLACE
TRIGGER POINT_AC 
      AFTER INSERT OR UPDATE OR DELETE ON POINT FOR EACH ROW
DECLARE
	ac_flag NUMBER;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM POINT WHERE ' ||
				'TYPE = ''' || :old.TYPE || ''' AND ' ||
				'NAME = ''' || :old.NAME || ''' AND ' ||
				'LATITUDE = ''' || :old.LATITUDE || ''' AND ' ||
				'LONGITUDE = ''' || :old.LONGITUDE || ''' AND ' ||
				'ICAO ' || NVL2(:old.ICAO, '= ''' || :old.ICAO || '''', 'IS NULL') || ' AND ' ||
				'AIRPORT ' || NVL2(:old.AIRPORT, '= ''' || :old.AIRPORT || '''', 'IS NULL')
			);
		ELSE
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO POINT(TYPE, NAME, LATITUDE, LONGITUDE, ICAO, AIRPORT) ' ||
				'VALUES(' ||
					'''' || :new.TYPE || ''', ' ||
					'''' || :new.NAME || ''', ' ||
					'''' || :new.LATITUDE || ''', ' ||
					'''' || :new.LONGITUDE || ''', ' ||
					'''' || :new.ICAO || ''', ' ||
					'''' || :new.AIRPORT || '''' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE POINT SET ' ||
					'TYPE = ''' || :new.TYPE || ''', ' ||
					'NAME = ''' || :new.NAME || ''', ' ||
					'LATITUDE = ''' || :new.LATITUDE || ''', ' ||
					'LONGITUDE = ''' || :new.LONGITUDE || ''', ' ||
					'ICAO = ''' || :new.ICAO || ''', ' ||
					'AIRPORT = ''' || :new.AIRPORT || ''' ' ||
				'WHERE ' ||
					'TYPE = ''' || :old.TYPE || ''' AND ' ||
					'NAME = ''' || :old.NAME || ''' AND ' ||
					'LATITUDE = ''' || :old.LATITUDE || ''' AND ' ||
					'LONGITUDE = ''' || :old.LONGITUDE || ''' AND ' ||
					'ICAO ' || NVL2(:old.ICAO, '= ''' || :old.ICAO || '''', 'IS NULL') || ' AND ' ||
					'AIRPORT ' || NVL2(:old.AIRPORT, '= ''' || :old.AIRPORT || '''', 'IS NULL')
				);
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER POINT_AC ENABLE;
