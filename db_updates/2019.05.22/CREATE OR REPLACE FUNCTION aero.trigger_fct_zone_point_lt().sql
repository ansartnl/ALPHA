-- Function: aero.trigger_fct_zone_point_lt()

-- DROP FUNCTION aero.trigger_fct_zone_point_lt();

CREATE OR REPLACE FUNCTION aero.trigger_fct_zone_point_lt()
  RETURNS trigger AS
$BODY$
BEGIN
    -- Touch the row
      update aero.sys_tables_info set lasttouch = now() AT TIME ZONE 'UTC' where tablename ='ZONE_POINT';
    RETURN NEW;
END
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.trigger_fct_zone_point_lt()
  OWNER TO aero;
