SET DEFINE OFF;
SET SERVEROUTPUT ON;

-- 1
BEGIN
   EXECUTE IMMEDIATE 'DROP VIEW vw_assignment_summary';
EXCEPTION WHEN OTHERS THEN
   IF SQLCODE <> -942 THEN RAISE; END IF;
END;
/

BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE assignments_obj PURGE';
EXCEPTION WHEN OTHERS THEN
   IF SQLCODE <> -942 THEN RAISE; END IF;
END;
/

BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE executors_obj PURGE';
EXCEPTION WHEN OTHERS THEN
   IF SQLCODE <> -942 THEN RAISE; END IF;
END;
/

BEGIN
   EXECUTE IMMEDIATE 'DROP TYPE assignment_t FORCE';
EXCEPTION WHEN OTHERS THEN
   IF SQLCODE NOT IN (-4043, -24094) THEN RAISE; END IF;
END;
/

BEGIN
   EXECUTE IMMEDIATE 'DROP TYPE executor_t FORCE';
EXCEPTION WHEN OTHERS THEN
   IF SQLCODE NOT IN (-4043, -24094) THEN RAISE; END IF;
END;
/








-- 2
CREATE OR REPLACE TYPE executor_t AS OBJECT (
    user_id NUMBER,
    full_name VARCHAR2(200),
    email VARCHAR2(200),
    role_id NUMBER,
    department_id NUMBER,
    MAP MEMBER FUNCTION executor_map RETURN VARCHAR2,
    MEMBER FUNCTION contact_summary RETURN VARCHAR2,
    MEMBER PROCEDURE anonymize_email,
    CONSTRUCTOR FUNCTION executor_t (
        p_user_id NUMBER,
        p_full_name VARCHAR2,
        p_email VARCHAR2,
        p_role_id NUMBER,
        p_department_id NUMBER
    ) RETURN SELF AS RESULT
);
/

CREATE OR REPLACE TYPE BODY executor_t AS
    CONSTRUCTOR FUNCTION executor_t (
        p_user_id NUMBER,
        p_full_name VARCHAR2,
        p_email VARCHAR2,
        p_role_id NUMBER,
        p_department_id NUMBER
    ) RETURN SELF AS RESULT IS
    BEGIN
        SELF.user_id := p_user_id;
        SELF.full_name := p_full_name;
        SELF.email := NVL(p_email, TRANSLATE(p_full_name, ' ', '.') || '@executor.local');
        SELF.role_id := p_role_id;
        SELF.department_id := p_department_id;
        RETURN;
    END;

    MAP MEMBER FUNCTION executor_map RETURN VARCHAR2 IS
    BEGIN
        RETURN NVL(full_name, 'Unknown Executor');
    END;

    MEMBER FUNCTION contact_summary RETURN VARCHAR2 IS
    BEGIN
        RETURN 'Executor: ' || full_name || ' <' || email || '>'; 
    END;

    MEMBER PROCEDURE anonymize_email IS
    BEGIN
        email := LOWER(REGEXP_REPLACE(NVL(full_name, 'guest'), '\s+', '.')) || '@company.local';
    END;
END;
/

CREATE OR REPLACE TYPE assignment_t AS OBJECT (
    assignment_id NUMBER,
    title VARCHAR2(255),
    description CLOB,
    creator executor_t,
    executor executor_t,
    status VARCHAR2(100),
    priority NUMBER,
    start_date DATE,
    due_date DATE,
    created_at DATE,
    MAP MEMBER FUNCTION assignment_map RETURN NUMBER DETERMINISTIC,
    MEMBER FUNCTION days_to_due RETURN NUMBER,
    MEMBER PROCEDURE escalate_priority(p_increment NUMBER),
    CONSTRUCTOR FUNCTION assignment_t (
        p_assignment_id NUMBER,
        p_title VARCHAR2,
        p_description CLOB,
        p_creator executor_t,
        p_executor executor_t,
        p_status VARCHAR2,
        p_priority NUMBER,
        p_start_date DATE,
        p_due_date DATE,
        p_created_at DATE
    ) RETURN SELF AS RESULT
);
/

CREATE OR REPLACE TYPE BODY assignment_t AS
    CONSTRUCTOR FUNCTION assignment_t (
        p_assignment_id NUMBER,
        p_title VARCHAR2,
        p_description CLOB,
        p_creator executor_t,
        p_executor executor_t,
        p_status VARCHAR2,
        p_priority NUMBER,
        p_start_date DATE,
        p_due_date DATE,
        p_created_at DATE
    ) RETURN SELF AS RESULT IS
    BEGIN
        SELF.assignment_id := p_assignment_id;
        SELF.title := p_title;
        SELF.description := p_description;
        SELF.creator := p_creator;
        SELF.executor := p_executor;
        SELF.status := NVL(p_status, 'New');
        SELF.priority := NVL(p_priority, 1);
        SELF.start_date := p_start_date;
        SELF.due_date := p_due_date;
        SELF.created_at := NVL(p_created_at, SYSDATE);
        RETURN;
    END;

    MAP MEMBER FUNCTION assignment_map RETURN NUMBER DETERMINISTIC IS
    BEGIN
        RETURN NVL(assignment_id, 0);
    END;

    MEMBER FUNCTION days_to_due RETURN NUMBER IS
    BEGIN
        IF due_date IS NULL THEN
            RETURN NULL;
        END IF;
        RETURN TRUNC(due_date) - TRUNC(SYSDATE);
    END;

    MEMBER PROCEDURE escalate_priority(p_increment NUMBER) IS
    BEGIN
        priority := NVL(priority, 1) + NVL(p_increment, 0);
    END;
