-- Sequence: aero.sectoring_bearing_seq

-- DROP SEQUENCE aero.sectoring_bearing_seq;

CREATE SEQUENCE aero.sectoring_bearing_seq
  INCREMENT 1
  MINVALUE 1
  MAXVALUE 9223372036854775807
  START 43741
  CACHE 20;
ALTER TABLE aero.sectoring_bearing_seq
  OWNER TO aero;

﻿-- Table: aero.sectoring_bearing

-- DROP TABLE aero.sectoring_bearing;

CREATE TABLE aero.sectoring_bearing
(
  id bigint NOT NULL,
  sect_id bigint NOT NULL,
  bearing_id bigint NOT NULL,
  CONSTRAINT sectoring_bearing_pkey PRIMARY KEY(id),
  CONSTRAINT sectoring_bearing_sectoring_fk1 FOREIGN KEY (sect_id)
      REFERENCES aero.sectoring (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE CASCADE,
  CONSTRAINT sectoring_bearing_sect_id_bearing_id_key UNIQUE (sect_id, bearing_id)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE aero.sectoring_bearing
  OWNER TO aero;

-- Trigger: sectoring_bearing_id_trg on aero.sectoring_bearing

-- DROP TRIGGER sectoring_bearing_id_trg ON aero.sectoring_bearing;

-- Function: aero.trigger_fct_sectoring_zone_id_trg()

-- DROP FUNCTION aero.trigger_fct_sectoring_zone_id_trg();

CREATE OR REPLACE FUNCTION aero.trigger_fct_sectoring_bearing_id_trg()
  RETURNS trigger AS
$BODY$
BEGIN
  if coalesce(NEW.id::text, '') = '' then
    SELECT  nextval('sectoring_bearing_seq') INTO NEW.id ;
  end if;
RETURN NEW;
END
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.trigger_fct_sectoring_bearing_id_trg()
  OWNER TO aero;

-- Function: aero.trigger_fct_sectoring_bearing_lt()

-- DROP FUNCTION aero.trigger_fct_sectoring_bearing_lt();

CREATE OR REPLACE FUNCTION aero.trigger_fct_sectoring_bearing_lt()
  RETURNS trigger AS
$BODY$
BEGIN
    -- Touch the row
      update sys_tables_info set lasttouch = now() AT TIME ZONE 'UTC' where tablename ='SECTORING_BEARING';
    RETURN NEW;
END
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION aero.trigger_fct_sectoring_bearing_lt()
  OWNER TO aero;


CREATE TRIGGER sectoring_bearing_id_trg
  BEFORE INSERT
  ON aero.sectoring_bearing
  FOR EACH ROW
  EXECUTE PROCEDURE aero.trigger_fct_sectoring_bearing_id_trg();

-- Trigger: sectoring_bearing_lt on aero.sectoring_bearing

-- DROP TRIGGER sectoring_bearing_lt ON aero.sectoring_bearing;

CREATE TRIGGER sectoring_bearing_lt
  AFTER INSERT OR UPDATE OR DELETE
  ON aero.sectoring_bearing
  FOR EACH ROW
  EXECUTE PROCEDURE aero.trigger_fct_sectoring_bearing_lt();

