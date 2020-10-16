CREATE OR REPLACE
TRIGGER AIRPORT_AC2
  AFTER INSERT OR UPDATE OR DELETE ON AIRPORT FOR EACH ROW
DECLARE
  ac_flag NUMBER;
  rec_exist NUMBER;

BEGIN
  ac_flag := sys_context('CLIENTCONTEXT', 'ARINC_CHANGES_FLAG');
  IF ac_flag = 777 THEN
    --DELETE
    IF DELETING THEN
      DELETE FROM AIRPORT_M WHERE id = :old.id;
    --INSERT or UPDATE
    ELSE
      SELECT COUNT(*) INTO rec_exist FROM AIRPORT_M WHERE id = :new.id;
      IF rec_exist > 0 THEN
        UPDATE AIRPORT_M SET icao = :new.icao, name = :new.name, latitude = :new.latitude
        , longitude = :new.longitude, altitude = :new.altitude WHERE id = :new.id;
      ELSE
        INSERT INTO AIRPORT_M VALUES (:new.id, :new.icao, :new.name, :new.latitude, :new.longitude
        , :new.altitude);
      END IF;
    END IF;
  END IF;
END;
/
ALTER TRIGGER AIRPORT_AC2 ENABLE;
