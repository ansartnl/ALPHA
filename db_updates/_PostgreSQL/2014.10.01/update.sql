CREATE OR REPLACE VIEW aero.vw_sectoring AS  (
SELECT sectoring.id, sectoring.name, sectoring.type_id, 0 AS sort_factor
FROM aero.sectoring
UNION 
SELECT 0 AS id, NULL::character varying AS name, 0 AS type_id, 1 AS sort_factor
UNION 
SELECT (-1) AS id, NULL::character varying AS name, 0 AS type_id, 1 AS sort_factor
ORDER BY sort_factor, name
);

ALTER TABLE aero.vw_sectoring
  OWNER TO aero;
