CREATE PLUGGABLE DATABASE YAPLLAB_PDB
  ADMIN USER pdbadmin IDENTIFIED BY StrongPass123
  ROLES = (DBA)
  DEFAULT TABLESPACE users
  DATAFILE '/opt/oracle/oradata/ORCLCDB/YAPLLAB_PDB/users01.dbf' SIZE 250M AUTOEXTEND ON
  FILE_NAME_CONVERT = ('pdbseed', 'YAPLLAB_PDB');

ALTER PLUGGABLE DATABASE YAP_PDB OPEN;

ALTER SESSION SET CONTAINER = YAPLLAB_PDB;

ALTER DATABASE OPEN;

-- 1
SELECT tablespace_name, file_name, bytes/1024/1024 AS size_mb, autoextensible
FROM dba_data_files
UNION ALL
SELECT tablespace_name, file_name, bytes/1024/1024 AS size_mb, autoextensible
FROM dba_temp_files
ORDER BY tablespace_name, file_name;


--2
CREATE SMALLFILE TABLESPACE YAP_QDAT
  DATAFILE '/opt/oracle/oradata/ORCLCDB/YAP_QDAT.dbf' SIZE 10M
  AUTOEXTEND ON NEXT 10M MAXSIZE 500M
  SEGMENT SPACE MANAGEMENT AUTO;

ALTER TABLESPACE YAP_QDAT ONLINE;

CREATE USER YAP_lab4 IDENTIFIED BY "YapNew"
  DEFAULT TABLESPACE YAP_QDAT
  TEMPORARY TABLESPACE TEMP
  ACCOUNT UNLOCK;

GRANT CREATE SESSION, CREATE TABLE, CREATE VIEW, CREATE PROCEDURE TO YAP_lab4;

ALTER USER YAP_lab4 QUOTA 2M ON YAP_QDAT;

--
ALTER SESSION SET CURRENT_SCHEMA = YAP_lab4;

CREATE TABLE YAP_T1 (
  id NUMBER PRIMARY KEY,
  name VARCHAR2(50)
) TABLESPACE YAP_QDAT;

INSERT INTO YAP_T1 VALUES (1, 'Liza');
INSERT INTO YAP_T1 VALUES (2, 'Pavel');
INSERT INTO YAP_T1 VALUES (3, 'Elena');
COMMIT;

SELECT * FROM YAP_T1;


-- 3
ALTER SESSION SET CONTAINER = YAPLLAB_PDB;
SELECT segment_name, segment_type, owner, tablespace_name
FROM dba_segments
WHERE tablespace_name = 'YAP_QDAT'
ORDER BY segment_type, segment_name;

SELECT segment_name, segment_type, owner, tablespace_name
FROM dba_segments
WHERE tablespace_name = 'YAP_QDAT'
  AND segment_name = 'YAP_T1'
  AND owner = 'YAP_LAB4';


--4
ALTER SESSION SET CURRENT_SCHEMA = YAP_lab4;
DROP TABLE YAP_T1;

SELECT segment_name, segment_type, owner, tablespace_name
FROM dba_segments
WHERE tablespace_name = 'YAP_QDAT'
ORDER BY segment_type, segment_name;

SELECT OWNER, ORIGINAL_NAME, OBJECT_NAME, OPERATION, DROPTIME
FROM DBA_RECYCLEBIN
WHERE ORIGINAL_NAME = 'YAP_T1';


--5
SHOW PARAMETER recyclebin;

FLASHBACK TABLE "YAP_LAB4"."YAP_T1" TO BEFORE DROP;

SELECT * FROM YAP_T1;

--6
BEGIN
  FOR i IN 4..10000 LOOP
    INSERT INTO YAP_T1 (id, name) VALUES (i, 'name_' || i);
    IF MOD(i, 1000) = 0 THEN
      COMMIT;
    END IF;
  END LOOP;
  COMMIT;
END;
/

SELECT COUNT(*) AS total_rows FROM YAP_LAB4.YAP_T1;


--7
SELECT owner, segment_name, segment_type, COUNT(*) AS extents_count,
       SUM(bytes) AS total_bytes,
       SUM(blocks) AS total_blocks
FROM dba_extents
WHERE owner = 'YAP_LAB4'
  AND segment_name = 'YAP_T1'
GROUP BY owner, segment_name, segment_type;

SELECT owner, segment_name, file_id, block_id, bytes, blocks
FROM dba_extents
WHERE owner = 'YAP_LAB4'
  AND segment_name = 'YAP_T1'
ORDER BY file_id, block_id;


--8
DROP TABLESPACE YAP_QDAT INCLUDING CONTENTS AND DATAFILES;

SELECT tablespace_name 
FROM dba_tablespaces 
WHERE tablespace_name = 'YAP_QDAT';


