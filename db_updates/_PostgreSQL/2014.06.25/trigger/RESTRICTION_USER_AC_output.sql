-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS restriction_user_ac ON restriction_user CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_restriction_user_ac () RETURNS trigger AS $BODY$
DECLARE
	ac_flag numeric;

	cursorfir zone_pkg.zone_rec_type;
	curfir cursorfir%ROWTYPE;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF TG_OP = 'DELETE' THEN
			zone_pkg.get_cur_zone(OLD.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM RESTRICTION_USER WHERE ' ||
				'NAME ' || NVL2(OLD.NAME, '= ''' || OLD.NAME || '''', 'IS NULL') || ' AND ' ||
				'TYPE = ''' || OLD.TYPE || ''' AND ' ||
				'GUID ' || NVL2(OLD.GUID, '= ''' || OLD.GUID || '''', 'IS NULL') || ' AND ' ||
				'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')'
			);
		ELSE
			zone_pkg.get_cur_zone(NEW.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			--INSERT
			IF TG_OP = 'INSERT' THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO RESTRICTION_USER(NAME, ZONE_ID, H_MIN, H_MAX, TIME_START, TIME_END, TYPE, GUID) ' ||
				'VALUES(' ||
					'''' || NEW.NAME || ''', ' ||
					'(SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || '''), ' ||
					'''' || NEW.H_MIN || ''', ' ||
					'''' || NEW.H_MAX || ''', ' ||
					NVL2(NEW.TIME_START, 'to_timestamp(''' || to_char(NEW.TIME_START, 'YYYY.MM.DD HH24:MI:SS.FF') || ''', ''YYYY.MM.DD HH24:MI:SS.FF'')', '''''') || ', ' ||
					NVL2(NEW.TIME_END, 'to_timestamp(''' || to_char(NEW.TIME_END, 'YYYY.MM.DD HH24:MI:SS.FF') || ''', ''YYYY.MM.DD HH24:MI:SS.FF'')', '''''') || ', ' ||
					'''' || NEW.TYPE || ''', ' ||
					'''' || NEW.GUID || '''' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE RESTRICTION_USER SET ' ||
					'NAME = ''' || NEW.NAME || ''', ' ||
					'H_MIN = ''' || NEW.H_MIN || ''', ' ||
					'H_MAX = ''' || NEW.H_MAX || ''', ' ||
					'TIME_START = ' || NVL2(NEW.TIME_START, 'to_timestamp(''' || to_char(NEW.TIME_START, 'YYYY.MM.DD HH24:MI:SS.FF') || ''', ''YYYY.MM.DD HH24:MI:SS.FF'')', '''''') || ', ' ||
					'TIME_END = ' || NVL2(NEW.TIME_END, 'to_timestamp(''' || to_char(NEW.TIME_END, 'YYYY.MM.DD HH24:MI:SS.FF') || ''', ''YYYY.MM.DD HH24:MI:SS.FF'')', '''''') || ', ' ||
					'TYPE = ''' || NEW.TYPE || ''', ' ||
					'GUID = ''' || NEW.GUID || ''' ' ||
				'WHERE ' ||
					'NAME ' || NVL2(OLD.NAME, '= ''' || OLD.NAME || '''', 'IS NULL') || ' AND ' ||
					'TYPE = ''' || OLD.TYPE || ''' AND ' ||
					'GUID ' || NVL2(OLD.GUID, '= ''' || OLD.GUID || '''', 'IS NULL') || ' AND ' ||
					'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')'
				);
			END IF;
		END IF;
	END IF;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER restriction_user_ac
	AFTER INSERT OR UPDATE OR DELETE ON restriction_user FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_restriction_user_ac();
