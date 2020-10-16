CREATE OR REPLACE
TRIGGER ZONE_AC2
  AFTER INSERT OR UPDATE OR DELETE ON ZONE FOR EACH ROW
DECLARE
  ac_flag NUMBER;
  rec_exist NUMBER;

BEGIN
  ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
  IF ac_flag = 777 THEN
    --DELETE
    IF DELETING THEN
      DELETE FROM ZONE_M WHERE id = :old.id;
    --INSERT or UPDATE
    ELSE
      SELECT COUNT(*) INTO rec_exist FROM ZONE_M WHERE id = :new.id;
      IF rec_exist > 0 THEN
        UPDATE ZONE_M SET name = :new.name, type = :new.type, id_name = :new.id_name
        , parent_id = :new.parent_id, low_limit = :new.low_limit, up_limit = :new.up_limit
        , short_name = :new.short_name, label_name = :new.label_name, frequency = :new.frequency
        WHERE id = :new.id;
      ELSE
        INSERT INTO ZONE_M VALUES (:new.id, :new.name, :new.type, :new.id_name, :new.parent_id
        , :new.low_limit, :new.up_limit, :new.short_name, :new.label_name, :new.frequency);
      END IF;
    END IF;
  END IF;
END;
/
ALTER TRIGGER ZONE_AC2 ENABLE;
