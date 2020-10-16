-- Function: aero.trigger_vw_zone_point_delete_trg()

-- DROP FUNCTION aero.trigger_vw_zone_point_delete_trg();

CREATE OR REPLACE FUNCTION aero.trigger_vw_zone_point_delete_trg()
  RETURNS trigger AS
$BODY$
DECLARE routerecord RECORD;
DECLARE ssarecord RECORD;
DECLARE route_name VARCHAR(500); -- := 'in route';
DECLARE ssa_name VARCHAR(500);
BEGIN
	
    IF ((OLD.point_id NOT IN (SELECT point_id FROM aero.route_point  WHERE route_id IN 
	(SELECT id FROM aero.route  WHERE zone_id = OLD.zone_id)))
	AND (OLD.point_id NOT IN (SELECT point_id FROM aero.ssa_point WHERE point_id IS NOT NULL AND ssa_id IN 
	(SELECT id FROM aero.ssa  WHERE runway_id IN 
	(SELECT id FROM aero.runway  WHERE airport_id IN
	(SELECT id FROM  aero.airport  WHERE id IN 
	(SELECT airport_id FROM  aero.zone_airport  WHERE zone_id = OLD.zone_id)))))))
	
    THEN
    DELETE FROM 
        aero.zone_point 
    WHERE 
        zone_id = OLD.zone_id 
      AND 
        point_id = OLD.point_id;
    ELSE
    FOR routerecord IN SELECT * FROM aero.route as r  LEFT JOIN aero.route_point as rp ON rp.route_id = r.id  WHERE rp.point_id = OLD.point_id LOOP
    route_name = CONCAT_WS(', ',route_name,routerecord.name);
    END LOOP;
    FOR ssarecord IN SELECT * FROM aero.ssa as s  LEFT JOIN aero.ssa_point as sp ON sp.ssa_id = s.id  WHERE sp.point_id = OLD.point_id LOOP
    ssa_name = CONCAT_WS(', ',ssa_name,ssarecord.name);
    END LOOP;
	IF((route_name IS NOT NULL) OR (ssa_name IS NOT NULL))
	THEN 
	RAISE EXCEPTION 'Point not delete, because it is  %.', CONCAT('in route ', route_name,' ; ','in ssa ',ssa_name);
	END IF;   
    END IF;
    RETURN OLD;
END
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.trigger_vw_zone_point_delete_trg()
  OWNER TO aero;
