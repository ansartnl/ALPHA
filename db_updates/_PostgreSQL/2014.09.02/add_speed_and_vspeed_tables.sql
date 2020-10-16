CREATE TABLE speed_in_kmph
(
  id integer NOT NULL,
  value real NOT NULL,
  CONSTRAINT speed_in_kmph_pkey PRIMARY KEY (id)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE speed_in_kmph
  OWNER TO aero;
  
CREATE TABLE speed_in_knot
(
  id integer NOT NULL,
  value real NOT NULL,
  CONSTRAINT speed_in_knot_pkey PRIMARY KEY (id)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE speed_in_knot
  OWNER TO aero;
  
CREATE TABLE speed_in_mach
(
  id integer NOT NULL,
  value real NOT NULL,
  CONSTRAINT speed_in_mach_pkey PRIMARY KEY (id)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE speed_in_mach
  OWNER TO aero;

CREATE TABLE vspeed_in_feet
(
  id integer NOT NULL,
  value real NOT NULL,
  CONSTRAINT vspeed_in_feet_pkey PRIMARY KEY (id)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE vspeed_in_feet
  OWNER TO aero;
  
CREATE TABLE vspeed_in_meter
(
  id integer NOT NULL,
  value real NOT NULL,
  CONSTRAINT vspeed_in_meter_pkey PRIMARY KEY (id)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE vspeed_in_meter
  OWNER TO aero;