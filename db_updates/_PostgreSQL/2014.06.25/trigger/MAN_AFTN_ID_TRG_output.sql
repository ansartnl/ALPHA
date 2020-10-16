-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS man_aftn_id_trg ON man_aftn CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_man_aftn_id_trg () RETURNS trigger AS $BODY$
BEGIN
  if coalesce(NEW.id::text, '') = '' then
    SELECT "MAN_AFTN_ID_SEQ".nextval INTO NEW.id ;
  end if;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER man_aftn_id_trg
	BEFORE INSERT ON man_aftn FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_man_aftn_id_trg();

