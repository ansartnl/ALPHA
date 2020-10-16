CREATE OR REPLACE
TRIGGER HOLDING_AC2
  AFTER INSERT OR UPDATE OR DELETE ON HOLDING FOR EACH ROW
DECLARE
  ac_flag NUMBER;
  rec_exist NUMBER;

BEGIN
  ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
  IF ac_flag = 777 THEN
    --DELETE
    IF DELETING THEN
      DELETE FROM HOLDING_M WHERE id = :old.id;
    --INSERT or UPDATE
    ELSE
      SELECT COUNT(*) INTO rec_exist FROM HOLDING_M WHERE id = :new.id;
      IF rec_exist > 0 THEN
        UPDATE HOLDING_M SET point_id = :new.point_id, type = :new.type, in_course = :new.in_course
        , turn_direction = :new.turn_direction, leg_length = :new.leg_length, leg_time = :new.leg_time
        , speed = :new.speed, zone_id = :new.zone_id WHERE id = :new.id;
      ELSE
        INSERT INTO HOLDING_M VALUES (:new.id, :new.point_id, :new.type, :new.in_course
        , :new.turn_direction, :new.leg_length, :new.leg_time, :new.speed, :new.zone_id);
      END IF;
    END IF;
  END IF;
END;
/
ALTER TRIGGER HOLDING_AC2 ENABLE;
