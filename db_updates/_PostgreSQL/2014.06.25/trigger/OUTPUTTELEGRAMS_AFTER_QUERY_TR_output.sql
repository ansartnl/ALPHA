-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS outputtelegrams_after_query_tr ON outputtelegrams CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_outputtelegrams_after_query_tr () RETURNS trigger AS $BODY$
DECLARE
	modification_type	char;
	current_user_id		numeric;
BEGIN
  BEGIN
    SELECT user_id INTO STRICT  current_user_id FROM login LIMIT 1;
   EXCEPTION
    WHEN NO_DATA_FOUND THEN
      current_user_id := -1;
  END;
  
  IF current_user_id <> -1 THEN
    IF TG_OP = 'DELETE' THEN
    INSERT INTO hist_outputtelegrams
      (id, created, header, text, addresses, modified_by, modification_type)
    VALUES
      (OLD.id, OLD.created, OLD.header, OLD.text, OLD.addresses, current_user_id, 'D');
    ELSE
      IF TG_OP = 'INSERT' THEN
        modification_type := 'I';
      --ELSE
        --modification_type := 'U';
      --END IF;
      INSERT INTO hist_outputtelegrams
        (id, created, header, text, addresses, modified_by, modification_type)
      VALUES
        (NEW.id, NEW.created, NEW.header, NEW.text, NEW.addresses, current_user_id, modification_type);
      END IF;--
    END IF;
  END IF;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER outputtelegrams_after_query_tr
	AFTER INSERT OR UPDATE OR DELETE ON outputtelegrams FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_outputtelegrams_after_query_tr();
