-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS airplane_speed_lt ON airplane_speed CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_airplane_speed_lt () RETURNS trigger AS $BODY$
BEGIN
    -- Touch the row
      update sys_tables_info set lasttouch = now() AT TIME ZONE 'UTC' where tablename ='AIRPLANE_SPEED';
    RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER airplane_speed_lt
	AFTER INSERT OR UPDATE OR DELETE ON airplane_speed FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_airplane_speed_lt();

