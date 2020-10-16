CREATE OR REPLACE
TRIGGER ROUTE_AC2
  AFTER INSERT OR UPDATE OR DELETE ON ROUTE FOR EACH ROW
DECLARE
  ac_flag NUMBER;
  rec_exist NUMBER;

BEGIN
  ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
  IF ac_flag = 777 THEN
    --DELETE
    IF DELETING THEN
      DELETE FROM ROUTE_M WHERE id = :old.id;
    --INSERT or UPDATE
    ELSE
      SELECT COUNT(*) INTO rec_exist FROM ROUTE_M WHERE id = :new.id;
      IF rec_exist > 0 THEN
        UPDATE ROUTE_M SET name = :new.name, zone_id = :new.zone_id WHERE id = :new.id;
      ELSE
        INSERT INTO ROUTE_M VALUES (:new.id, :new.name, :new.zone_id);
      END IF;
    END IF;
  END IF;
END;
/
ALTER TRIGGER ROUTE_AC2 ENABLE;
