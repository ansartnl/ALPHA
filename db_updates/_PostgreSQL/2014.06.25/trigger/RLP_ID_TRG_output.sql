-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS rlp_id_trg ON rlp CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_rlp_id_trg () RETURNS trigger AS $BODY$
BEGIN
  if coalesce(NEW.id::text, '') = '' then
    SELECT nextval('rlp_id_seq') INTO NEW.ID ;
  end if;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER rlp_id_trg
	BEFORE INSERT ON rlp FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_rlp_id_trg();