--9
ALTER SESSION SET CONTAINER = CDB$ROOT;
SELECT GROUP#, STATUS, MEMBERS FROM v$log ORDER BY GROUP#;
SELECT GROUP#, STATUS, MEMBERS FROM v$log WHERE STATUS = 'CURRENT';


--10
SELECT GROUP#, MEMBER, TYPE, STATUS FROM v$logfile ORDER BY GROUP#, MEMBER;


--11
ALTER SYSTEM SWITCH LOGFILE;
SELECT TO_CHAR(SYSDATE, 'DD-MM-YYYY HH24:MI:SS') AS switch_time FROM DUAL;
SELECT GROUP#, STATUS FROM V$LOG WHERE STATUS = 'CURRENT';


--12
ALTER SESSION SET CONTAINER = CDB$ROOT;
ALTER DATABASE ADD LOGFILE GROUP 4 (
    '/opt/oracle/oradata/ORCLCDB/redo_g4_1.log',
    '/opt/oracle/oradata/ORCLCDB/redo_g4_2.log',
    '/opt/oracle/oradata/ORCLCDB/redo_g4_3.log'
) SIZE 50M;

SELECT GROUP#, STATUS, MEMBERS FROM v$log WHERE group# = 4;
SELECT GROUP#, MEMBER FROM v$logfile WHERE group# = 4;

ALTER SYSTEM SWITCH LOGFILE;
SELECT CURRENT_SCN FROM v$DATABASE;

--13

ALTER SESSION SET CONTAINER = CDB$ROOT;
ALTER SYSTEM SWITCH LOGFILE;

ALTER SYSTEM CHECKPOINT GLOBAL;

SELECT GROUP#, STATUS FROM V$LOG WHERE GROUP# = 4;

ALTER DATABASE DROP LOGFILE GROUP 4;

-- rm -f /opt/oracle/oradata/ORCLCDB/redo_g4_1.log
-- rm -f /opt/oracle/oradata/ORCLCDB/redo_g4_2.log
-- rm -f /opt/oracle/oradata/ORCLCDB/redo_g4_3.log


--14
ARCHIVE LOG LIST;
SELECT LOG_MODE FROM V$DATABASE;


--15
SELECT NVL(MAX(SEQUENCE#), 0) AS last_archive FROM V$ARCHIVED_LOG;


--16
--docker exec -it oracledb19c bach
-- sqlplus / as sysdba
-- SHUTDOWN IMMEDIATE;
-- STARTUP MOUNT;
-- ALTER DATABASE ARCHIVELOG;
-- ALTER DATABASE OPEN;

ARCHIVE LOG LIST;


--17
ALTER SESSION SET CONTAINER = CDB$ROOT;
ALTER SYSTEM SWITCH LOGFILE;

SELECT SEQUENCE#, NAME, FIRST_TIME, NEXT_TIME FROM V$ARCHIVED_LOG ORDER BY SEQUENCE# DESC FETCH FIRST 5 ROWS ONLY;
SELECT MAX(SEQUENCE#) AS last_archive FROM V$ARCHIVED_LOG;
SELECT CURRENT_SCN FROM V$DATABASE;
SELECT SEQUENCE#, FIRST_CHANGE#, NEXT_CHANGE# FROM V$ARCHIVED_LOG ORDER BY SEQUENCE#;


--18
--docker exec -it oracledb19c bash
-- sqlplus / as sysdba
-- SHUTDOWN IMMEDIATE;
-- STARTUP MOUNT;
-- ALTER DATABASE NOARCHIVELOG;
-- ALTER DATABASE OPEN;

ARCHIVE LOG LIST;


--19
SELECT NAME FROM V$CONTROLFILE;


--20
-- strings /opt/oracle/oradata/ORCLCDB/control01.ctl | more
SHOW PARAMETER CONTROL;

--21
SHOW PARAMETER spfile;
-- ls -lh /opt/oracle/product/19c/dbhome_1/dbs/spfileORCLCDB.ora


--22
CREATE PFILE='/opt/oracle/product/19c/dbhome_1/dbs/YAP_PFILE.ORA' FROM SPFILE;
-- cat /opt/oracle/product/19c/dbhome_1/dbs/YAP_PFILE.ORA

--23
SHOW PARAMETER PASSWORD;

--24
SELECT * FROM v$diag_info;


//25
// less /opt/oracle/diag/rdbms/orclcdb/ORCLCDB/trace/alert_ORCLCDB.log
//ls -lh /opt/oracle/oradata/dbconfig/ORCLCDB/orapwORCLCDB


--26
-- rm -rf /opt/oracle/oradata/FREE/YAP_PDB/YAP_QDAT.dbf

-- rm -rf /opt/oracle/oradata/FREE/redo_g4_1.log
-- rm -rf /opt/oracle/oradata/FREE/redo_g4_2.log
-- rm -rf /opt/oracle/oradata/FREE/redo_g4_3.log


-- rm -rf /opt/oracle/product/23ai/dbhomeFree/dbs/YAP_PFILE.ORA