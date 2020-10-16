-- Table: aero.orcam_range

-- DROP TABLE aero.orcam_range;

CREATE TABLE aero.orcam_range
(
  id bigint NOT NULL, -- Primary key
  zone_id bigint NOT NULL,
  orcam_min_code character varying(4) NOT NULL, 
  orcam_max_code character varying(4) NOT NULL,
  sectortype_id bigint, 
  CONSTRAINT orcam_range_pkey PRIMARY KEY (id),
  CONSTRAINT fk_orcam_range_zone_id FOREIGN KEY (zone_id)
	REFERENCES aero.zone (id) MATCH FULL
	ON UPDATE NO ACTION ON DELETE CASCADE
)
WITH (
  OIDS=FALSE
);
ALTER TABLE aero.orcam_range
  OWNER TO aero;