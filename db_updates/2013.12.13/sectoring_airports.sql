CREATE TABLE SECTORING_AIRPORTS 
(
  ID NUMBER NOT NULL 
, SECTOR_ID NUMBER NOT NULL 
, AIRPORTS VARCHAR2(4000) 
, CONSTRAINT SECTORING_AIRPORTS_PK PRIMARY KEY 
  (
    ID 
  )
  ENABLE 
);

ALTER TABLE SECTORING_AIRPORTS
ADD CONSTRAINT SECTORING_AIRPORTS_SECTOR_FK1 FOREIGN KEY
(
  SECTOR_ID 
)
REFERENCES SECTORING
(
  ID 
)
ON DELETE CASCADE ENABLE;

/

CREATE SEQUENCE  SECTORING_AIRPORTS_SEQ  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
 
/

  CREATE OR REPLACE TRIGGER SECTORING_AIRPORTS_TRG BEFORE INSERT ON SECTORING_AIRPORTS 
FOR EACH ROW 
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    IF :NEW.ID IS NULL THEN
      SELECT SECTORING_AIRPORTS_SEQ.NEXTVAL INTO :NEW.ID FROM DUAL;
    END IF;
  END COLUMN_SEQUENCES;
END;
/
ALTER TRIGGER SECTORING_AIRPORTS_TRG ENABLE;
 
