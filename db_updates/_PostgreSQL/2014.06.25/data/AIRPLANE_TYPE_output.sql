-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

SET search_path = aero, pg_catalog;
INSERT INTO airplane_type (name,level_max,bank_max,bank_default) VALUES (E'A111',1000,1,1);
INSERT INTO airplane_type (name,level_max,bank_max,bank_default) VALUES (E'IL76',11900,30,20);
INSERT INTO airplane_type (name,level_max,bank_max,bank_default) VALUES (E'B737',12500,35,20);
INSERT INTO airplane_type (name,level_max,bank_max,bank_default) VALUES (E'T134',11000,25,20);
INSERT INTO airplane_type (name,level_max,bank_max,bank_default) VALUES (E'AN24',8000,35,20);
INSERT INTO airplane_type (name,level_max,bank_max,bank_default) VALUES (E'CRJ2',12500,45,25);
INSERT INTO airplane_type (name,level_max,bank_max,bank_default) VALUES (E'A320',13000,45,25);
INSERT INTO airplane_type (name,level_max,bank_max,bank_default) VALUES (E'T154',12500,40,20);
