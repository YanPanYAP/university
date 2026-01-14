ALTER SESSION SET CONTAINER = CDB$ROOT;

show user;
show con_name;

//1
select sum(value) as total_sga_bytes from v$sga;

// 2
select pool, name, sum(bytes)/1024/1024 as size_mb from v$sgastat
    where pool is not null
    group by pool, name;
    
//3
select name, bytes
    from v$sgainfo
    where name = 'Granule Size';
    
// 4
select sum(bytes)/1024/1024 as free_memory_mb
    from v$sgastat
    where name = 'free memory';
    
// 5
select name, value/1024/1024 as size_mb
    from v$parameter
    where name in ('sga_target', 'sga_max_size');
    
// 6
select name, block_size, buffers, (block_size*buffers)/1024/1024 as size_mb from v$buffer_pool;

alter system set db_keep_cache_size = 50M;
alter system set db_recycle_cache_size = 20M;

// 7
CREATE PLUGGABLE DATABASE FREEPDB1
  ADMIN USER pdbadmin IDENTIFIED BY StrongPass123
  ROLES = (DBA)
  DEFAULT TABLESPACE users
  DATAFILE '/opt/oracle/oradata/ORCLCDB/YAP_PDB/users01.dbf' SIZE 250M AUTOEXTEND ON
  FILE_NAME_CONVERT = ('pdbseed', 'FREEPDB1');
  ALTER PLUGGABLE DATABASE FREEPDB1 OPEN;

  
ALTER SESSION SET CONTAINER = FREEPDB1;

CREATE TABLE KEEP_TABLE (
  id NUMBER,
  data VARCHAR2(100)
)
STORAGE (BUFFER_POOL KEEP);

select segment_name, buffer_pool from dba_segments where segment_name = 'KEEP_TABLE';

// 8
CREATE TABLE DEFAULT_TABLE (
  id NUMBER,
  data VARCHAR2(100)
)
STORAGE (BUFFER_POOL DEFAULT);

select segment_name, buffer_pool from dba_segments where segment_name = 'DEFAULT_TABLE';

// 9
ALTER SESSION SET CONTAINER = CDB$ROOT;

select bytes/1024/1024 as redo_log_buffer_mb
    from v$sgastat
    where name = 'log_buffer';
    
// 10
select pool, name, bytes/1024/1024 as free_db
    from v$sgastat
    where pool = 'large pool' and name = 'free memory';
    
// 11
select name, status, busy, idle from v$dispatcher;  // если пусто, значит shared-соединений нет

select username, program, server
    from v$session
    where username is not null;
    
// 12
select name, paddr, description
    from v$bgprocess
    where paddr is not null;
    
// 13
select sid, serial#, username, program, server, status
    from v$session
    where username is not null;
    
// 14
select count(*) as dbw_process_count
    from v$bgprocess
    where name like 'DBW%';

// 15
select name, network_name
    from v$services
    where name is not null;
    
// 16
select name, value
    from v$parameter
    where name like '%dispatcher%';
    
// 17
// ps -ef | grep tnslsnr

// 18
// docker exec -it oracledb19c bash

// cat $ORACLE_HOME/network/admin/listener.ora

// 19
// lsnrctl
// LSNRCTL> status
// LSNRCTL> services
// LSNRCTL> stop
// LSNRCTL> start
// LSNRCTL> version
// LSNRCTL> help
// LSNRCTL> exit

// 20
// lsnrctl
// LSNRCTL> services
