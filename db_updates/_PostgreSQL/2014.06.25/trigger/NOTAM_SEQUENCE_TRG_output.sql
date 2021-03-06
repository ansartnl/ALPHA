-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS notam_sequence_trg ON notam CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_notam_sequence_trg () RETURNS trigger AS $BODY$
declare
   updated_sequence_id numeric;
  BEGIN
    if NEW.nature = 'C' or NEW.nature = 'R' then
      updated_sequence_id := NULL;
      UPDATE notam SET state = NEW.nature 
        where series = NEW.replace_series 
          and year = NEW.replace_year
          and snumber = NEW.replace_snumber
          returning sequence_id into updated_sequence_id;
      if coalesce(updated_sequence_id::text, '') = '' then
          RAISE EXCEPTION 'ERROR: NOTAM for replacing not exists'; --ERROR: NOTAM for replacing not exists
      end if;
      NEW.sequence_id := updated_sequence_id;
      if NEW.nature = 'C' then
        NEW.state := 'L';
      end if;
    end if;
  RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER notam_sequence_trg
	BEFORE INSERT ON notam FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_notam_sequence_trg();

