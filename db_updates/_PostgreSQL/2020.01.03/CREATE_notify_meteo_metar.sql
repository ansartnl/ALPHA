-- Function: aero.meteo_metar_notify()

-- DROP FUNCTION aero.meteo_metar_notify();

CREATE OR REPLACE FUNCTION aero.meteo_metar_notify()
  RETURNS trigger AS
$BODY$
    BEGIN
        NOTIFY meteo_metar, 'fired by NOTIFY';
        RETURN NULL;
    END; 
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.meteo_metar_notify()
  OWNER TO aero;

-- Trigger: trigger_notify_meteo_metar on aero.meteo_metar

-- DROP TRIGGER trigger_notify_meteo_metar ON aero.meteo_metar;

CREATE TRIGGER trigger_notify_meteo_metar
  AFTER INSERT OR UPDATE OR DELETE
  ON aero.meteo_metar
  FOR EACH ROW
  EXECUTE PROCEDURE aero.meteo_metar_notify();

