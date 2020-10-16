ALTER TABLE aero.sectoring
  ADD COLUMN bearing_point_id bigint DEFAULT NULL;
ALTER TABLE aero.sectoring
  ADD FOREIGN KEY (bearing_point_id) REFERENCES aero.bearing_point (id) ON DELETE SET DEFAULT;

DROP VIEW aero.vw_sectoring;

CREATE OR REPLACE VIEW aero.vw_sectoring AS 
        (         SELECT sectoring.id, sectoring.name, sectoring.type_id, 
                    sectoring.bearing_point_id, 0 AS sort_factor
                   FROM aero.sectoring
        UNION 
                 SELECT 0 AS id, NULL::character varying AS name, 0 AS type_id, 
                    NULL::bigint AS bearing_point_id, 1 AS sort_factor)
UNION 
         SELECT (-1) AS id, NULL::character varying AS name, 0 AS type_id, 
            NULL::bigint AS bearing_point_id, 1 AS sort_factor
  ORDER BY 5, 2;

ALTER TABLE aero.vw_sectoring
  OWNER TO aero;

