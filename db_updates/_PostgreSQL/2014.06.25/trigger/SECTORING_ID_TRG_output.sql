-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS sectoring_id_trg ON sectoring CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_sectoring_id_trg () RETURNS trigger AS $BODY$
BEGIN
  if coalesce(NEW.id::text, '') = '' then
    SELECT  nextval('sectoring_seq') INTO NEW.id ;
  end if;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER sectoring_id_trg
	BEFORE INSERT ON sectoring FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_sectoring_id_trg();

