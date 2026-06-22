SET DEFINE OFF;
SET SERVEROUTPUT ON;

BEGIN
    EXECUTE IMMEDIATE 'ALTER SESSION SET CONTAINER = XEPDB1';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE NOT IN (-65090, -65144) THEN
            RAISE;
        END IF;
END;
/

DECLARE
    v_exists NUMBER;
    v_dest   VARCHAR2(4000);
BEGIN
    SELECT COUNT(*)
      INTO v_exists
      FROM dba_tablespaces
     WHERE tablespace_name = 'LOB_TS';

    IF v_exists = 0 THEN
        SELECT value
          INTO v_dest
          FROM v$parameter
         WHERE name = 'db_create_file_dest';

        IF v_dest IS NOT NULL THEN
            EXECUTE IMMEDIATE 'CREATE TABLESPACE lob_ts DATAFILE SIZE 100M AUTOEXTEND ON NEXT 20M MAXSIZE 2G';
        ELSE
            EXECUTE IMMEDIATE q'[CREATE TABLESPACE lob_ts DATAFILE '/opt/oracle/oradata/XE/XEPDB1/lob_ts01.dbf' SIZE 100M AUTOEXTEND ON NEXT 20M MAXSIZE 2G]';
        END IF;
    END IF;
END;
/

CREATE OR REPLACE DIRECTORY LOB_DOC_DIR AS '/opt/oracle/lab10_files';

BEGIN
    EXECUTE IMMEDIATE 'GRANT CREATE SESSION TO task_control';
    EXECUTE IMMEDIATE 'GRANT CREATE PROCEDURE TO task_control';
    EXECUTE IMMEDIATE 'GRANT READ ON DIRECTORY lob_doc_dir TO task_control';
END;
/

ALTER USER task_control QUOTA 300M ON lob_ts;

DECLARE
    v_has_foto NUMBER;
    v_has_doc  NUMBER;
BEGIN
    SELECT COUNT(*)
      INTO v_has_foto
      FROM dba_tab_columns
     WHERE owner = 'TASK_CONTROL'
       AND table_name = 'ATTACHMENT'
       AND column_name = 'FOTO';

    SELECT COUNT(*)
      INTO v_has_doc
      FROM dba_tab_columns
     WHERE owner = 'TASK_CONTROL'
       AND table_name = 'ATTACHMENT'
       AND column_name = 'DOC_FILE';

    IF v_has_foto = 0 AND v_has_doc = 0 THEN
        EXECUTE IMMEDIATE 'ALTER TABLE task_control.attachment ADD (foto BLOB, doc_file BFILE) LOB (foto) STORE AS (TABLESPACE lob_ts)';
    ELSIF v_has_foto = 0 AND v_has_doc > 0 THEN
        EXECUTE IMMEDIATE 'ALTER TABLE task_control.attachment ADD (foto BLOB) LOB (foto) STORE AS (TABLESPACE lob_ts)';
    ELSIF v_has_foto > 0 AND v_has_doc = 0 THEN
        EXECUTE IMMEDIATE 'ALTER TABLE task_control.attachment ADD (doc_file BFILE)';
    END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'ALTER TABLE task_control.attachment MOVE LOB (foto) STORE AS (TABLESPACE lob_ts)';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE NOT IN (-904, -22859) THEN
            RAISE;
        END IF;
END;
/

DECLARE
    v_task_id        NUMBER;
    v_role_id        NUMBER;
    v_status_id      NUMBER;
    v_creator_id     NUMBER;
    v_executor_id    NUMBER;
BEGIN
    SELECT MIN(task_id) INTO v_task_id FROM task_control.task;
    IF v_task_id IS NOT NULL THEN
        RETURN;
    END IF;

    SELECT MIN(role_id) INTO v_role_id FROM task_control.role;
    IF v_role_id IS NULL THEN
        INSERT INTO task_control.role (role_id, role_name, description)
        VALUES (task_control.seq_role.NEXTVAL, 'LOB_TEMP_ROLE', 'Auto row for lab10')
        RETURNING role_id INTO v_role_id;
    END IF;

    SELECT MIN(status_id) INTO v_status_id FROM task_control.status;
    IF v_status_id IS NULL THEN
        INSERT INTO task_control.status (status_id, status_name)
        VALUES (task_control.seq_status.NEXTVAL, 'LOB_TEMP_STATUS')
        RETURNING status_id INTO v_status_id;
    END IF;

    BEGIN
        SELECT user_id INTO v_creator_id
          FROM task_control.users
         WHERE email = 'lob_creator_lab10@example.local';
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            INSERT INTO task_control.users (user_id, full_name, email, password, role_id, department_id)
            VALUES (task_control.seq_user.NEXTVAL, 'LOB Creator', 'lob_creator_lab10@example.local', 'pass', v_role_id, NULL)
            RETURNING user_id INTO v_creator_id;
    END;

    BEGIN
        SELECT user_id INTO v_executor_id
          FROM task_control.users
         WHERE email = 'lob_executor_lab10@example.local';
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            INSERT INTO task_control.users (user_id, full_name, email, password, role_id, department_id)
            VALUES (task_control.seq_user.NEXTVAL, 'LOB Executor', 'lob_executor_lab10@example.local', 'pass', v_role_id, NULL)
            RETURNING user_id INTO v_executor_id;
    END;

    INSERT INTO task_control.task (
        task_id, title, description, creator_id, executor_id, status_id, priority, start_date, due_date, created_at
    )
    VALUES (
        task_control.seq_task.NEXTVAL, 'Lab10 LOB Task', 'Auto-created for lab10', v_creator_id, v_executor_id, v_status_id,
        1, SYSDATE, SYSDATE + 7, SYSDATE
    );
