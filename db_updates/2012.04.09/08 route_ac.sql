CREATE OR REPLACE
TRIGGER ROUTE_AC 
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
				'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')');
		ELSE
			zone_pkg.get_cur_zone(:new.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO ROUTE(NAME, ZONE_ID) ' ||
				'VALUES(' ||
					'''' || :new.NAME || ''', ' ||
					'(SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE ROUTE SET ' ||
					'NAME = ''' || :new.NAME || ''' ' ||
				'WHERE ' ||
					'NAME = ''' || :old.NAME || ''' AND ' ||
					'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')');
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER ROUTE_AC ENABLE;
