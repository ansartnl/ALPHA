-- Column: eet

-- ALTER TABLE hist_fpl DROP COLUMN eet;

ALTER TABLE hist_fpl ADD COLUMN eet time without time zone;
COMMENT ON COLUMN fpl.eet IS '??? Time of flight end';
  -------------------------------------------
  -- Trigger: hist_fpl_lt on hist_fpl

-- DROP TRIGGER hist_fpl_lt ON hist_fpl;

CREATE TRIGGER hist_fpl_lt
  AFTER INSERT OR UPDATE OR DELETE
  ON hist_fpl
  FOR EACH ROW
  EXECUTE PROCEDURE trigger_fct_hist_fpl_lt();


---------------------------------------------------
  -- Trigger: hist_fpl_id_trg on hist_fpl

-- DROP TRIGGER hist_fpl_id_trg ON hist_fpl;

CREATE TRIGGER hist_fpl_id_trg
  BEFORE INSERT
  ON hist_fpl
  FOR EACH ROW
  EXECUTE PROCEDURE trigger_fct_hist_fpl_id_trg();
--------------------------------------------

