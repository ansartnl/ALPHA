-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS restriction_user_delete_trg ON restriction_user CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_restriction_user_delete_trg () RETURNS trigger AS $BODY$
BEGIN
  BEGIN
    insert into restriction_user_sync (id, type) values(OLD.id, 0);
  EXCEPTION
  WHEN unique_violation THEN
    update restriction_user_sync SET type = 0, lasttouch = now() where id = OLD.id;
  WHEN OTHERS THEN
    null;
  END;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER restriction_user_delete_trg
	BEFORE DELETE ON restriction_user FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_restriction_user_delete_trg();