END;
/






-- 3
MERGE INTO Role tgt
USING (SELECT 'Manager' role_name, 'Manages assignment execution' description FROM dual) src
ON (tgt.role_name = src.role_name)
WHEN NOT MATCHED THEN INSERT (role_id, role_name, description)
VALUES (seq_role.NEXTVAL, src.role_name, src.description);

MERGE INTO Role tgt
USING (SELECT 'Executor' role_name, 'Performs assigned tasks' description FROM dual) src
ON (tgt.role_name = src.role_name)
WHEN NOT MATCHED THEN INSERT (role_id, role_name, description)
VALUES (seq_role.NEXTVAL, src.role_name, src.description);

MERGE INTO Department tgt
USING (SELECT 'Operations' department_name FROM dual) src
ON (tgt.department_name = src.department_name)
WHEN NOT MATCHED THEN INSERT (department_id, department_name, parent_department_id)
VALUES (seq_department.NEXTVAL, src.department_name, NULL);

MERGE INTO Department tgt
USING (SELECT 'Quality Control' department_name, 'Operations' parent FROM dual) src
ON (tgt.department_name = src.department_name)
WHEN NOT MATCHED THEN INSERT (department_id, department_name, parent_department_id)
VALUES (
    seq_department.NEXTVAL,
    src.department_name,
    (SELECT department_id FROM Department WHERE department_name = src.parent)
);

MERGE INTO Status tgt
USING (SELECT 'New' status_name FROM dual) src
ON (tgt.status_name = src.status_name)
WHEN NOT MATCHED THEN INSERT (status_id, status_name)
VALUES (seq_status.NEXTVAL, src.status_name);

MERGE INTO Status tgt
USING (SELECT 'In Progress' status_name FROM dual) src
ON (tgt.status_name = src.status_name)
WHEN NOT MATCHED THEN INSERT (status_id, status_name)
VALUES (seq_status.NEXTVAL, src.status_name);

MERGE INTO Status tgt
USING (SELECT 'Completed' status_name FROM dual) src
ON (tgt.status_name = src.status_name)
WHEN NOT MATCHED THEN INSERT (status_id, status_name)
VALUES (seq_status.NEXTVAL, src.status_name);

MERGE INTO Users tgt
USING (
    SELECT 'ivan.k@company.local' email,
           'Ivan Klimov' full_name,
           'Pass#123' password,
           (SELECT role_id FROM Role WHERE role_name = 'Manager') role_id,
           (SELECT department_id FROM Department WHERE department_name = 'Operations') department_id
    FROM dual
) src
ON (tgt.email = src.email)
WHEN NOT MATCHED THEN INSERT (user_id, full_name, email, password, role_id, department_id)
VALUES (
    seq_user.NEXTVAL,
    src.full_name,
    src.email,
    src.password,
    src.role_id,
    src.department_id
);

MERGE INTO Users tgt
USING (
    SELECT 'olga.s@company.local' email,
           'Olga Sidorova' full_name,
           'Exec#321' password,
           (SELECT role_id FROM Role WHERE role_name = 'Executor') role_id,
           (SELECT department_id FROM Department WHERE department_name = 'Quality Control') department_id
    FROM dual
) src
ON (tgt.email = src.email)
WHEN NOT MATCHED THEN INSERT (user_id, full_name, email, password, role_id, department_id)
VALUES (
    seq_user.NEXTVAL,
    src.full_name,
    src.email,
    src.password,
    src.role_id,
    src.department_id
);

MERGE INTO Task tgt
USING (
    SELECT 'Reporting module requirements check' title,
           TO_CLOB('Collect and compare assignment lists from project documentation with actual execution status') description,
           (SELECT user_id FROM Users WHERE email = 'ivan.k@company.local') creator_id,
           (SELECT user_id FROM Users WHERE email = 'olga.s@company.local') executor_id,
           (SELECT status_id FROM Status WHERE status_name = 'In Progress') status_id,
           3 priority,
           DATE '2026-03-28' start_date,
           DATE '2026-04-15' due_date
    FROM dual
) src
ON (tgt.title = src.title)
WHEN NOT MATCHED THEN INSERT (
    task_id, title, description, creator_id, executor_id, status_id, priority, start_date, due_date, created_at
)
VALUES (
    seq_task.NEXTVAL,
    src.title,
    src.description,
    src.creator_id,
    src.executor_id,
    src.status_id,
    src.priority,
    src.start_date,
    src.due_date,
    SYSDATE
);

