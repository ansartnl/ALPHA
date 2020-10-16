-- Function: aero.ssa_point_pseudo_fk_fct_point_trg()

-- DROP FUNCTION aero.ssa_point_pseudo_fk_fct_point_trg();

CREATE OR REPLACE FUNCTION aero.ssa_point_pseudo_fk_fct_point_trg()
  RETURNS trigger AS
$BODY$
BEGIN
   UPDATE aero.ssa_point SET point_id = 0 WHERE point_id = OLD.id;
   RETURN NEW;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.ssa_point_pseudo_fk_fct_point_trg()
  OWNER TO postgres;
