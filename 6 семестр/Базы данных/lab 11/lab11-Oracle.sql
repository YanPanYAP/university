SET DEFINE OFF;
SET SERVEROUTPUT ON;

-- 5
CREATE OR REPLACE TYPE lab11_task_row_ot AS OBJECT (
    task_id        NUMBER,
    title          VARCHAR2(255),
    description    VARCHAR2(4000),
    creator_id     NUMBER,
    creator_name   VARCHAR2(200),
    executor_id    NUMBER,
    executor_name  VARCHAR2(200),
    status_id      NUMBER,
    status_name    VARCHAR2(100),
    priority       NUMBER(10,1),
    start_date     DATE,
    due_date       DATE,
    created_at     DATE
);
/

CREATE OR REPLACE TYPE lab11_task_tab_nt AS TABLE OF lab11_task_row_ot;
/

CREATE OR REPLACE FUNCTION fn_lab11_tasks_period
(
    p_date_from IN DATE,
    p_date_to   IN DATE
)
RETURN lab11_task_tab_nt PIPELINED
AS
BEGIN
    FOR r IN (
        SELECT
            t.task_id,
            t.title,
            DBMS_LOB.SUBSTR(t.description, 4000, 1) AS description,
            t.creator_id,
            u_creator.full_name AS creator_name,
            t.executor_id,
            u_executor.full_name AS executor_name,
            t.status_id,
            s.status_name,
            ROUND(t.priority, 1) AS priority,
            t.start_date,
            t.due_date,
            t.created_at
        FROM Task t
        JOIN Users u_creator ON u_creator.user_id = t.creator_id
        JOIN Users u_executor ON u_executor.user_id = t.executor_id
        JOIN Status s ON s.status_id = t.status_id
        WHERE t.created_at >= TRUNC(p_date_from)
          AND t.created_at < TRUNC(p_date_to) + 1
        ORDER BY t.created_at, t.task_id
    ) LOOP
        PIPE ROW(
            lab11_task_row_ot(
                r.task_id,
                r.title,
                r.description,
                r.creator_id,
                r.creator_name,
                r.executor_id,
                r.executor_name,
                r.status_id,
                r.status_name,
                r.priority,
                r.start_date,
                r.due_date,
                r.created_at
            )
        );
    END LOOP;

    RETURN;
END;
/

SELECT *
FROM TABLE(fn_lab11_tasks_period(DATE '2024-01-15', DATE '2024-02-15'))
ORDER BY created_at, task_id;
/

-- 6
SET PAGESIZE 50000
SET LINESIZE 32767
SET FEEDBACK OFF
SET HEADING OFF
SET TRIMSPOOL ON

SPOOL '/Users/yanpaulovich/Documents/fit/БД/lab 11/export11-oracle.csv'
SELECT
    task_id || ';' ||
    title || ';' ||
    REPLACE(REPLACE(description, CHR(10), ' '), CHR(13), ' ') || ';' ||
    creator_id || ';' ||
    creator_name || ';' ||
    executor_id || ';' ||
    executor_name || ';' ||
    status_id || ';' ||
    status_name || ';' ||
    TO_CHAR(priority, 'FM9999990D0', 'NLS_NUMERIC_CHARACTERS=.,') || ';' ||
    TO_CHAR(start_date, 'YYYY-MM-DD') || ';' ||
    TO_CHAR(due_date, 'YYYY-MM-DD') || ';' ||
    TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI:SS')
FROM TABLE(fn_lab11_tasks_period(DATE '2024-01-15', DATE '2024-02-15'))
ORDER BY created_at, task_id;
SPOOL OFF

SET FEEDBACK ON
SET HEADING ON

/*
docker exec oracle21c bash -lc "mkdir -p /tmp/lab11"

docker cp "/Users/yanpaulovich/Documents/fit/БД/lab 11/oracle_task_loader.ctl" oracle21c:/tmp/lab11/oracle_task_loader.ctl
docker cp "/Users/yanpaulovich/Documents/fit/БД/lab 11/import11-oracle.txt" oracle21c:/tmp/lab11/import11-oracle.txt

docker exec oracle21c bash -lc "/opt/oracle/product/21c/dbhomeXE/bin/sqlldr \
  userid='task_control/MyPass123@XEPDB1' \
  control='/tmp/lab11/oracle_task_loader.ctl' \
  log='/tmp/lab11/oracle_task_loader.log' \
  bad='/tmp/lab11/oracle_task_loader.bad'"


*/

SELECT
    task_id,
    title,
    creator_id,
    executor_id,
    status_id,
    priority,
    start_date,
    due_date,
    created_at
FROM Task
WHERE title LIKE 'ORACLE LOAD%'
ORDER BY task_id DESC;
/


DELETE FROM Task
WHERE title LIKE 'ORACLE LOAD%';

COMMIT;

