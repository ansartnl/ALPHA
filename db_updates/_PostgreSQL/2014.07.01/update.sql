CREATE TABLE last_sync
(
  id serial NOT NULL,
  "time" timestamp without time zone,
  CONSTRAINT last_sync_pkey PRIMARY KEY (id)
)
WITH (
  OIDS=FALSE
);

ALTER TABLE last_sync OWNER TO aero;
GRANT ALL ON TABLE last_sync TO aero;
