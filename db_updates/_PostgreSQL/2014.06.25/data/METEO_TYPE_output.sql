-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

SET search_path = aero, pg_catalog;
INSERT INTO meteo_type (id,name,full_name) VALUES (6,E'S',E'Snow');
INSERT INTO meteo_type (id,name,full_name) VALUES (7,E'N',E'Rain');
INSERT INTO meteo_type (id,name,full_name) VALUES (8,E'F',E'Fog');
INSERT INTO meteo_type (id,name,full_name) VALUES (0,E'LVN',E'Cloudburst');
INSERT INTO meteo_type (id,name,full_name) VALUES (1,E'(R)',E'LoStorm');
INSERT INTO meteo_type (id,name,full_name) VALUES (2,E'R)',E'MedStorm');
INSERT INTO meteo_type (id,name,full_name) VALUES (3,E'R',E'HiStorm');
INSERT INTO meteo_type (id,name,full_name) VALUES (4,E'G',E'Hail');
INSERT INTO meteo_type (id,name,full_name) VALUES (5,E'H00',E'Squall');