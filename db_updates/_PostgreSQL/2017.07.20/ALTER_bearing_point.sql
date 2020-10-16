ALTER TABLE bearing_point DROP COLUMN fir_name;

ALTER TABLE bearing_point ADD COLUMN airport_id bigint;

COMMENT ON COLUMN bearing_point.airport_id IS 'Foreign key to AIRPORT.ID';

ALTER TABLE bearing_point ALTER COLUMN airport_id SET NOT NULL;
ALTER TABLE bearing_point
  ADD CONSTRAINT fk_bearing_point_airport_id FOREIGN KEY (airport_id)
      REFERENCES airport (id) MATCH FULL
      ON UPDATE NO ACTION ON DELETE CASCADE;