MERGE INTO Task tgt
USING (
    SELECT 'Access assignment execution control' title,
           TO_CLOB('Review daily reports for access control work and record deviations') description,
           (SELECT user_id FROM Users WHERE email = 'ivan.k@company.local') creator_id,
           (SELECT user_id FROM Users WHERE email = 'olga.s@company.local') executor_id,
           (SELECT status_id FROM Status WHERE status_name = 'New') status_id,
           2 priority,
           DATE '2026-04-01' start_date,
           DATE '2026-04-22' due_date
    FROM dual
) src
ON (tgt.title = src.title)
WHEN NOT MATCHED THEN INSERT (
    task_id, title, description, creator_id, executor_id, status_id, priority, start_date, due_date, created_at
)
VALUES (
    seq_task.NEXTVAL,
    src.title,
    src.description,
    src.creator_id,
    src.executor_id,
    src.status_id,
    src.priority,
    src.start_date,
    src.due_date,
    SYSDATE
);

COMMIT;








--
CREATE TABLE executors_obj OF executor_t (
    PRIMARY KEY (user_id)
) OBJECT IDENTIFIER IS PRIMARY KEY;

CREATE TABLE assignments_obj OF assignment_t (
    PRIMARY KEY (assignment_id)
) OBJECT IDENTIFIER IS PRIMARY KEY;

INSERT INTO executors_obj
SELECT executor_t(
    u.user_id,
    u.full_name,
    u.email,
    u.role_id,
    u.department_id
)
FROM Users u
WHERE NOT EXISTS (
    SELECT 1
    FROM executors_obj eo
    WHERE eo.user_id = u.user_id
);

INSERT INTO assignments_obj
SELECT assignment_t(
    t.task_id,
    t.title,
    t.description,
    executor_t(c.user_id, c.full_name, c.email, c.role_id, c.department_id),
    executor_t(e.user_id, e.full_name, e.email, e.role_id, e.department_id),
    s.status_name,
    t.priority,
    t.start_date,
    t.due_date,
    t.created_at
)
FROM Task t
JOIN Users c ON t.creator_id = c.user_id
JOIN Users e ON t.executor_id = e.user_id
JOIN Status s ON t.status_id = s.status_id
WHERE NOT EXISTS (
    SELECT 1
    FROM assignments_obj ao
    WHERE ao.assignment_id = t.task_id
);

COMMIT;




-- 4
CREATE OR REPLACE VIEW vw_assignment_summary AS
SELECT
    a.assignment_id,
    a.title,
    a.status,
    a.priority,
    a.executor.full_name AS executor_name,
    a.creator.full_name AS creator_name,
    a.days_to_due() AS days_left,
    a.executor.contact_summary() AS executor_contact
FROM assignments_obj a;





-- 5
CREATE INDEX idx_assignment_due ON assignments_obj(due_date);
CREATE INDEX idx_assignment_map ON assignments_obj((SYS_NC_ROWINFO$.assignment_map()));

COMMIT;




-- 6
SELECT
    assignment_id,
    title,
    status,
    days_left,
    executor_contact
FROM vw_assignment_summary
ORDER BY assignment_id;

SELECT
    a.assignment_id,
    a.title,
    a.days_to_due() AS days_to_due
FROM assignments_obj a
ORDER BY a.assignment_id;

DECLARE
    v_assignment assignment_t;
BEGIN
    SELECT VALUE(a)
    INTO v_assignment
    FROM assignments_obj a
    WHERE a.assignment_id = (SELECT MIN(assignment_id) FROM assignments_obj);

    DBMS_OUTPUT.PUT_LINE('Priority before: ' || NVL(v_assignment.priority, 0));
    v_assignment.escalate_priority(1);
    DBMS_OUTPUT.PUT_LINE('Priority after:  ' || NVL(v_assignment.priority, 0));
END;
/

DECLARE
    v_executor executor_t;
BEGIN
    SELECT VALUE(e)
    INTO v_executor
    FROM executors_obj e
    WHERE e.user_id = (SELECT MIN(user_id) FROM executors_obj);

    DBMS_OUTPUT.PUT_LINE('Email before: ' || v_executor.email);
    v_executor.anonymize_email;
    DBMS_OUTPUT.PUT_LINE('Email after:  ' || v_executor.email);
END;
/

SELECT index_name, table_name, index_type
FROM user_indexes
WHERE table_name = 'ASSIGNMENTS_OBJ'
ORDER BY index_name;

SELECT index_name, column_position, column_expression
FROM user_ind_expressions
WHERE index_name = 'IDX_ASSIGNMENT_MAP'
ORDER BY column_position;
