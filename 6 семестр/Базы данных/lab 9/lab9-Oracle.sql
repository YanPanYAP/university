SET DEFINE OFF;
SET SERVEROUTPUT ON;

-- 1
BEGIN
    EXECUTE IMMEDIATE 'DROP TYPE k1_executor_tasks_ntt FORCE';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE <> -4043 THEN
            RAISE;
        END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'DROP TYPE k1_executor_tasks_t FORCE';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE <> -4043 THEN
            RAISE;
        END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'DROP TYPE k2_assignment_ntt FORCE';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE <> -4043 THEN
            RAISE;
        END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'DROP TYPE assignment_title_varray_t FORCE';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE <> -4043 THEN
            RAISE;
        END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'DROP TYPE assignment_id_ntt FORCE';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE <> -4043 THEN
            RAISE;
        END IF;
END;
/

-- 2a
CREATE OR REPLACE TYPE k2_assignment_ntt AS TABLE OF assignment_t;
/

-- 
CREATE OR REPLACE TYPE k1_executor_tasks_t AS OBJECT (
    executor_obj executor_t,
    k2_assignments k2_assignment_ntt,
    MAP MEMBER FUNCTION k1_map RETURN NUMBER
);
/

CREATE OR REPLACE TYPE BODY k1_executor_tasks_t AS
    MAP MEMBER FUNCTION k1_map RETURN NUMBER IS
    BEGIN
        RETURN NVL(executor_obj.user_id, -1);
    END;
END;
/

CREATE OR REPLACE TYPE k1_executor_tasks_ntt AS TABLE OF k1_executor_tasks_t;
/

--
CREATE OR REPLACE TYPE assignment_title_varray_t AS VARRAY(1000) OF VARCHAR2(255);
/

CREATE OR REPLACE TYPE assignment_id_ntt AS TABLE OF NUMBER;
/

--
DECLARE
    v_k1 k1_executor_tasks_ntt;
    v_probe_user_id NUMBER := 1;
    v_probe k1_executor_tasks_t;
    v_found_text VARCHAR2(30);
BEGIN
    SELECT CAST(
        MULTISET(
            SELECT k1_executor_tasks_t(
                VALUE(e),
                CAST(
                    MULTISET(
                        SELECT VALUE(a)
                        FROM assignments_obj a
                        WHERE a.executor.user_id = e.user_id
                    ) AS k2_assignment_ntt
                )
            )
            FROM executors_obj e
            ORDER BY e.user_id
        ) AS k1_executor_tasks_ntt
    )
    INTO v_k1
    FROM dual;

    DBMS_OUTPUT.PUT_LINE('K1 elements: ' || v_k1.COUNT);

    -- 2b
    BEGIN
        SELECT k1_executor_tasks_t(
                   VALUE(e),
                   CAST(
                       MULTISET(
                           SELECT VALUE(a)
                           FROM assignments_obj a
                           WHERE a.executor.user_id = e.user_id
                       ) AS k2_assignment_ntt
                   )
               )
        INTO v_probe
        FROM executors_obj e
        WHERE e.user_id = v_probe_user_id;

        IF v_probe MEMBER OF v_k1 THEN
            v_found_text := 'YES';
        ELSE
            v_found_text := 'NO';
        END IF;

        DBMS_OUTPUT.PUT_LINE('Element user_id=' || v_probe_user_id || ' is a member of K1: ' || v_found_text);
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.PUT_LINE('Element user_id=' || v_probe_user_id || ' is absent in t1 (executors_obj).');
    END;

    -- 2c
    DBMS_OUTPUT.PUT_LINE('Executors with empty K2:');
    FOR rec IN (
        SELECT
            k.executor_obj.user_id AS user_id,
            k.executor_obj.full_name AS full_name
        FROM TABLE(v_k1) k
        WHERE CARDINALITY(k.k2_assignments) = 0
        ORDER BY k.executor_obj.user_id
    ) LOOP
        DBMS_OUTPUT.PUT_LINE('  user_id=' || rec.user_id || ', name=' || rec.full_name);
    END LOOP;
END;
/


-- 3.1
DECLARE
    v_titles assignment_title_varray_t;
BEGIN
    SELECT CAST(
        MULTISET(
            SELECT a.title
            FROM assignments_obj a
            ORDER BY a.assignment_id
            FETCH FIRST 20 ROWS ONLY
        ) AS assignment_title_varray_t
    )
    INTO v_titles
    FROM dual;

    DBMS_OUTPUT.PUT_LINE('VARRAY elements: ' || v_titles.COUNT);
    IF v_titles.COUNT > 0 THEN
        DBMS_OUTPUT.PUT_LINE('First title: ' || v_titles(1));
    END IF;
END;
/

-- 3.2
WITH k1_data AS (
    SELECT CAST(
        MULTISET(
            SELECT k1_executor_tasks_t(
                VALUE(e),
                CAST(
                    MULTISET(
                        SELECT VALUE(a)
                        FROM assignments_obj a
                        WHERE a.executor.user_id = e.user_id
                    ) AS k2_assignment_ntt
                )
            )
            FROM executors_obj e
        ) AS k1_executor_tasks_ntt
    ) AS k1
    FROM dual
)
SELECT
    k.executor_obj.user_id AS executor_id,
    k.executor_obj.full_name AS executor_name,
    CARDINALITY(k.k2_assignments) AS assignments_count
FROM k1_data d
CROSS JOIN TABLE(d.k1) k
ORDER BY executor_id;

--4
DECLARE
    v_assignment_ids assignment_id_ntt;
BEGIN
    SELECT a.assignment_id
    BULK COLLECT INTO v_assignment_ids
    FROM assignments_obj a
    WHERE a.assignment_id IS NOT NULL
    FETCH FIRST 100 ROWS ONLY;

    DBMS_OUTPUT.PUT_LINE('BULK COLLECT elements: ' || v_assignment_ids.COUNT);

    FORALL i IN INDICES OF v_assignment_ids
        UPDATE Task t
        SET t.priority = t.priority
        WHERE t.task_id = v_assignment_ids(i);

    DBMS_OUTPUT.PUT_LINE('FORALL processed rows: ' || SQL%ROWCOUNT);
END;
/



