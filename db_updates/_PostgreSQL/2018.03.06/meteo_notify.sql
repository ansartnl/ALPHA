CREATE OR REPLACE FUNCTION meteo_notify()
  RETURNS trigger AS
$BODY$
    BEGIN
        NOTIFY meteo;
        RETURN NULL;
    END; 
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION meteo_notify() OWNER TO aero;

CREATE TRIGGER trigger_notify_meteo
AFTER INSERT OR UPDATE OR DELETE OR TRUNCATE ON meteo EXECUTE PROCEDURE meteo_notify();
