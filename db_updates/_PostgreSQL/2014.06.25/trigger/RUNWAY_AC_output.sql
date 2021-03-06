-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS runway_ac ON runway CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_runway_ac () RETURNS trigger AS $BODY$
DECLARE
	ac_flag numeric;

	cursorairport zone_pkg.airport_rec_type;
	curairport cursorairport%ROWTYPE;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF TG_OP = 'DELETE' THEN
			zone_pkg.get_cur_airport(OLD.airport_id, cursorairport);
			FETCH cursorairport INTO curairport;
			CLOSE cursorairport;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM RUNWAY WHERE ' ||
				'NAME = ''' || OLD.NAME || ''' AND ' ||
				'LATITUDE = ''' || OLD.LATITUDE || ''' AND ' ||
				'LONGITUDE = ''' || OLD.LONGITUDE || ''' AND ' ||
				'LENGTH = ''' || OLD.LENGTH || ''' AND ' ||
				'AZIMUTH = ''' || OLD.AZIMUTH || ''' AND ' ||
				'AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''') AND ' ||
				'WIDTH = ''' || OLD.WIDTH || ''''
			);
		ELSE
			zone_pkg.get_cur_airport(NEW.airport_id, cursorairport);
			FETCH cursorairport INTO curairport;
			CLOSE cursorairport;
			--INSERT
			IF TG_OP = 'INSERT' THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO RUNWAY(NAME, LATITUDE, LONGITUDE, LENGTH, AZIMUTH, AIRPORT_ID, WIDTH) ' ||
				'VALUES(' ||
					'''' || NEW.NAME || ''', ' ||
					'''' || NEW.LATITUDE || ''', ' ||
					'''' || NEW.LONGITUDE || ''', ' ||
					'''' || NEW.LENGTH || ''', ' ||
					'''' || NEW.AZIMUTH || ''', ' ||
					'(SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || '''), ' ||
					'''' || NEW.WIDTH || '''' ||
					')');
			--UPDATE
			ELSE
				zone_pkg.get_cur_airport(OLD.airport_id, cursorairport);
				FETCH cursorairport INTO curairport;
				CLOSE cursorairport;
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE RUNWAY SET ' ||
					'NAME = ''' || NEW.NAME || ''', ' ||
					'LATITUDE = ''' || NEW.LATITUDE || ''', ' ||
					'LONGITUDE = ''' || NEW.LONGITUDE || ''', ' ||
					'LENGTH = ''' || NEW.LENGTH || ''', ' ||
					'AZIMUTH = ''' || NEW.AZIMUTH || ''', ' ||
					'WIDTH = ''' || NEW.WIDTH || ''' ' ||
				'WHERE ' ||
					'NAME = ''' || OLD.NAME || ''' AND ' ||
					'LATITUDE = ''' || OLD.LATITUDE || ''' AND ' ||
					'LONGITUDE = ''' || OLD.LONGITUDE || ''' AND ' ||
					'LENGTH = ''' || OLD.LENGTH || ''' AND ' ||
					'AZIMUTH = ''' || OLD.AZIMUTH || ''' AND ' ||
					'AIRPORT_ID = (SELECT ID FROM AIRPORT WHERE ICAO = ''' || curairport.ICAO || ''' AND NAME = ''' || curairport.NAME || ''') AND ' ||
					'WIDTH = ''' || OLD.WIDTH || ''''
				);
			END IF;
		END IF;
	END IF;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER runway_ac
	AFTER INSERT OR UPDATE OR DELETE ON runway FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_runway_ac();

