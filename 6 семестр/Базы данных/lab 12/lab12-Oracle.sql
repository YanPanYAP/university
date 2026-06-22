SET DEFINE OFF;
SET SERVEROUTPUT ON;

-- drop
BEGIN
    EXECUTE IMMEDIATE 'DROP PROCEDURE pr_lab12_extract_by_status';
EXCEPTION WHEN OTHERS THEN
    IF SQLCODE <> -4043 THEN RAISE; END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'DROP PROCEDURE pr_lab12_insert_report';
EXCEPTION WHEN OTHERS THEN
    IF SQLCODE <> -4043 THEN RAISE; END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'DROP PROCEDURE pr_lab12_generate_report_xml';
EXCEPTION WHEN OTHERS THEN
    IF SQLCODE <> -4043 THEN RAISE; END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'DROP INDEX ix_report_xml';
EXCEPTION WHEN OTHERS THEN
    IF SQLCODE <> -1418 THEN RAISE; END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'DROP TABLE Report PURGE';
EXCEPTION WHEN OTHERS THEN
    IF SQLCODE <> -942 THEN RAISE; END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'DROP SEQUENCE seq_report';
EXCEPTION WHEN OTHERS THEN
    IF SQLCODE <> -2289 THEN RAISE; END IF;
END;
/

-- 1
CREATE TABLE Report
(
    id NUMBER PRIMARY KEY,
    report_xml XMLTYPE NOT NULL
);

CREATE SEQUENCE seq_report START WITH 1 INCREMENT BY 1 NOCACHE;

-- 2
CREATE OR REPLACE PROCEDURE pr_lab12_generate_report_xml
(
    p_date_from IN DATE,
    p_date_to   IN DATE,
    p_result_xml OUT XMLTYPE
)
AS
BEGIN
    SELECT
        XMLELEMENT("task_report",
            XMLATTRIBUTES(
                TO_CHAR(SYSTIMESTAMP, 'YYYY-MM-DD"T"HH24:MI:SS.FF3TZH:TZM') AS "generated_at",
                TO_CHAR(TRUNC(p_date_from), 'YYYY-MM-DD') AS "date_from",
                TO_CHAR(TRUNC(p_date_to), 'YYYY-MM-DD') AS "date_to"
            ),
            XMLELEMENT("tasks",
                (
                    SELECT XMLAGG(
                        XMLELEMENT("task",
                            XMLATTRIBUTES(
                                t.task_id AS "task_id",
                                t.status_id AS "status_id",
                                t.creator_id AS "creator_id",
                                t.executor_id AS "executor_id"
                            ),
                            XMLELEMENT("title", t.title),
                            XMLELEMENT("description", DBMS_LOB.SUBSTR(t.description, 4000, 1)),
                            XMLELEMENT("status_name", s.status_name),
                            XMLELEMENT("creator_name", uc.full_name),
                            XMLELEMENT("executor_name", ue.full_name),
                            XMLELEMENT("priority", ROUND(t.priority, 1)),
                            XMLELEMENT("start_date", TO_CHAR(t.start_date, 'YYYY-MM-DD')),
                            XMLELEMENT("due_date", TO_CHAR(t.due_date, 'YYYY-MM-DD')),
                            XMLELEMENT("created_at", TO_CHAR(t.created_at, 'YYYY-MM-DD HH24:MI:SS'))
                        )
                    )
                    FROM Task t
                    JOIN Users uc ON uc.user_id = t.creator_id
                    JOIN Users ue ON ue.user_id = t.executor_id
                    JOIN Status s ON s.status_id = t.status_id
                    WHERE t.created_at >= TRUNC(p_date_from)
                      AND t.created_at < TRUNC(p_date_to) + 1
                )
            ),
            XMLELEMENT("status_totals",
                (
                    SELECT XMLAGG(
                        XMLELEMENT("status_total",
                            XMLATTRIBUTES(
                                q.status_id AS "status_id",
                                q.status_name AS "status_name",
                                q.task_count AS "task_count",
                                q.avg_priority AS "avg_priority"
                            )
                        )
                    )
                    FROM (
                        SELECT
                            t.status_id,
                            s.status_name,
                            COUNT(*) AS task_count,
                            ROUND(AVG(t.priority), 1) AS avg_priority
                        FROM Task t
                        JOIN Status s ON s.status_id = t.status_id
                        WHERE t.created_at >= TRUNC(p_date_from)
                          AND t.created_at < TRUNC(p_date_to) + 1
                        GROUP BY t.status_id, s.status_name
                    ) q
                )
            ),
            XMLELEMENT("executor_totals",
                (
                    SELECT XMLAGG(
                        XMLELEMENT("executor_total",
                            XMLATTRIBUTES(
                                q.executor_id AS "executor_id",
                                q.executor_name AS "executor_name",
                                q.task_count AS "task_count"
                            )
                        )
                    )
                    FROM (
                        SELECT
                            t.executor_id,
                            ue.full_name AS executor_name,
                            COUNT(*) AS task_count
                        FROM Task t
                        JOIN Users ue ON ue.user_id = t.executor_id
                        WHERE t.created_at >= TRUNC(p_date_from)
                          AND t.created_at < TRUNC(p_date_to) + 1
                        GROUP BY t.executor_id, ue.full_name
                    ) q
                )
            ),
            XMLELEMENT("overall_total",
                XMLATTRIBUTES(
                    (
                        SELECT COUNT(*)
                        FROM Task t
                        WHERE t.created_at >= TRUNC(p_date_from)
                          AND t.created_at < TRUNC(p_date_to) + 1
                    ) AS "task_count",
                    (
                        SELECT ROUND(AVG(t.priority), 1)
                        FROM Task t
                        WHERE t.created_at >= TRUNC(p_date_from)
                          AND t.created_at < TRUNC(p_date_to) + 1
                    ) AS "avg_priority"
                )
            )
        )
    INTO p_result_xml
    FROM dual;
