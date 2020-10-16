-- Function: aero.meteo_sigmet_notify()

-- DROP FUNCTION aero.meteo_sigmet_notify();

CREATE OR REPLACE FUNCTION aero.meteo_sigmet_notify()
  RETURNS trigger AS
$BODY$
    BEGIN
        --SELECT pg_notify('mymessage', 'fired by FUNCTION');
        NOTIFY meteo_sigmet, 'fired by NOTIFY';
        RETURN NULL;
    END; 
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.meteo_sigmet_notify()
  OWNER TO aero;


-- Trigger: trigger_notify_meteo_sigmet on aero.meteo_sigmet

-- DROP TRIGGER trigger_notify_meteo_sigmet ON aero.meteo_sigmet;

CREATE TRIGGER trigger_notify_meteo_sigmet
  AFTER INSERT OR UPDATE OR DELETE
  ON aero.meteo_sigmet
  FOR EACH ROW
  EXECUTE PROCEDURE aero.meteo_sigmet_notify();

