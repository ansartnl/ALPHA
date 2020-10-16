-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS aftn_abonent_group_trg ON aftn_abonent_group CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_aftn_abonent_group_trg () RETURNS trigger AS $BODY$
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    IF coalesce(NEW.ID::text, '') = '' THEN
      SELECT nextval('aftn_abonent_group_seq') INTO NEW.ID ;
    END IF;
  END COLUMN_SEQUENCES;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER aftn_abonent_group_trg
	BEFORE INSERT ON aftn_abonent_group FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_aftn_abonent_group_trg();
