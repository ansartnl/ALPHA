-- Function: aero.meteo_taf_notify()

-- DROP FUNCTION aero.meteo_taf_notify();

CREATE OR REPLACE FUNCTION aero.meteo_taf_notify()
  RETURNS trigger AS
$BODY$
    BEGIN
        NOTIFY meteo_taf, 'fired by NOTIFY';
        RETURN NULL;
    END; 
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.meteo_taf_notify()
  OWNER TO aero;

-- Trigger: trigger_notify_meteo_taf on aero.meteo_taf

-- DROP TRIGGER trigger_notify_meteo_taf ON aero.meteo_taf;

CREATE TRIGGER trigger_notify_meteo_taf
  AFTER INSERT OR UPDATE OR DELETE
  ON aero.meteo_taf
  FOR EACH ROW
  EXECUTE PROCEDURE aero.meteo_taf_notify();

