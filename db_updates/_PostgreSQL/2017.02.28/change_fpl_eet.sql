-- Column: eet

-- ALTER TABLE fpl DROP COLUMN eet;

--ALTER TABLE fpl ADD COLUMN eet character varying(5);
--COMMENT ON COLUMN fpl.eet IS '??? Time of flight end';

ALTER TABLE fpl ADD COLUMN eet time without time zone;
COMMENT ON COLUMN fpl.eet IS '??? Time of flight end';
--ALTER TABLE fpl DROP COLUMN eet;

----------------------------------------------------------------
-- View: vw_fpl

-- DROP VIEW vw_fpl;

CREATE OR REPLACE VIEW vw_fpl AS 
 SELECT fpl.id,
    fpl.fir_name,
    fpl.aircraft_id,
    fpl.taircraft,
    fpl.frules,
    fpl.tflight,
    fpl.num,
    fpl.wtc,
    fpl.equipment,
    fpl.adep,
    fpl."time",
    fpl.speed,
    fpl.flevel,
    fpl.route,
    fpl.ades,
    fpl.eet,
    fpl.alt1,
    fpl.alt2,
    fpl.other,
    fpl.trtime,
    fpl.exfpl,
    fpl.visibility,
    fpl.message_id,
    fpl.entry_fir_eet,
    fpl_additional.additional_info
   FROM fpl
   JOIN fpl_additional ON fpl_additional.fpl_id_ref = fpl.id;

ALTER TABLE vw_fpl
  OWNER TO aero;


-- Trigger: vw_fpl_delete_trg on vw_fpl

-- DROP TRIGGER vw_fpl_delete_trg ON vw_fpl;

CREATE TRIGGER vw_fpl_delete_trg
  INSTEAD OF DELETE
  ON vw_fpl
  FOR EACH ROW
  EXECUTE PROCEDURE trigger_fct_vw_fpl_delete();

-- Trigger: vw_fpl_insert_trg on vw_fpl

-- DROP TRIGGER vw_fpl_insert_trg ON vw_fpl;

CREATE TRIGGER vw_fpl_insert_trg
  INSTEAD OF INSERT
  ON vw_fpl
  FOR EACH ROW
  EXECUTE PROCEDURE trigger_fct_vw_fpl_insert();

-- Trigger: vw_fpl_update_trg on vw_fpl

-- DROP TRIGGER vw_fpl_update_trg ON vw_fpl;

CREATE TRIGGER vw_fpl_update_trg
  INSTEAD OF UPDATE
  ON vw_fpl
  FOR EACH ROW
  EXECUTE PROCEDURE trigger_fct_vw_fpl_update();
--------------------------------------------------------------------------
-- Rule: "_RETURN" ON vw_fpl

-- DROP RULE "_RETURN" ON vw_fpl;

CREATE OR REPLACE RULE "_RETURN" AS
    ON SELECT TO vw_fpl DO INSTEAD  SELECT fpl.id,
    fpl.fir_name,
    fpl.aircraft_id,
    fpl.taircraft,
    fpl.frules,
    fpl.tflight,
    fpl.num,
    fpl.wtc,
    fpl.equipment,
    fpl.adep,
    fpl."time",
    fpl.speed,
    fpl.flevel,
    fpl.route,
    fpl.ades,
    fpl.eet,
    fpl.alt1,
    fpl.alt2,
    fpl.other,
    fpl.trtime,
    fpl.exfpl,
    fpl.visibility,
    fpl.message_id,
    fpl.entry_fir_eet,
    fpl_additional.additional_info
   FROM fpl
   JOIN fpl_additional ON fpl_additional.fpl_id_ref = fpl.id;
-------------------------------------------------------------
-------------------------------------------------------------
-- View: vw_fpl_with_null

-- DROP VIEW vw_fpl_with_null;

CREATE OR REPLACE VIEW vw_fpl_with_null AS 
         SELECT fpl.id,
            fpl.aircraft_id
           FROM fpl
UNION
         SELECT NULL::bigint AS id,
            '(empty)'::character varying AS aircraft_id;

ALTER TABLE vw_fpl_with_null
  OWNER TO aero;
-----------------------------------------
-- Rule: "_RETURN" ON vw_fpl_with_null

-- DROP RULE "_RETURN" ON vw_fpl_with_null;

CREATE OR REPLACE RULE "_RETURN" AS
    ON SELECT TO vw_fpl_with_null DO INSTEAD          SELECT fpl.id,
            fpl.aircraft_id
           FROM fpl
UNION
         SELECT NULL::bigint AS id,
            '(empty)'::character varying AS aircraft_id;

