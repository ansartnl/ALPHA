-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS boundary_point_type_lt ON boundary_point_type CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_boundary_point_type_lt () RETURNS trigger AS $BODY$
BEGIN
    -- Touch the row
      update sys_tables_info set lasttouch = now() AT TIME ZONE 'UTC' where tablename ='BOUNDARY_POINT_TYPE';
    RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER boundary_point_type_lt
	AFTER INSERT OR UPDATE OR DELETE ON boundary_point_type FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_boundary_point_type_lt();
