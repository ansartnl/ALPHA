delete from ARC_AFTN;
delete from ARCHIVEINPUTMESSAGES;
delete from ARCHIVEMESSAGES;
delete from FPL_CHANGES_MASTER;
delete from HIST_FPL;
delete from HIST_MAN_AFTN;
delete from HIST_OUTPUTTELEGRAMS;
delete from HIST_RPL;
delete from INPUTMESSAGES;
delete from LOGIN_LOG;
delete from MAN_AFTN;
delete from OLDI_CLIENTS;
delete from OUTPUTMESSAGES;
delete from OUTPUTTELEGRAMS;
delete from RECEIVED_AFTN;
truncate table REPLICATION_TABLE_VALUES;
delete from REPLICATION_TABLE;
delete from REPLICATION_TABLE_LINKS;
delete from SENT_AFTN;

commit;