END;
/

-- 3
CREATE OR REPLACE PROCEDURE pr_lab12_insert_report
(
    p_date_from IN DATE,
    p_date_to   IN DATE,
    p_new_report_id OUT NUMBER
)
AS
    v_xml XMLTYPE;
BEGIN
    pr_lab12_generate_report_xml(p_date_from, p_date_to, v_xml);

    INSERT INTO Report(id, report_xml)
    VALUES (seq_report.NEXTVAL, v_xml)
    RETURNING id INTO p_new_report_id;
END;
/

-- 4
BEGIN
    EXECUTE IMMEDIATE 'CREATE INDEX ix_report_xml ON Report(report_xml) INDEXTYPE IS XDB.XMLINDEX';
EXCEPTION WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('XMLINDEX not created: ' || SQLERRM);
END;
/

-- 5
CREATE OR REPLACE PROCEDURE pr_lab12_extract_by_status
(
    p_status_name IN VARCHAR2,
    p_result OUT SYS_REFCURSOR
)
AS
BEGIN
    OPEN p_result FOR
        SELECT
            r.id AS report_id,
            x.task_id,
            x.title,
            x.status_name,
            x.executor_name,
            x.priority,
            x.due_date
        FROM Report r,
             XMLTABLE(
                 '/task_report/tasks/task'
                 PASSING r.report_xml
                 COLUMNS
                     task_id NUMBER PATH '@task_id',
                     title VARCHAR2(255) PATH 'title',
                     status_name VARCHAR2(100) PATH 'status_name',
                     executor_name VARCHAR2(200) PATH 'executor_name',
                     priority NUMBER PATH 'priority',
                     due_date VARCHAR2(10) PATH 'due_date'
             ) x
        WHERE x.status_name = p_status_name
        ORDER BY r.id DESC, x.task_id;
END;
/

-- vrrif
DECLARE
    v_xml CLOB;
    v_xml_obj XMLTYPE;
BEGIN
    pr_lab12_generate_report_xml(DATE '2024-01-01', DATE '2026-12-31', v_xml_obj);
    SELECT XMLSERIALIZE(CONTENT v_xml_obj AS CLOB) INTO v_xml FROM dual;
    DBMS_OUTPUT.PUT_LINE('XML preview (first 3000 chars):');
    DBMS_OUTPUT.PUT_LINE(DBMS_LOB.SUBSTR(v_xml, 3000, 1));
END;
/

DECLARE
    v_report_id NUMBER;
BEGIN
    pr_lab12_insert_report(DATE '2024-01-01', DATE '2026-12-31', v_report_id);
    DBMS_OUTPUT.PUT_LINE('Inserted report id = ' || v_report_id);
END;
/

SELECT *
FROM (
    SELECT
        r.id,
        XMLCAST(XMLQUERY('/task_report/@generated_at' PASSING r.report_xml RETURNING CONTENT) AS VARCHAR2(40)) AS generated_at,
        XMLCAST(XMLQUERY('count(/task_report/tasks/task)' PASSING r.report_xml RETURNING CONTENT) AS NUMBER) AS task_count
    FROM Report r
    ORDER BY r.id DESC
)
WHERE ROWNUM <= 5;
/

SELECT
    id,
    XMLSERIALIZE(CONTENT report_xml AS CLOB) AS report_xml_text
FROM Report
ORDER BY id;
/

DECLARE
    v_status VARCHAR2(100);
    v_cur SYS_REFCURSOR;
    v_report_id NUMBER;
    v_task_id NUMBER;
    v_title VARCHAR2(255);
    v_status_name VARCHAR2(100);
    v_executor_name VARCHAR2(200);
    v_priority NUMBER;
    v_due_date VARCHAR2(10);
    v_rows NUMBER := 0;
BEGIN
    SELECT MIN(x.status_name)
    INTO v_status
    FROM Report r,
         XMLTABLE(
             '/task_report/tasks/task'
             PASSING r.report_xml
             COLUMNS status_name VARCHAR2(100) PATH 'status_name'
         ) x;

    DBMS_OUTPUT.PUT_LINE('Selected status = ' || NVL(v_status, '<no reports/tasks>'));

    pr_lab12_extract_by_status(v_status, v_cur);

    LOOP
        FETCH v_cur INTO v_report_id, v_task_id, v_title, v_status_name, v_executor_name, v_priority, v_due_date;
        EXIT WHEN v_cur%NOTFOUND;
        v_rows := v_rows + 1;
        DBMS_OUTPUT.PUT_LINE('report=' || v_report_id || ', task=' || v_task_id || ', status=' || v_status_name || ', title=' || v_title);
    END LOOP;

    CLOSE v_cur;

    IF v_rows = 0 THEN
        DBMS_OUTPUT.PUT_LINE('No rows found. Insert a report first with pr_lab12_insert_report.');
    END IF;
END;
/
