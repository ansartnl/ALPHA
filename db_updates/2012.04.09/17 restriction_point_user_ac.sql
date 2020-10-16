CREATE OR REPLACE
TRIGGER RESTRICTION_POINT_USER_AC 
      AFTER INSERT OR UPDATE OR DELETE ON RESTRICTION_POINT_USER FOR EACH ROW
DECLARE
	ac_flag NUMBER;
	
	cursorfir zone_pkg.zone_rec_type;
	curfir cursorfir%ROWTYPE;

	cursorrestriction_user zone_pkg.restriction_user_rec_type;
	currestriction_user cursorrestriction_user%ROWTYPE;

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			zone_pkg.get_cur_restriction_user(:old.restriction_id, cursorrestriction_user);
			FETCH cursorrestriction_user INTO currestriction_user;
			CLOSE cursorrestriction_user;
			zone_pkg.get_cur_zone(currestriction_user.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM RESTRICTION_POINT_USER WHERE ' ||
				'SN = ''' || :old.SN || ''' AND ' ||
				'TYPE = ''' || :old.TYPE || ''' AND ' ||
				'LATITUDE = ''' || :old.LATITUDE || ''' AND ' ||
				'LONGITUDE = ''' || :old.LONGITUDE || ''' AND ' ||
				'ARC_LATITUDE = ''' || :old.ARC_LATITUDE || ''' AND ' ||
				'ARC_LONGITUDE = ''' || :old.ARC_LONGITUDE || ''' AND ' ||
				'ARC_DISTANCE = ''' || :old.ARC_DISTANCE || ''' AND ' ||
				'RESTRICTION_ID = (SELECT ID FROM RESTRICTION_USER WHERE ' || 'NAME ' || NVL2(currestriction_user.NAME, '= ''' || currestriction_user.NAME || '''', 'IS NULL') || ' AND ' || 'TYPE = ''' || currestriction_user.TYPE || ''' AND ' || 'GUID ' || NVL2(currestriction_user.GUID, '= ''' || currestriction_user.GUID || '''', 'IS NULL') || ' AND ' || 'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')' || ')'
			);
		ELSE
			zone_pkg.get_cur_restriction_user(:new.restriction_id, cursorrestriction_user);
			FETCH cursorrestriction_user INTO currestriction_user;
			CLOSE cursorrestriction_user;
			zone_pkg.get_cur_zone(currestriction_user.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO RESTRICTION_POINT_USER(SN, TYPE, LATITUDE, LONGITUDE, ARC_LATITUDE, ARC_LONGITUDE, ARC_DISTANCE, RESTRICTION_ID) ' ||
				'VALUES(' ||
					'''' || :new.SN || ''', ' ||
					'''' || :new.TYPE || ''', ' ||
					'''' || :new.LATITUDE || ''', ' ||
					'''' || :new.LONGITUDE || ''', ' ||
					'''' || :new.ARC_LATITUDE || ''', ' ||
					'''' || :new.ARC_LONGITUDE || ''', ' ||
					'''' || :new.ARC_DISTANCE || ''', ' ||
					'(SELECT ID FROM RESTRICTION_USER WHERE ' || 'NAME ' || NVL2(currestriction_user.NAME, '= ''' || currestriction_user.NAME || '''', 'IS NULL') || ' AND ' || 'TYPE = ''' || currestriction_user.TYPE || ''' AND ' || 'GUID ' || NVL2(currestriction_user.GUID, '= ''' || currestriction_user.GUID || '''', 'IS NULL') || ' AND ' || 'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')' || ')' ||
					')');
			--UPDATE
			ELSE
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE RESTRICTION_POINT_USER SET ' ||
					'SN = ''' || :new.SN || ''', ' ||
					'TYPE = ''' || :new.TYPE || ''', ' ||
					'LATITUDE = ''' || :new.LATITUDE || ''', ' ||
					'LONGITUDE = ''' || :new.LONGITUDE || ''', ' ||
					'ARC_LATITUDE = ''' || :new.ARC_LATITUDE || ''', ' ||
					'ARC_LONGITUDE = ''' || :new.ARC_LONGITUDE || ''', ' ||
					'ARC_DISTANCE = ''' || :new.ARC_DISTANCE || ''' ' ||
				'WHERE ' ||
					'SN = ''' || :old.SN || ''' AND ' ||
					'TYPE = ''' || :old.TYPE || ''' AND ' ||
					'LATITUDE = ''' || :old.LATITUDE || ''' AND ' ||
					'LONGITUDE = ''' || :old.LONGITUDE || ''' AND ' ||
					'ARC_LATITUDE = ''' || :old.ARC_LATITUDE || ''' AND ' ||
					'ARC_LONGITUDE = ''' || :old.ARC_LONGITUDE || ''' AND ' ||
					'ARC_DISTANCE = ''' || :old.ARC_DISTANCE || ''' AND ' ||
					'RESTRICTION_ID = (SELECT ID FROM RESTRICTION_USER WHERE ' || 'NAME ' || NVL2(currestriction_user.NAME, '= ''' || currestriction_user.NAME || '''', 'IS NULL') || ' AND ' || 'TYPE = ''' || currestriction_user.TYPE || ''' AND ' || 'GUID ' || NVL2(currestriction_user.GUID, '= ''' || currestriction_user.GUID || '''', 'IS NULL') || ' AND ' || 'ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')' || ')'
				);
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER RESTRICTION_POINT_USER_AC ENABLE;
