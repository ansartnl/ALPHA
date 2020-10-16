-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS arinc_changes_trg ON arinc_changes CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_arinc_changes_trg () RETURNS trigger AS $BODY$
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    SELECT nextval('arinc_changes_seq') INTO NEW.ID ;
  END COLUMN_SEQUENCES;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER arinc_changes_trg
	BEFORE INSERT ON arinc_changes FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_arinc_changes_trg();
