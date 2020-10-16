-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS users_admin ON users CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_users_admin () RETURNS trigger AS $BODY$
BEGIN
  if TG_OP = 'DELETE' then
    if OLD.id = 1 then
      RAISE EXCEPTION 'ERROR: superuser can not be deleted';
    end if;
    if OLD.id = 0 then
      RAISE EXCEPTION 'ERROR: aftnservice user can not be deleted';
    end if;
  elsif TG_OP = 'UPDATE' then
    if OLD.id = 1 and NEW.isactive = 0
      then
      RAISE EXCEPTION 'ERROR: superuser can not be updated';
    end if;
    if OLD.id = 0 then
      RAISE EXCEPTION 'ERROR: aftnservice user can not be deleted';
    end if;
  elsif TG_OP = 'INSERT' then
    NULL;
  end if;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER users_admin
	BEFORE INSERT OR UPDATE OR DELETE ON users FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_users_admin();
