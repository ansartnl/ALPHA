CREATE OR REPLACE
TRIGGER ROUTE_POINT_AC2
  AFTER INSERT OR UPDATE OR DELETE ON ROUTE_POINT FOR EACH ROW
DECLARE
  ac_flag NUMBER;
  rec_exist NUMBER;

BEGIN
  ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
  IF ac_flag = 777 THEN
    --DELETE
    IF DELETING THEN
      DELETE FROM ROUTE_POINT_M WHERE id = :old.id;
    --INSERT or UPDATE
    ELSE
      SELECT COUNT(*) INTO rec_exist FROM ROUTE_POINT_M WHERE id = :new.id;
      IF rec_exist > 0 THEN
        UPDATE ROUTE_POINT_M SET sn = :new.sn, point_id = :new.point_id, route_id = :new.route_id, end_flag = :new.end_flag
        WHERE id = :new.id;
      ELSE
        INSERT INTO ROUTE_POINT_M VALUES (:new.id, :new.sn, :new.point_id, :new.route_id, :new.end_flag);
      END IF;
    END IF;
  END IF;
END;
/
ALTER TRIGGER ROUTE_POINT_AC2 ENABLE;
