
-- Function: fpl_notify()

-- DROP FUNCTION fpl_notify();

CREATE OR REPLACE FUNCTION fpl_notify()
  RETURNS trigger AS
$BODY$
    BEGIN
        --SELECT pg_notify('mymessage', 'fired by FUNCTION');
        NOTIFY fpl, 'fired by NOTIFY';
        RETURN NULL;
    END; 
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION fpl_notify() OWNER TO aero;

CREATE TRIGGER trigger_notify_fpl
AFTER INSERT OR UPDATE OR DELETE ON fpl FOR EACH ROW EXECUTE PROCEDURE fpl_notify();

commit;