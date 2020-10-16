-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS level_types_lt ON level_types CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_level_types_lt () RETURNS trigger AS $BODY$
BEGIN
    -- Touch the row
      update sys_tables_info set lasttouch = now() AT TIME ZONE 'UTC' where tablename ='LEVEL_TYPES';
    RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER level_types_lt
	AFTER INSERT OR UPDATE OR DELETE ON level_types FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_level_types_lt();

