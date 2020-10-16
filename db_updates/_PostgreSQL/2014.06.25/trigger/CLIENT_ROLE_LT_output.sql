-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS client_role_lt ON client_role CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_client_role_lt () RETURNS trigger AS $BODY$
BEGIN
    -- Touch the row
      update sys_tables_info set lasttouch = now() AT TIME ZONE 'UTC' where tablename ='CLIENT_ROLE';
    RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER client_role_lt
	AFTER INSERT OR UPDATE OR DELETE ON client_role FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_client_role_lt();

