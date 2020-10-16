--------------------------------------------------------
--  File created - четверг-Ноябрь-17-2011   
--------------------------------------------------------

ALTER TABLE RESTRICTION_USER_TYPE  
MODIFY (NAME VARCHAR2(21 BYTE) );

REM INSERTING into RESTRICTION_USER_TYPE
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (90,'No Conflict Zone');
