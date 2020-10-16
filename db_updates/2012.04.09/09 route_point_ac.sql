CREATE OR REPLACE
TRIGGER ROUTE_POINT_AC 
      AFTER INSERT OR UPDATE OR DELETE ON ROUTE_POINT FOR EACH ROW
DECLARE
	ac_flag NUMBER;
	
	cursorfir zone_pkg.zone_rec_type;
	curfir cursorfir%ROWTYPE;

	cursorroute zone_pkg.route_rec_type;
	curroute cursorroute%ROWTYPE;

	cursorpoint zone_pkg.point_rec_type;
	newpoint cursorpoint%ROWTYPE;		
	oldpoint cursorpoint%ROWTYPE;		

BEGIN
	ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
	IF ac_flag = 777 THEN
		--DELETE
		IF DELETING THEN
			zone_pkg.get_cur_point(:old.point_id, cursorpoint);
			FETCH cursorpoint INTO oldpoint;
			CLOSE cursorpoint;
			zone_pkg.get_cur_route(:old.route_id, cursorroute);
			FETCH cursorroute INTO curroute;
			CLOSE cursorroute;
			zone_pkg.get_cur_zone(curroute.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			INSERT INTO ARINC_CHANGES(STATEMENT)
			VALUES
			('DELETE FROM ROUTE_POINT WHERE ' ||
				'SN = ''' || :old.SN || ''' AND ' ||
				'POINT_ID = (SELECT ID FROM POINT WHERE TYPE = ''' || oldpoint.TYPE || ''' AND NAME = ''' || oldpoint.NAME || ''' AND LATITUDE = ''' || oldpoint.LATITUDE || ''' AND LONGITUDE = ''' || oldpoint.LONGITUDE || ''') AND ' ||
				'ROUTE_ID = (SELECT ID FROM ROUTE WHERE NAME = ''' || curroute.NAME || ''' AND ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')) AND ' ||
				'END_FLAG = ''' || :old.END_FLAG || '''');
		ELSE
			zone_pkg.get_cur_point(:new.point_id, cursorpoint);
			FETCH cursorpoint INTO newpoint;
			CLOSE cursorpoint;
			zone_pkg.get_cur_route(:new.route_id, cursorroute);
			FETCH cursorroute INTO curroute;
			CLOSE cursorroute;
			zone_pkg.get_cur_zone(curroute.zone_id, cursorfir);
			FETCH cursorfir INTO curfir;
			CLOSE cursorfir;
			--INSERT
			IF INSERTING THEN
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('INSERT INTO ROUTE_POINT(SN, POINT_ID, ROUTE_ID, END_FLAG) ' ||
				'VALUES(' ||
					'''' || :new.SN || ''', ' ||
					'(SELECT ID FROM POINT WHERE TYPE = ''' || newpoint.TYPE || ''' AND NAME = ''' || newpoint.NAME || ''' AND LATITUDE = ''' || newpoint.LATITUDE || ''' AND LONGITUDE = ''' || newpoint.LONGITUDE || '''), ' ||
					'(SELECT ID FROM ROUTE WHERE NAME = ''' || curroute.NAME || ''' AND ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')), ' ||
					'''' || :new.END_FLAG || '''' ||
					')');
			--UPDATE
			ELSE
				zone_pkg.get_cur_point(:old.point_id, cursorpoint);
				FETCH cursorpoint INTO oldpoint;
				CLOSE cursorpoint;
				INSERT INTO ARINC_CHANGES(STATEMENT)
				VALUES
				('UPDATE ROUTE_POINT SET ' ||
					'SN = ''' || :new.SN || ''', ' ||
					'POINT_ID = (SELECT ID FROM POINT WHERE TYPE = ''' || newpoint.TYPE || ''' AND NAME = ''' || newpoint.NAME || ''' AND LATITUDE = ''' || newpoint.LATITUDE || ''' AND LONGITUDE = ''' || newpoint.LONGITUDE || '''), ' ||
					--'ROUTE_ID = (SELECT ID FROM ROUTE WHERE NAME = ''' || curroute.NAME || ''' AND ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')), ' ||
					'END_FLAG = ''' || :new.END_FLAG || ''' ' ||
				'WHERE ' ||
					'SN = ''' || :old.SN || ''' AND ' ||
					'POINT_ID = (SELECT ID FROM POINT WHERE TYPE = ''' || oldpoint.TYPE || ''' AND NAME = ''' || oldpoint.NAME || ''' AND LATITUDE = ''' || oldpoint.LATITUDE || ''' AND LONGITUDE = ''' || oldpoint.LONGITUDE || ''') AND ' ||
					'ROUTE_ID = (SELECT ID FROM ROUTE WHERE NAME = ''' || curroute.NAME || ''' AND ZONE_ID = (SELECT ID FROM ZONE WHERE ID_NAME = ''' || curfir.ID_NAME || ''' AND TYPE = ''' || curfir.TYPE || ''' AND NAME = ''' || curfir.NAME || ''')) AND ' ||
					'END_FLAG = ''' || :old.END_FLAG || '''');
			END IF;
		END IF;
	END IF;
END;
/
ALTER TRIGGER ROUTE_POINT_AC ENABLE;
