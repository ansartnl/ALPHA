REM INSERTING into RESTRICTION_USER_TYPE
Update RESTRICTION_USER_TYPE set NAME = 'Restricted' where ID = 82;
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (65,'Alert');
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (67,'Caution');
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (68,'Danger');
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (77,'Military');
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (80,'Prohibited');
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (84,'Training');
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (87,'Warning');
Insert into RESTRICTION_USER_TYPE (ID,NAME) values (85,'Unknown');
