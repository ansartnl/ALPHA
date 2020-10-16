CREATE OR REPLACE
TRIGGER ZONE_POINT_AC2
  AFTER INSERT OR UPDATE OR DELETE ON ZONE_POINT FOR EACH ROW
DECLARE
  ac_flag NUMBER;
  rec_exist NUMBER;

BEGIN
  ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
  IF ac_flag = 777 THEN
    --DELETE
    IF DELETING THEN
      DELETE FROM ZONE_POINT_M WHERE id = :old.id;
    --INSERT or UPDATE
    ELSE
      SELECT COUNT(*) INTO rec_exist FROM ZONE_POINT_M WHERE id = :new.id;
      IF rec_exist > 0 THEN
        UPDATE ZONE_POINT_M SET zone_id = :new.zone_id, point_id = :new.point_id
        WHERE id = :new.id;
      ELSE
        INSERT INTO ZONE_POINT_M VALUES (:new.id, :new.zone_id, :new.point_id);
      END IF;
    END IF;
  END IF;
END;
/
ALTER TRIGGER ZONE_POINT_AC2 ENABLE;
