-- Выполнение запроса:
SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;

-- Выполнение запроса:
CREATE TABLE svg_items (
	id bigint NOT NULL,
	latitude_start double precision DEFAULT 0,
	longitude_start double precision DEFAULT 0,
	latitude_finish double precision DEFAULT 0,
	longitude_finish double precision DEFAULT 0,
	filename character varying(1000)
);

-- Выполнение запроса:
ALTER TABLE svg_items ADD PRIMARY KEY (id);

-- Выполнение запроса:
CREATE SEQUENCE svg_items_seq INCREMENT 1 MINVALUE 1 NO MAXVALUE START 1 CACHE 20;

-- Выполнение запроса:
DROP TRIGGER IF EXISTS svg_items_trg ON svg_items CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_svg_items_trg () RETURNS trigger AS $BODY$
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    IF coalesce(NEW.ID::text, '') = '' THEN
      SELECT nextval('svg_items_seq') INTO NEW.ID ;
    END IF;
  END COLUMN_SEQUENCES;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER svg_items_trg
	BEFORE INSERT ON svg_items FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_svg_items_trg();
