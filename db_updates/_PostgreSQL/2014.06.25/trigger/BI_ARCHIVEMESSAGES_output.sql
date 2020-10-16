-- Generated by Ora2Pg, the Oracle database Schema converter, version 13.0
-- Copyright 2000-2014 Gilles DAROLD. All rights reserved.
-- DATASOURCE: dbi:Oracle:host=192.20.1.81;sid=XE

SET client_encoding TO 'UTF8';

SET search_path = aero, pg_catalog;\set ON_ERROR_STOP ON

DROP TRIGGER IF EXISTS bi_archivemessages ON archivemessages CASCADE;
CREATE OR REPLACE FUNCTION trigger_fct_bi_archivemessages () RETURNS trigger AS $BODY$
begin   
    SELECT "ARCHIVEMESSAGES_SEQ".nextval into NEW.MSGID ; 
end; 
$BODY$
 LANGUAGE 'plpgsql';

CREATE TRIGGER bi_archivemessages
	BEFORE INSERT ON archivemessages FOR EACH ROW
	EXECUTE PROCEDURE trigger_fct_bi_archivemessages();
