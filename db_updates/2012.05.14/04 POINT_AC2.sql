CREATE OR REPLACE
TRIGGER POINT_AC2
  AFTER INSERT OR UPDATE OR DELETE ON POINT FOR EACH ROW
DECLARE
  ac_flag NUMBER;
  rec_exist NUMBER;

BEGIN
  ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
  IF ac_flag = 777 THEN
    --DELETE
    IF DELETING THEN
      DELETE FROM POINT_M WHERE id = :old.id;
    --INSERT or UPDATE
    ELSE
      SELECT COUNT(*) INTO rec_exist FROM POINT_M WHERE id = :new.id;
      IF rec_exist > 0 THEN
        UPDATE POINT_M SET type = :new.type, name = :new.name, latitude = :new.latitude
        , longitude = :new.longitude, icao = :new.icao, airport = :new.airport WHERE id = :new.id;
      ELSE
        INSERT INTO POINT_M VALUES (:new.id, :new.type, :new.name, :new.latitude, :new.longitude
        , :new.icao, :new.airport);
      END IF;
    END IF;
  END IF;
END;
/
ALTER TRIGGER POINT_AC2 ENABLE;
