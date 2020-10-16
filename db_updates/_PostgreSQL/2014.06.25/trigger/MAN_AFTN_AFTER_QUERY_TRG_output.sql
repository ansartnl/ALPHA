-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS man_aftn_after_query_trg ON man_aftn CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_man_aftn_after_query_trg () RETURNS trigger AS $BODY$
DECLARE modification_type char;
  current_user_id numeric;
  cnt          integer;
BEGIN
-- ???????? ??? ????????????
  BEGIN
    SELECT user_id INTO STRICT  current_user_id FROM login LIMIT 1;
   EXCEPTION
  -- ????? ????????? -1
    WHEN NO_DATA_FOUND THEN
      current_user_id := -1;
  END;
  
  if current_user_id <> -1 then
  -- ???? ?????? ?????????/?????????????? ?? ??????????? ?????????
    IF TG_OP = 'DELETE' THEN
    -- ????????? ?? ? ??????? ???????
    INSERT INTO hist_man_aftn
      (id, created, header, text, modified_by, modification_type)
    VALUES
      (OLD.id, OLD.created, OLD.header, OLD.text, current_user_id, 'D');
  -- ????????? ?? ? ??????? ???????
    ELSE
      IF TG_OP = 'INSERT' THEN
      modification_type := 'I';
      ELSE
        modification_type := 'U';
      END IF;
      INSERT INTO hist_man_aftn
        (id, created, header, text, modified_by, modification_type)
      VALUES
        (NEW.id, NEW.created, NEW.header, NEW.text, current_user_id, modification_type);
    END IF;
  end if;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER man_aftn_after_query_trg
	AFTER INSERT OR UPDATE OR DELETE ON man_aftn FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_man_aftn_after_query_trg();

