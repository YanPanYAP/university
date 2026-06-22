SET SERVEROUTPUT ON;

-- 2
CREATE OR REPLACE PROCEDURE pr_get_sub_departments (
    p_parent_id IN NUMBER
)
AS
    v_exists NUMBER;
BEGIN
    SELECT COUNT(*)
    INTO v_exists
    FROM Department
    WHERE department_id = p_parent_id;

    IF v_exists = 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'Указанный родительский узел не найден.');
    END IF;

    FOR rec IN (
        SELECT
            department_id,
            department_name,
            parent_department_id,
            LEVEL - 1 AS hierarchy_level
        FROM Department
        START WITH department_id = p_parent_id
        CONNECT BY PRIOR department_id = parent_department_id
        ORDER SIBLINGS BY department_id
    )
    LOOP
        IF rec.department_id <> p_parent_id THEN
            DBMS_OUTPUT.PUT_LINE(
                'id=' || rec.department_id ||
                ', name=' || rec.department_name ||
                ', parent_id=' || rec.parent_department_id ||
                ', level=' || rec.hierarchy_level
            );
        END IF;
    END LOOP;
END;
/

-- 3
CREATE OR REPLACE PROCEDURE pr_add_sub_department (
    p_parent_id IN NUMBER,
    p_name IN VARCHAR2
)
AS
    v_exists NUMBER;
BEGIN
    SELECT COUNT(*)
    INTO v_exists
    FROM Department
    WHERE department_id = p_parent_id;

    IF v_exists = 0 THEN
        RAISE_APPLICATION_ERROR(-20002, 'Родительский узел для добавления не найден.');
    END IF;

    INSERT INTO Department (
        department_id,
        department_name,
        parent_department_id
    )
    VALUES (
        seq_department.NEXTVAL,
        p_name,
        p_parent_id
    );
END;
/

-- 4
CREATE OR REPLACE PROCEDURE pr_move_sub_departments (
    p_old_parent IN NUMBER,
    p_new_parent IN NUMBER
)
AS
    v_old_exists NUMBER;
    v_new_exists NUMBER;
    v_is_descendant NUMBER;
BEGIN
    SELECT COUNT(*)
    INTO v_old_exists
    FROM Department
    WHERE department_id = p_old_parent;

    IF v_old_exists = 0 THEN
        RAISE_APPLICATION_ERROR(-20003, 'Исходный родительский узел не найден.');
    END IF;

    SELECT COUNT(*)
    INTO v_new_exists
    FROM Department
    WHERE department_id = p_new_parent;

    IF v_new_exists = 0 THEN
        RAISE_APPLICATION_ERROR(-20004, 'Новый родительский узел не найден.');
    END IF;

    IF p_old_parent = p_new_parent THEN
        RAISE_APPLICATION_ERROR(-20005, 'Исходный и новый родитель совпадают.');
    END IF;

    SELECT COUNT(*)
    INTO v_is_descendant
    FROM (
        SELECT department_id
        FROM Department
        START WITH department_id = p_old_parent
        CONNECT BY PRIOR department_id = parent_department_id
    )
    WHERE department_id = p_new_parent;

    IF v_is_descendant > 0 THEN
        RAISE_APPLICATION_ERROR(-20006, 'Нельзя переносить подчиненных в собственную ветвь.');
    END IF;

    UPDATE Department
    SET parent_department_id = p_new_parent
    WHERE parent_department_id = p_old_parent;

    DBMS_OUTPUT.PUT_LINE('Перенесено подчиненных: ' || SQL%ROWCOUNT);
END;
/

--
BEGIN
    pr_get_sub_departments(1);
END;
/

BEGIN
    pr_add_sub_department(2, 'TEAM');
END;
/

SELECT
    department_id,
    department_name,
    parent_department_id
FROM Department
WHERE parent_department_id = 2
ORDER BY department_id;

BEGIN
    pr_move_sub_departments(3, 13);
END;
/

SELECT
    department_id,
    department_name,
    parent_department_id
FROM Department
ORDER BY NVL(parent_department_id, 0), department_id;
