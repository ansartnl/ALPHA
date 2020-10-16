SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;

-- USER_PICS
CREATE TABLE user_pics (
	id bigint NOT NULL,
	zone_id bigint NOT NULL,
	name character varying(1000)
);

ALTER TABLE user_pics ADD PRIMARY KEY (id);

CREATE SEQUENCE user_pics_seq INCREMENT 1 MINVALUE 1 NO MAXVALUE START 1 CACHE 20;

DROP TRIGGER IF EXISTS user_pics_trg ON user_pics CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_user_pics_trg () RETURNS trigger AS $BODY$
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    IF coalesce(NEW.ID::text, '') = '' THEN
      SELECT nextval('user_pics_seq') INTO NEW.ID ;
    END IF;
  END COLUMN_SEQUENCES;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER user_pics_trg
	BEFORE INSERT ON user_pics FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_user_pics_trg();

CREATE INDEX fk_user_pics_zone_id ON user_pics (zone_id);
ALTER TABLE user_pics ADD CONSTRAINT fk_user_pics_zone_id FOREIGN KEY (zone_id) REFERENCES zone(id) ON DELETE CASCADE NOT DEFERRABLE INITIALLY IMMEDIATE;

-- USER_PICS_POINT
CREATE TABLE user_pics_point (
	id bigint NOT NULL,
	sn bigint NOT NULL,
	type bigint NOT NULL,
	latitude double precision NOT NULL,
	longitude double precision NOT NULL,
	arc_latitude double precision NOT NULL,
	arc_longitude double precision NOT NULL,
	arc_distance double precision NOT NULL,
	user_pics_id bigint NOT NULL
);

ALTER TABLE user_pics_point ADD PRIMARY KEY (id);

CREATE SEQUENCE user_pics_point_seq INCREMENT 1 MINVALUE 1 NO MAXVALUE START 1 CACHE 20;

DROP TRIGGER IF EXISTS user_pics_point_trg ON user_pics_point CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_user_pics_point_trg () RETURNS trigger AS $BODY$
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    IF coalesce(NEW.ID::text, '') = '' THEN
      SELECT nextval('user_pics_point_seq') INTO NEW.ID ;
    END IF;
  END COLUMN_SEQUENCES;
RETURN NEW;
END
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER user_pics_point_trg
	BEFORE INSERT ON user_pics_point FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_user_pics_point_trg();

CREATE INDEX fk_user_pics_point_user_pics_i ON user_pics_point (user_pics_id);
ALTER TABLE user_pics_point ADD CONSTRAINT fk_user_pics_point_user_pics_i FOREIGN KEY (user_pics_id) REFERENCES user_pics(id) ON DELETE CASCADE NOT DEFERRABLE INITIALLY IMMEDIATE;
ALTER TABLE user_pics_point ADD CONSTRAINT fk_user_pics_point_type_id FOREIGN KEY (type) REFERENCES boundary_point_type(id) ON DELETE NO ACTION NOT DEFERRABLE INITIALLY IMMEDIATE;