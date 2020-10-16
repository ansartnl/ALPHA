CREATE OR REPLACE
TRIGGER ZONE_TYPE_AC2
  AFTER INSERT OR UPDATE OR DELETE ON ZONE_TYPE FOR EACH ROW
DECLARE
  ac_flag NUMBER;
  rec_exist NUMBER;

BEGIN
  ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
  IF ac_flag = 777 THEN
    --DELETE
    IF DELETING THEN
      DELETE FROM ZONE_TYPE_M WHERE id = :old.id;
    --INSERT or UPDATE
    ELSE
      SELECT COUNT(*) INTO rec_exist FROM ZONE_TYPE_M WHERE id = :new.id;
      IF rec_exist > 0 THEN
        UPDATE ZONE_TYPE_M SET name = :new.name WHERE id = :new.id;
      ELSE
        INSERT INTO ZONE_TYPE_M VALUES (:new.id, :new.name);
      END IF;
    END IF;
  END IF;
END;
/
ALTER TRIGGER ZONE_TYPE_AC2 ENABLE;
