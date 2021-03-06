-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS exercise_id_exercise_trg ON exercise CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_exercise_id_exercise_trg () RETURNS trigger AS $BODY$
BEGIN
  if coalesce(NEW.id_exercise::text, '') = '' then
   SELECT  nextval('exercise_id_exercise_seq') INTO  NEW.id_exercise ;
  end if;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER exercise_id_exercise_trg
	BEFORE INSERT ON exercise FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_exercise_id_exercise_trg();

