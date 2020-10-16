-- Function: aero.trigger_vw_zone_point_delete_trg()

-- DROP FUNCTION aero.trigger_vw_zone_point_delete_trg();

CREATE OR REPLACE FUNCTION aero.trigger_vw_zone_point_delete_trg()
  RETURNS trigger AS
$BODY$
BEGIN
    DELETE FROM 
        aero.zone_point 
    WHERE 
        zone_id = OLD.zone_id 
      AND 
        point_id = OLD.point_id;
    RETURN OLD;
END
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.trigger_vw_zone_point_delete_trg()
  OWNER TO aero;
