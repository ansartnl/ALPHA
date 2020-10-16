CREATE OR REPLACE
TRIGGER ZONE_BOUNDARY_AC2
  AFTER INSERT OR UPDATE OR DELETE ON ZONE_BOUNDARY FOR EACH ROW
DECLARE
  ac_flag NUMBER;
  rec_exist NUMBER;

BEGIN
  ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
  IF ac_flag = 777 THEN
    --DELETE
    IF DELETING THEN
      DELETE FROM ZONE_BOUNDARY_M WHERE id = :old.id;
    --INSERT or UPDATE
    ELSE
      SELECT COUNT(*) INTO rec_exist FROM ZONE_BOUNDARY_M WHERE id = :new.id;
      IF rec_exist > 0 THEN
        UPDATE ZONE_BOUNDARY_M SET type = :new.type, sn = :new.sn, latitude = :new.latitude
        , longitude = :new.longitude, zone_id = :new.zone_id, arc_latitude = :new.arc_latitude
        , arc_longitude = :new.arc_longitude, arc_distance = :new.arc_distance WHERE id = :new.id;
      ELSE
        INSERT INTO ZONE_BOUNDARY_M VALUES (:new.id, :new.type, :new.sn, :new.latitude, :new.longitude
        , :new.zone_id, :new.arc_latitude, :new.arc_longitude, :new.arc_distance);
      END IF;
    END IF;
  END IF;
END;
/
ALTER TRIGGER ZONE_BOUNDARY_AC2 ENABLE;
