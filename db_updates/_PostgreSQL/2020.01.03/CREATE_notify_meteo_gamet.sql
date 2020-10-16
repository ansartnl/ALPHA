-- Function: aero.meteo_gamet_notify()

-- DROP FUNCTION aero.meteo_gamet_notify();

CREATE OR REPLACE FUNCTION aero.meteo_gamet_notify()
  RETURNS trigger AS
$BODY$
    BEGIN
        NOTIFY meteo_gamet, 'fired by NOTIFY';
        RETURN NULL;
    END; 
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.meteo_gamet_notify()
  OWNER TO aero;

-- Trigger: trigger_notify_meteo_gamet on aero.meteo_gamet

-- DROP TRIGGER trigger_notify_meteo_gamet ON aero.meteo_gamet;

CREATE TRIGGER trigger_notify_meteo_gamet
  AFTER INSERT OR UPDATE OR DELETE
  ON aero.meteo_gamet
  FOR EACH ROW
  EXECUTE PROCEDURE aero.meteo_gamet_notify();

