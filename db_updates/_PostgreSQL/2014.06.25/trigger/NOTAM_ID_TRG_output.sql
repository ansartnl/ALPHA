-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS notam_id_trg ON notam CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_notam_id_trg () RETURNS trigger AS $BODY$
BEGIN
  
  if coalesce(NEW.id::text, '') = '' then
    SELECT "NOTAM_ID_SEQ".nextval INTO NEW.ID ;
  end if;
  PERFORM coalesce(NEW.SEQUENCE_ID, NEW.ID) INTO NEW.SEQUENCE_ID ;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER notam_id_trg
	BEFORE INSERT ON notam FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_notam_id_trg();

