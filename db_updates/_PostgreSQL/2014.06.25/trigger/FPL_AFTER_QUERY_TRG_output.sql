-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS fpl_after_query_trg ON fpl CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_fpl_after_query_trg () RETURNS trigger AS $BODY$
DECLARE
  modification_type char;
  current_user_id numeric;
  hist_message_id numeric;
BEGIN
  BEGIN
    SELECT user_id INTO STRICT  current_user_id FROM login LIMIT 1;
   EXCEPTION
    WHEN NO_DATA_FOUND THEN
      current_user_id := -1;
  END;
  
  if current_user_id <> -1 then
  UPDATE hist_fpl SET actual='0' WHERE id = OLD.id;
  
  IF TG_OP = 'DELETE' THEN
    hist_message_id := OLD.message_id;
    INSERT INTO hist_fpl
    (
      id,      fir_name, aircraft_id, taircraft, frules,
      tflight, num,      wtc,         equipment, adep,
      time,    speed,    flevel,      route,     ades,
      eet,     alt1,     alt2,        other,     trtime,
      exfpl,   visibility, modified_by, modification_type, actual, message_id, entry_fir_eet
    )
    VALUES
    (
      OLD.id,      OLD.fir_name, OLD.aircraft_id, OLD.taircraft, OLD.frules,
      OLD.tflight, OLD.num,      OLD.wtc,         OLD.equipment, OLD.adep,
      OLD.time,    OLD.speed,    OLD.flevel,      OLD.route,     OLD.ades,
      OLD.eet,     OLD.alt1,     OLD.alt2,        OLD.other,     OLD.trtime,
      OLD.exfpl,   OLD.visibility, current_user_id,    'D',           '1',         hist_message_id, OLD.entry_fir_eet
    );
  ELSE
    IF TG_OP = 'INSERT' THEN
      modification_type := 'I';
    ELSE
      modification_type := 'U';
    END IF;
    
    if (current_user_id = 0)then
      hist_message_id := NEW.message_id;
    else 
      hist_message_id := null;
    end if;
    INSERT INTO hist_fpl
    (
      id,      fir_name, aircraft_id, taircraft, frules,
      tflight, num,      wtc,         equipment, adep,
      time,    speed,    flevel,      route,     ades,
      eet,     alt1,     alt2,        other,     trtime,
      exfpl,   visibility, modified_by, modification_type, actual, message_id, entry_fir_eet
    )
    VALUES
    (
      NEW.id,      NEW.fir_name, NEW.aircraft_id, NEW.taircraft, NEW.frules,
      NEW.tflight, NEW.num,      NEW.wtc,         NEW.equipment, NEW.adep,
      NEW.time,    NEW.speed,    NEW.flevel,      NEW.route,     NEW.ades,
      NEW.eet,     NEW.alt1,     NEW.alt2,        NEW.other,     NEW.trtime,
      NEW.exfpl,   NEW.visibility, current_user_id,    modification_type, '1', hist_message_id, NEW.entry_fir_eet
    );
  END IF;
  end if;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER fpl_after_query_trg
	AFTER INSERT OR UPDATE OR DELETE ON fpl FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_fpl_after_query_trg();

