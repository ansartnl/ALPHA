-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS zone_point_ac ON zone_point CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_zone_point_ac () RETURNS trigger AS $BODY$
DECLARE
	ac_flag numeric;

	cursorfir zone_pkg.zone_rec_type;
	curfir cursorfir%ROWTYPE;

	cursorpoint zone_pkg.point_rec_type;
	curpoint cursorpoint%ROWTYPE;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF TG_OP = 'DELETE' THEN
			zone_pkg.get_cur_zone(OLD.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			zone_pkg.get_cur_point(OLD.point_id, cursorpoint);
			FETCH cursorpoint INTO curpoint;
			CLOSE cursorpoint;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM ZONE_POINT WHERE ' ||
				'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''') AND ' ||
				'POINT_ID = (SELECT ID FROM POINT WHERE TYPE = ''' || curpoint.TYPE || ''' AND NAME = ''' || curpoint.NAME || ''' AND LATITUDE = ''' || curpoint.LATITUDE || ''' AND LONGITUDE = ''' || curpoint.LONGITUDE || ''')');
		ELSE
			--INSERT
			IF TG_OP = 'INSERT' THEN
				zone_pkg.get_cur_zone(NEW.zone_id, cursorfir);
				FETCH cursorfir INTO curfir;
				CLOSE cursorfir;
				zone_pkg.get_cur_point(NEW.point_id, cursorpoint);
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
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER zone_point_ac
	AFTER INSERT OR UPDATE OR DELETE ON zone_point FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_zone_point_ac();
