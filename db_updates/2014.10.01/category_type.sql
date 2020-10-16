CREATE TABLE CATEGORY_TYPE 
(
  ID NUMBER NOT NULL 
, NAME VARCHAR2(20 CHAR) NOT NULL 
, CONSTRAINT CATEGORY_TYPE_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);

DELETE FROM CATEGORY_TYPE;
INSERT INTO CATEGORY_TYPE (ID, NAME) VALUES (1, 'VT');
INSERT INTO CATEGORY_TYPE (ID, NAME) VALUES (2, 'MVL 1');
INSERT INTO CATEGORY_TYPE (ID, NAME) VALUES (3, 'MVL 2');
COMMIT;


-- Route
ALTER TABLE ROUTE 
ADD (CATEGORY_TYPE NUMBER DEFAULT 1);

UPDATE ROUTE SET CATEGORY_TYPE = 1;
COMMIT;

ALTER TABLE ROUTE
ADD CONSTRAINT ROUTE_CATEGORY_TYPE_FK1 FOREIGN KEY
(
  CATEGORY_TYPE 
)
REFERENCES CATEGORY_TYPE
(
  ID 
)
ENABLE;

ALTER TABLE ROUTE  
MODIFY (CATEGORY_TYPE NOT NULL);


CREATE OR REPLACE TRIGGER "ROUTE_AC" 
      AFTER INSERT OR UPDATE OR DELETE ON ROUTE FOR EACH ROW
DECLARE
	ac_flag NUMBER;

	cursorfir zone_pkg.zone_rec_type;
	curfir cursorfir%ROWTYPE;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			zone_pkg.get_cur_zone(:old.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM ROUTE WHERE ' ||
				'NAME = ''' || :old.NAME || ''' AND ' ||
				'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''') AND ' ||
				'CATEGORY_TYPE = ''' || :old.CATEGORY_TYPE || '''');
		ELSE
			zone_pkg.get_cur_zone(:new.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO ROUTE(NAME, ZONE_ID, CATEGORY_TYPE) ' ||
				'VALUES(' ||
					'''' || :new.NAME || ''', ' ||
					'(SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || '''), ' ||
					'''' || :new.CATEGORY_TYPE || '''' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE ROUTE SET ' ||
					'NAME = ''' || :new.NAME || ''', ' ||
					'CATEGORY_TYPE = ''' || :new.CATEGORY_TYPE || ''' ' ||
				'WHERE ' ||
					'NAME = ''' || :old.NAME || ''' AND ' ||
					'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''') AND ' ||
					'CATEGORY_TYPE = ''' || :old.CATEGORY_TYPE || '''');
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER "ROUTE_AC" ENABLE;
 


-- Point
ALTER TABLE POINT 
ADD (CATEGORY_TYPE NUMBER DEFAULT 1);

UPDATE POINT SET CATEGORY_TYPE = 1;
COMMIT;

ALTER TABLE POINT
ADD CONSTRAINT POINT_CATEGORY_TYPE_FK1 FOREIGN KEY
(
  CATEGORY_TYPE 
)
REFERENCES CATEGORY_TYPE
(
  ID 
)
ENABLE;

ALTER TABLE POINT  
MODIFY (CATEGORY_TYPE NOT NULL);


CREATE OR REPLACE FORCE VIEW POINT_VIEW ("ID", "ICAO", "TYPE", "NAME", "LATITUDE", "LONGITUDE", "AIRPORT", "CATEGORY_TYPE") AS 
	SELECT id, icao, type, name, latitude, longitude, airport, category_type 
	FROM point
	UNION 
	SELECT 0 id, '' ICAO, 100 type, '_NULL_' name , 0 latitude, 0 longitude, '' airport, 1 category_type 
	FROM dual;
 


CREATE OR REPLACE TRIGGER "POINT_AC" 
      AFTER INSERT OR UPDATE OR DELETE ON POINT FOR EACH ROW
DECLARE
	ac_flag NUMBER;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM POINT WHERE ' ||
				'TYPE = ''' || :old.TYPE || ''' AND ' ||
				'NAME = ''' || :old.NAME || ''' AND ' ||
				'LATITUDE = ''' || :old.LATITUDE || ''' AND ' ||
				'LONGITUDE = ''' || :old.LONGITUDE || ''' AND ' ||
				'ICAO ' || NVL2(:old.ICAO, '= ''' || :old.ICAO || '''', 'IS NULL') || ' AND ' ||
				'AIRPORT ' || NVL2(:old.AIRPORT, '= ''' || :old.AIRPORT || '''', 'IS NULL') || ' AND ' ||
				'CATEGORY_TYPE = ''' || :old.CATEGORY_TYPE || ''''
			);
		ELSE
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO POINT(TYPE, NAME, LATITUDE, LONGITUDE, ICAO, AIRPORT, CATEGORY_TYPE) ' ||
				'VALUES(' ||
					'''' || :new.TYPE || ''', ' ||
					'''' || :new.NAME || ''', ' ||
					'''' || :new.LATITUDE || ''', ' ||
					'''' || :new.LONGITUDE || ''', ' ||
					'''' || :new.ICAO || ''', ' ||
					'''' || :new.AIRPORT || ''', ' ||
					'''' || :new.CATEGORY_TYPE || '''' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE POINT SET ' ||
					'TYPE = ''' || :new.TYPE || ''', ' ||
					'NAME = ''' || :new.NAME || ''', ' ||
					'LATITUDE = ''' || :new.LATITUDE || ''', ' ||
					'LONGITUDE = ''' || :new.LONGITUDE || ''', ' ||
					'ICAO = ''' || :new.ICAO || ''', ' ||
					'AIRPORT = ''' || :new.AIRPORT || ''', ' ||
					'CATEGORY_TYPE = ''' || :new.CATEGORY_TYPE || ''' ' ||
				'WHERE ' ||
					'TYPE = ''' || :old.TYPE || ''' AND ' ||
					'NAME = ''' || :old.NAME || ''' AND ' ||
					'LATITUDE = ''' || :old.LATITUDE || ''' AND ' ||
					'LONGITUDE = ''' || :old.LONGITUDE || ''' AND ' ||
					'ICAO ' || NVL2(:old.ICAO, '= ''' || :old.ICAO || '''', 'IS NULL') || ' AND ' ||
					'AIRPORT ' || NVL2(:old.AIRPORT, '= ''' || :old.AIRPORT || '''', 'IS NULL') || ' AND ' ||
					'CATEGORY_TYPE = ''' || :old.CATEGORY_TYPE || ''''
				);
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER "POINT_AC" ENABLE;
 
