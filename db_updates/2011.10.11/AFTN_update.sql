create or replace
PACKAGE BODY        "AFTN" is

  function is_connection_active_for_init return char as
    type log_cur_type is ref cursor;
    log_cur log_cur_type;
    result char(1);
    result2 char(1);
  begin
    select active into result2 from server_list where rownum < 2;
    if (result2 is null) then
      return 0;
    else
      return 1;
    end if;
  end;
  
  function is_connection_active return char as
    result char(1) := 0;
    result2 char(1) := 0;
  begin
      begin
        select replicate into result from login where rownum < 2;
        exception WHEN OTHERS THEN null;
      end;
      begin
        select active into result2 from server_list where rownum < 2;
        exception WHEN OTHERS THEN null;
      end;
      if result = result2 then
        return result;
      end if;
      return 0;
    
  end;
  
  function replicate(table_name varchar2) return char as
    res char(1);
  begin
    select replicate into res from sys_tables_info where tablename = UPPER(table_name);
    return res;
  end;

  function connection_address return varchar2 as
    result varchar2(100);
  begin
    select address into result from server_list where rownum < 2;
    return result;
  end;

  function connection_username return varchar2 as
    result varchar2(100);
  begin
    select username into result from server_list where rownum < 2;
    return result;
  end;

  function connection_password return varchar2 as
    result varchar2(100);
  begin
    select password into result from server_list where rownum < 2;
    return result;
  end;

  -- Return UTC time (used for trtime)
  function get_utc_time return varchar2 as
  utc_time timestamp;
  begin
    select sys_extract_utc(current_timestamp) into utc_time from dual;
    return to_char(utc_time,'yyyymmddhh24miss');
  end;

  -- Exec SQL command
  procedure run_cmd(cmd varchar2) as
  pragma autonomous_transaction;
  begin
    -- out cmd
--    dbms_output.put_line('cmd_in = '|| cmd);
    --------------
    execute immediate cmd; 
    commit;
    EXCEPTION WHEN OTHERS THEN begin
      dbms_output.put_line('Error(''' || cmd || '''): ' || dbms_utility.format_error_stack);
      rollback;
      end;
  end;

  procedure create_db_link(host varchar2,login varchar2,pwd varchar2) as
    cmd     varchar2(512);
    already_exists int;
  begin
    select count(*) into already_exists from all_db_links where db_link = 'REPLICATION';
    if already_exists = 1 then
      run_cmd('drop public database link replication');
    end if;
    dbms_output.put_line('drop public database link replication');
    if is_connection_active() = 1 then
      cmd := 'CREATE PUBLIC DATABASE LINK replication CONNECT TO ' || login
              || ' IDENTIFIED BY ' || pwd
              || ' USING  ''(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(COMMUNITY=TCP)(PROTOCOL=TCP)(Host='
              || host || ')(Port=1521)))(CONNECT_DATA=(SID=XE)))''';
      run_cmd(cmd);
      dbms_output.put_line('create_db_link: ' || cmd);
    end if;
  end;
  
    procedure create_db_link_for_init(host varchar2,login varchar2,pwd varchar2) as
    cmd     varchar2(512);
    already_exists int;
  begin
    select count(*) into already_exists from all_db_links where db_link = 'REPLICATION';
    if already_exists = 1 then
      run_cmd('drop public database link replication');
    end if;
    dbms_output.put_line('drop public database link replication');
    if is_connection_active_for_init() = 1 then
      cmd := 'CREATE PUBLIC DATABASE LINK replication CONNECT TO ' || login
              || ' IDENTIFIED BY ' || pwd
              || ' USING  ''(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(COMMUNITY=TCP)(PROTOCOL=TCP)(Host='
              || host || ')(Port=1521)))(CONNECT_DATA=(SID=XE)))''';
      run_cmd(cmd);
      dbms_output.put_line('create_db_link: ' || cmd);
    end if;
  end;

  procedure check_and_copy as
    type back_tables_type is ref cursor;
    
    back_tables back_tables_type;
    tablename varchar(200);
    sequencename varchar(200);
    curval numeric;
    
  begin
    open back_tables for 'select tbl1.tablename, tbl1.sequence from sys_tables_info@replication tbl1 join sys_tables_info tbl2
      on tbl1.tablename = tbl2.tablename
        and tbl2.replicate = 1
        and tbl2.lasttouch < tbl1.lasttouch';
    
    loop     
      fetch back_tables into tablename, sequencename;
      exit when back_tables%NOTFOUND;
      aftn.run_cmd('delete from ' || tablename);
      aftn.run_cmd('insert into ' || tablename || ' select * from ' || tablename || '@replication');
      if sequencename is not null then
        execute immediate 'select last_number from user_sequences@replication where sequence_name = ''' || UPPER(sequencename) ||'''' into curval;
        reset_seq(UPPER(sequencename), curval);
      end if;
    end loop;
    close back_tables;
  end;

  procedure init as
  res integer;
  begin
    select active into res from server_list where rownum < 2;
    if res = 1 then
      create_db_link_fcataor_init(connection_address, connection_username, connection_password);
      check_and_copy();
    end if;
  end;

  procedure reset_seq(p_seq_name in varchar2, newvalue in integer)
  as
    l_val number;
  begin
--    dbms_output.put_line('reser_seq');
    execute immediate
    'select ' || p_seq_name || '.nextval from dual' INTO l_val;

    execute immediate
    'alter sequence ' || p_seq_name || ' increment by -' || l_val ||
                                                          ' minvalue 0';
    execute immediate
    'select ' || p_seq_name || '.nextval from dual' INTO l_val;

    l_val := newvalue - 1;
    execute immediate
    'alter sequence ' || p_seq_name || ' increment by ' || l_val ||
                                                          ' minvalue 0';
    execute immediate
    'select ' || p_seq_name || '.nextval from dual' INTO l_val;

    execute immediate
    'alter sequence ' || p_seq_name || ' increment by 1 minvalue 0';
  end;

  --declare
--cursor cv is select table_name from user_tables;
--table_name varchar2(100);
--begin
--open cv;
--fetch cv into table_name;
--while cv%FOUND loop
--  aftn.run_cmd('create or replace
    --TRIGGER '||table_name||'_LT
    --AFTER INSERT OR UPDATE OR DELETE ON '|| table_name ||'
    --FOR EACH ROW
    --BEGIN
    ---- Touch the row
--      update sys_tables_info set lasttouch = SYS_EXTRACT_UTC(SYSTIMESTAMP) where tablename =''' || table_name || ''';
    --END;');
  --fetch cv into table_name;
--end loop;
--close cv;
--end;

end; -- Aftn package bodyA