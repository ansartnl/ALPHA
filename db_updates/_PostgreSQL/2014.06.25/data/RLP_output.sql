-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

SET search_path = aero, pg_catalog;
INSERT INTO rlp (id,name,latitude,longitude,angle,radius) VALUES (1,E'РЛП1 (Минск)',53.83,27.9,0,300);
INSERT INTO rlp (id,name,latitude,longitude,angle,radius) VALUES (2,E'РЛП2 (Донецк)',50.5,24.5,10,400);
INSERT INTO rlp (id,name,latitude,longitude,angle,radius) VALUES (3,E'РЛП3 (Питер)',56.4,32.4,5,200);
INSERT INTO rlp (id,name,latitude,longitude,angle,radius) VALUES (4,E'РЛП4 (Вильнюс)',55.9,21.5,30,350);
INSERT INTO rlp (id,name,latitude,longitude,angle,radius) VALUES (5,E'РЛП5 (Самара)',55.5,43,-20,300);
INSERT INTO rlp (id,name,latitude,longitude,angle,radius) VALUES (20510,E'РЛП (SAC=80 SIC=30)',53,28,0,300);