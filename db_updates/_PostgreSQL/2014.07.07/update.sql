ALTER TABLE zone_point ADD COLUMN is_fir_border boolean NOT NULL DEFAULT FALSE;
ALTER TABLE zone_point ADD COLUMN is_sector_border boolean NOT NULL DEFAULT FALSE;

-- CREATE VIEW
CREATE OR REPLACE VIEW aero.vw_zone_point AS
SELECT 
    zone_point.id, zone_point.point_id, zone_point.zone_id, 
    point_type.name AS type, point.name, point.latitude, 
    point.longitude, point.icao, point.airport, 
    zone_point.is_fir_border, zone_point.is_sector_border 
FROM 
    zone_point 
LEFT JOIN 
    point 
ON 
    point.id = zone_point.point_id 
LEFT JOIN 
    point_type 
ON 
    point.type = point_type.id;
ALTER TABLE aero.vw_zone_point
  OWNER TO aero;
GRANT SELECT, UPDATE, INSERT, DELETE, REFERENCES, TRIGGER ON TABLE aero.vw_zone_point TO aero;

-- CREATE TRIGGER FUNCTIONS
CREATE FUNCTION trigger_vw_zone_point_insert_trg() RETURNS trigger AS
$BODY$
BEGIN
    INSERT INTO 
        zone_point 
    VALUES 
    (
        default, NEW.zone_id, NEW.point_id, 
        NEW.is_fir_border, NEW.is_sector_border 
    );
    RETURN NEW;
END
$BODY$
LANGUAGE plpgsql VOLATILE NOT LEAKPROOF;
ALTER FUNCTION aero.trigger_vw_zone_point_insert_trg()
  OWNER TO aero;

CREATE FUNCTION trigger_vw_zone_point_update_trg() RETURNS trigger AS
$BODY$
BEGIN
    UPDATE 
        zone_point 
    SET 
        is_fir_border = NEW.is_fir_border, 
        is_sector_border = NEW.is_sector_border
    WHERE 
        zone_id = NEW.zone_id 
      AND 
        point_id = NEW.point_id;
    RETURN NEW;
END
$BODY$
LANGUAGE plpgsql VOLATILE NOT LEAKPROOF;
ALTER FUNCTION aero.trigger_vw_zone_point_update_trg()
  OWNER TO aero;

CREATE FUNCTION trigger_vw_zone_point_delete_trg() RETURNS trigger AS
$BODY$
BEGIN
    DELETE FROM 
        zone_point 
    WHERE 
        zone_id = OLD.zone_id 
      AND 
        point_id = OLD.point_id;
    RETURN OLD;
END
$BODY$
LANGUAGE plpgsql VOLATILE NOT LEAKPROOF;
ALTER FUNCTION aero.trigger_vw_zone_point_delete_trg()
  OWNER TO aero;

-- CREATE TRIGGERS INSTEAD OF INSERT/UPDATE/DELETE
CREATE TRIGGER vw_zone_point_insert_trg INSTEAD OF INSERT
   ON vw_zone_point FOR EACH ROW
   EXECUTE PROCEDURE aero.trigger_vw_zone_point_insert_trg();

CREATE TRIGGER vw_zone_point_update_trg INSTEAD OF UPDATE
   ON vw_zone_point FOR EACH ROW
   EXECUTE PROCEDURE aero.trigger_vw_zone_point_update_trg();

CREATE TRIGGER vw_zone_point_delete_trg INSTEAD OF DELETE
   ON vw_zone_point FOR EACH ROW
   EXECUTE PROCEDURE aero.trigger_vw_zone_point_delete_trg();
