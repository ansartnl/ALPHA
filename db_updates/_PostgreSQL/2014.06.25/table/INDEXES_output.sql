-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

\set ON_ERROR_STOP ON

SET search_path = aero, pg_catalog;


CREATE INDEX fk_restriction_zone_id ON restriction (zone_id);



CREATE INDEX fk_wind_zone_id ON wind (zone_id);










CREATE INDEX fk_routes_zoneid ON route (zone_id);


CREATE INDEX fk_routepoints_point_id ON route_point (point_id);
CREATE INDEX fk_routepoints_routid ON route_point (route_id);

CREATE INDEX fk_fpl_master_taircraft ON fpl_master (taircraft);



CREATE INDEX fk_sectorpoints_zone_id ON zone_point (zone_id);
CREATE INDEX fk_sectorpoints_point_id ON zone_point (point_id);








CREATE INDEX index_uniq_1 ON zone (name,id_name);
CREATE INDEX fk_zoneairports_zone_id ON zone_airport (zone_id);
CREATE INDEX fk_zoneairports_airport_id ON zone_airport (airport_id);







CREATE INDEX fk_fpl_taircraft ON fpl (taircraft);

CREATE INDEX fk_runways_airport_id ON runway (airport_id);

CREATE INDEX fk_ffpl_taircraft ON ffpl (taircraft);
CREATE INDEX index_runway_id ON meteo_runway (runway_id);












CREATE INDEX fk_exercise_plane_id ON exercise_plane (id_plane);

CREATE UNIQUE INDEX restriction_user_sync_index1 ON restriction_user_sync (id);










CREATE INDEX fk_meteo_zone_id ON meteo (zone_id);









CREATE INDEX fk_ssapoints_point_id ON ssa_point (point_id);
CREATE INDEX fk_ssapoints_ssa_id ON ssa_point (ssa_id);







CREATE INDEX fk_holding_point_id ON holding (point_id);
CREATE INDEX fk_holding_zone_id ON holding (zone_id);

CREATE INDEX fk_ssa_runway_id ON ssa (runway_id);

CREATE INDEX fk_restriction_point_restricti ON restriction_point (restriction_id);












CREATE INDEX fk_meteo_point_meteo_id ON meteo_point (meteo_id);















CREATE INDEX index_unique ON airport (icao,name);
CREATE INDEX restriction_point_user_index1 ON restriction_point_user (restriction_id);
CREATE UNIQUE INDEX modification_types_pk ON modification_types (type);
