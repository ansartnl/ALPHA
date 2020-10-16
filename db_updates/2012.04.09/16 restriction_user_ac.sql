CREATE OR REPLACE
TRIGGER RESTRICTION_USER_AC 
      AFTER INSERT OR UPDATE OR DELETE ON RESTRICTION_USER FOR EACH ROW
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
			('DELETE FROM RESTRICTION_USER WHERE ' ||
				'NAME ' || NVL2(:old.NAME, '= ''' || :old.NAME || '''', 'IS NULL') || ' AND ' ||
				'TYPE = ''' || :old.TYPE || ''' AND ' ||
				'GUID ' || NVL2(:old.GUID, '= ''' || :old.GUID || '''', 'IS NULL') || ' AND ' ||
				'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')'
			);
		ELSE
			zone_pkg.get_cur_zone(:new.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO RESTRICTION_USER(NAME, ZONE_ID, H_MIN, H_MAX, TIME_START, TIME_END, TYPE, GUID) ' ||
				'VALUES(' ||
					'''' || :new.NAME || ''', ' ||
					'(SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || '''), ' ||
					'''' || :new.H_MIN || ''', ' ||
					'''' || :new.H_MAX || ''', ' ||
					NVL2(:new.TIME_START, 'to_timestamp(''' || to_char(:new.TIME_START, 'YYYY.MM.DD HH24:MI:SS.FF') || ''', ''YYYY.MM.DD HH24:MI:SS.FF'')', '''''') || ', ' ||
					NVL2(:new.TIME_END, 'to_timestamp(''' || to_char(:new.TIME_END, 'YYYY.MM.DD HH24:MI:SS.FF') || ''', ''YYYY.MM.DD HH24:MI:SS.FF'')', '''''') || ', ' ||
					'''' || :new.TYPE || ''', ' ||
					'''' || :new.GUID || '''' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE RESTRICTION_USER SET ' ||
					'NAME = ''' || :new.NAME || ''', ' ||
					'H_MIN = ''' || :new.H_MIN || ''', ' ||
					'H_MAX = ''' || :new.H_MAX || ''', ' ||
					'TIME_START = ' || NVL2(:new.TIME_START, 'to_timestamp(''' || to_char(:new.TIME_START, 'YYYY.MM.DD HH24:MI:SS.FF') || ''', ''YYYY.MM.DD HH24:MI:SS.FF'')', '''''') || ', ' ||
					'TIME_END = ' || NVL2(:new.TIME_END, 'to_timestamp(''' || to_char(:new.TIME_END, 'YYYY.MM.DD HH24:MI:SS.FF') || ''', ''YYYY.MM.DD HH24:MI:SS.FF'')', '''''') || ', ' ||
					'TYPE = ''' || :new.TYPE || ''', ' ||
					'GUID = ''' || :new.GUID || ''' ' ||
				'WHERE ' ||
					'NAME ' || NVL2(:old.NAME, '= ''' || :old.NAME || '''', 'IS NULL') || ' AND ' ||
					'TYPE = ''' || :old.TYPE || ''' AND ' ||
					'GUID ' || NVL2(:old.GUID, '= ''' || :old.GUID || '''', 'IS NULL') || ' AND ' ||
					'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')'
				);
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER RESTRICTION_USER_AC ENABLE;
