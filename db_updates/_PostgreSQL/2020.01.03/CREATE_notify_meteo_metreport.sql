-- Function: aero.meteo_metreport_notify()

-- DROP FUNCTION aero.meteo_metreport_notify();

CREATE OR REPLACE FUNCTION aero.meteo_metreport_notify()
  RETURNS trigger AS
$BODY$
    BEGIN
        NOTIFY meteo_metreport, 'fired by NOTIFY';
        RETURN NULL;
    END; 
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.meteo_metreport_notify()
  OWNER TO aero;

-- Trigger: trigger_notify_meteo_metreport on aero.meteo_metreport

-- DROP TRIGGER trigger_notify_meteo_metreport ON aero.meteo_metreport;

CREATE TRIGGER trigger_notify_meteo_metreport
  AFTER INSERT OR UPDATE OR DELETE
  ON aero.meteo_metreport
  FOR EACH ROW
  EXECUTE PROCEDURE aero.meteo_metreport_notify();

