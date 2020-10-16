-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS point_ac ON point CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_point_ac () RETURNS trigger AS $BODY$
DECLARE
	ac_flag numeric;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF TG_OP = 'DELETE' THEN
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM POINT WHERE ' ||
				'TYPE = ''' || OLD.TYPE || ''' AND ' ||
				'NAME = ''' || OLD.NAME || ''' AND ' ||
				'LATITUDE = ''' || OLD.LATITUDE || ''' AND ' ||
				'LONGITUDE = ''' || OLD.LONGITUDE || ''' AND ' ||
				'ICAO ' || NVL2(OLD.ICAO, '= ''' || OLD.ICAO || '''', 'IS NULL') || ' AND ' ||
				'AIRPORT ' || NVL2(OLD.AIRPORT, '= ''' || OLD.AIRPORT || '''', 'IS NULL')
			);
		ELSE
			--INSERT
			IF TG_OP = 'INSERT' THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO POINT(TYPE, NAME, LATITUDE, LONGITUDE, ICAO, AIRPORT) ' ||
				'VALUES(' ||
					'''' || NEW.TYPE || ''', ' ||
					'''' || NEW.NAME || ''', ' ||
					'''' || NEW.LATITUDE || ''', ' ||
					'''' || NEW.LONGITUDE || ''', ' ||
					'''' || NEW.ICAO || ''', ' ||
					'''' || NEW.AIRPORT || '''' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE POINT SET ' ||
					'TYPE = ''' || NEW.TYPE || ''', ' ||
					'NAME = ''' || NEW.NAME || ''', ' ||
					'LATITUDE = ''' || NEW.LATITUDE || ''', ' ||
					'LONGITUDE = ''' || NEW.LONGITUDE || ''', ' ||
					'ICAO = ''' || NEW.ICAO || ''', ' ||
					'AIRPORT = ''' || NEW.AIRPORT || ''' ' ||
				'WHERE ' ||
					'TYPE = ''' || OLD.TYPE || ''' AND ' ||
					'NAME = ''' || OLD.NAME || ''' AND ' ||
					'LATITUDE = ''' || OLD.LATITUDE || ''' AND ' ||
					'LONGITUDE = ''' || OLD.LONGITUDE || ''' AND ' ||
					'ICAO ' || NVL2(OLD.ICAO, '= ''' || OLD.ICAO || '''', 'IS NULL') || ' AND ' ||
					'AIRPORT ' || NVL2(OLD.AIRPORT, '= ''' || OLD.AIRPORT || '''', 'IS NULL')
				);
			END IF;
		END IF;
	END IF;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER point_ac
	AFTER INSERT OR UPDATE OR DELETE ON point FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_point_ac();
