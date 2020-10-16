DROP TABLE FPL_ADDITIONAL CASCADE CONSTRAINTS;

CREATE TABLE FPL_ADDITIONAL 
(
  FPL_ID_REF NUMBER NOT NULL 
, ADDITIONAL_INFO VARCHAR2(4000 CHAR) 
, CONSTRAINT FPL_ADDITIONAL_PK PRIMARY KEY 
  (
    FPL_ID_REF 
  )
  ENABLE 
);

ALTER TABLE FPL_ADDITIONAL
ADD CONSTRAINT FPL_ADDITIONAL_FPL_FK1 FOREIGN KEY
(
  FPL_ID_REF 
)
REFERENCES FPL
(
  ID 
)
ON DELETE CASCADE ENABLE;

COMMENT ON TABLE FPL_ADDITIONAL IS 'FPL additional info (field 19 and more)';

COMMENT ON COLUMN FPL_ADDITIONAL.FPL_ID_REF IS 'ID in FPL table (primary and foreign key)';

COMMENT ON COLUMN FPL_ADDITIONAL.ADDITIONAL_INFO IS 'Additional info (field 19)';



INSERT INTO fpl_additional(fpl_id_ref, additional_info)
   SELECT id, '' FROM fpl;
COMMIT;



CREATE OR REPLACE TRIGGER FPL_ADDITIONAL_TRG
      AFTER INSERT ON FPL FOR EACH ROW
	
BEGIN
	--INSERT
	IF INSERTING THEN
		INSERT INTO FPL_ADDITIONAL(FPL_ID_REF)
		VALUES(:new.ID);
	END IF;
END;
/
ALTER TRIGGER FPL_ADDITIONAL_TRG ENABLE;
 


CREATE OR REPLACE VIEW VW_FPL AS 
	SELECT fpl.*, fpl_additional.additional_info
	FROM fpl
	INNER JOIN fpl_additional ON fpl_id_ref = id;
