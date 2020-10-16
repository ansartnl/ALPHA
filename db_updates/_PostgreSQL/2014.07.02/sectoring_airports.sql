CREATE TABLE sectoring_airports (
	id bigint NOT NULL,
	sector_id bigint NOT NULL,
	airports varchar(4000)
);

ALTER TABLE sectoring_airports ADD PRIMARY KEY (id);

ALTER TABLE sectoring_airports ADD CONSTRAINT sectoring_airports_sector_fk1 FOREIGN KEY (sector_id) REFERENCES sectoring(id) ON DELETE CASCADE NOT DEFERRABLE INITIALLY IMMEDIATE;

CREATE SEQUENCE sectoring_airports_seq INCREMENT 1 MINVALUE 1 NO MAXVALUE START 1 CACHE 20;

DROP TRIGGER IF EXISTS sectoring_airports_trg ON sectoring_airports CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_sectoring_airports_trg () RETURNS trigger AS $BODY$
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    IF coalesce(NEW.ID::text, '') = '' THEN
      SELECT nextval('sectoring_airports_seq') INTO NEW.ID ;
    END IF;
  END COLUMN_SEQUENCES;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER sectoring_airports_trg
	BEFORE INSERT ON sectoring_airports FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_sectoring_airports_trg();

