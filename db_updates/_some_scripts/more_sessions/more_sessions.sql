connect sys/sys as sysdba;
select name, value from v$parameter where name in('processes','sessions','transactions');
alter system set processes = 150 scope = spfile;
alter system set sessions = 300 scope = spfile;
alter system set transactions = 330 scope = spfile;
shutdown immediate;
startup;
select name, value from v$parameter where name in('processes','sessions','transactions');