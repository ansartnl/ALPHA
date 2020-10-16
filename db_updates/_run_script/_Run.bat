SQLPLUS /nolog @1.sql

EXP aero/aero FILE=aero_old.dmp OWNER=aero STATISTICS=NONE

SQLPLUS /nolog @2.sql

IMP aero/aero FILE=aero.dmp FROMUSER=aero TOUSER=aero

pause