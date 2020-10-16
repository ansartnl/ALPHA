
  CREATE OR REPLACE TRIGGER "OUTPUTTELEGRAMS_AFTER_QUERY_TR" 
AFTER INSERT OR UPDATE OR DELETE ON "OUTPUTTELEGRAMS"
FOR EACH ROW
DECLARE
	modification_type	CHAR;
	current_user_id		NUMBER;
BEGIN
  BEGIN
    SELECT user_id INTO current_user_id FROM login WHERE ROWNUM <= 1;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      current_user_id := -1;
  END;
  
  IF current_user_id <> -1 THEN
    IF DELETING THEN
    INSERT INTO hist_outputtelegrams
      (id, created, header, text, addresses, modified_by, modification_type)
    VALUES
      (:OLD.id, :OLD.created, :OLD.header, :OLD.text, :OLD.addresses, current_user_id, 'D');
    ELSE
      IF INSERTING THEN
        modification_type := 'I';
      --ELSE
        --modification_type := 'U';
      --END IF;
      INSERT INTO hist_outputtelegrams
        (id, created, header, text, addresses, modified_by, modification_type)
      VALUES
        (:NEW.id, :NEW.created, :NEW.header, :NEW.text, :NEW.addresses, current_user_id, modification_type);
      END IF;--
    END IF;
  END IF;
END;
/
ALTER TRIGGER "OUTPUTTELEGRAMS_AFTER_QUERY_TR" ENABLE;
 
