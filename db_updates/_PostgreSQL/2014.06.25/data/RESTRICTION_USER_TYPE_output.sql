-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

SET search_path = aero, pg_catalog;
INSERT INTO restriction_user_type (id,name) VALUES (88,E'Map');
INSERT INTO restriction_user_type (id,name) VALUES (79,E'Obstacle');
INSERT INTO restriction_user_type (id,name) VALUES (82,E'Restricted');
INSERT INTO restriction_user_type (id,name) VALUES (90,E'No Conflict Zone');
INSERT INTO restriction_user_type (id,name) VALUES (65,E'Alert');
INSERT INTO restriction_user_type (id,name) VALUES (67,E'Caution');
INSERT INTO restriction_user_type (id,name) VALUES (68,E'Danger');
INSERT INTO restriction_user_type (id,name) VALUES (77,E'Military');
INSERT INTO restriction_user_type (id,name) VALUES (80,E'Prohibited');
INSERT INTO restriction_user_type (id,name) VALUES (84,E'Training');
INSERT INTO restriction_user_type (id,name) VALUES (87,E'Warning');
INSERT INTO restriction_user_type (id,name) VALUES (85,E'Unknown');
INSERT INTO restriction_user_type (id,name) VALUES (91,E'Zone without RI');