END;
/

DECLARE
BEGIN
    FOR rec IN (
        SELECT owner, index_name
        FROM dba_indexes
        WHERE table_owner = 'TASK_CONTROL'
          AND table_name = 'ATTACHMENT'
          AND status = 'UNUSABLE'
    ) LOOP
        EXECUTE IMMEDIATE 'ALTER INDEX ' || rec.owner || '.' || rec.index_name || ' REBUILD';
    END LOOP;
END;
/

CREATE OR REPLACE PROCEDURE task_control.pr_add_attachment_lob (
    p_photo_filename IN VARCHAR2,
    p_doc_filename   IN VARCHAR2,
    p_task_id        IN NUMBER DEFAULT NULL
)
AS
    v_task_id       NUMBER;
    v_attachment_id NUMBER;
    v_blob          BLOB;
    v_bfile_photo   BFILE;
BEGIN
    IF p_task_id IS NULL THEN
        SELECT MIN(task_id) INTO v_task_id FROM task_control.task;
    ELSE
        v_task_id := p_task_id;
    END IF;

    IF v_task_id IS NULL THEN
        RAISE_APPLICATION_ERROR(-20011, 'TASK not found for attachment insert');
    END IF;

    INSERT INTO task_control.attachment (
        attachment_id, task_id, file_name, file_path, uploaded_at, foto, doc_file
    )
    VALUES (
        task_control.seq_attachment.NEXTVAL,
        v_task_id,
        p_photo_filename,
        '/opt/oracle/lab10_files/' || p_doc_filename,
        SYSDATE,
        EMPTY_BLOB(),
        BFILENAME('LOB_DOC_DIR', p_doc_filename)
    )
    RETURNING attachment_id INTO v_attachment_id;

    SELECT foto
      INTO v_blob
      FROM task_control.attachment
     WHERE attachment_id = v_attachment_id
       FOR UPDATE;

    v_bfile_photo := BFILENAME('LOB_DOC_DIR', p_photo_filename);

    IF DBMS_LOB.FILEEXISTS(v_bfile_photo) = 1 THEN
        DBMS_LOB.FILEOPEN(v_bfile_photo, DBMS_LOB.FILE_READONLY);
        DBMS_LOB.LOADFROMFILE(v_blob, v_bfile_photo, DBMS_LOB.GETLENGTH(v_bfile_photo));
        DBMS_LOB.FILECLOSE(v_bfile_photo);
    ELSE
        RAISE_APPLICATION_ERROR(-20010, 'Photo file not found: ' || p_photo_filename);
    END IF;

    COMMIT;
END;
/

BEGIN
    task_control.pr_add_attachment_lob(
        p_photo_filename => 'photo.jpg',
        p_doc_filename   => 'doc.pdf'
    );
END;
/

SELECT
    'photo.jpg' AS photo_name,
    '/opt/oracle/lab10_files/photo.jpg' AS photo_path,
    DBMS_LOB.FILEEXISTS(BFILENAME('LOB_DOC_DIR', 'photo.jpg')) AS photo_exists,
    CASE
        WHEN DBMS_LOB.FILEEXISTS(BFILENAME('LOB_DOC_DIR', 'photo.jpg')) = 1
        THEN DBMS_LOB.GETLENGTH(BFILENAME('LOB_DOC_DIR', 'photo.jpg'))
        ELSE NULL
    END AS photo_len
FROM dual;

SELECT
    'doc.pdf' AS doc_name,
    '/opt/oracle/lab10_files/doc.pdf' AS doc_path,
    DBMS_LOB.FILEEXISTS(BFILENAME('LOB_DOC_DIR', 'doc.pdf')) AS doc_exists,
    CASE
        WHEN DBMS_LOB.FILEEXISTS(BFILENAME('LOB_DOC_DIR', 'doc.pdf')) = 1
        THEN DBMS_LOB.GETLENGTH(BFILENAME('LOB_DOC_DIR', 'doc.pdf'))
        ELSE NULL
    END AS doc_len
FROM dual;


-- docker cp "/Users/yanpaulovich/Documents/porshe/DT 1.jpg" oracle21c:/opt/oracle/lab10_files/photo.jpg
-- docker cp "/Users/yanpaulovich/Documents/fit/БД/lab 1/БД lab 1.docx.pdf" oracle21c:/opt/oracle/lab10_files/doc.pdf

DELETE FROM task_control.attachment
WHERE file_name = 'photo.jpg'
   OR file_path = '/opt/oracle/lab10_files/doc.pdf';

COMMIT;

SELECT
    attachment_id,
    file_name,
    file_path,
    DBMS_LOB.GETLENGTH(foto) AS photo_size_bytes
FROM task_control.attachment
WHERE file_name = 'photo.jpg';

