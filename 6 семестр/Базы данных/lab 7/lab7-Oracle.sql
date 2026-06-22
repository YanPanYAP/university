DECLARE
    v_exists NUMBER := 0;
    v_status_completed NUMBER;
BEGIN
    SELECT COUNT(*)
    INTO v_exists
    FROM Task
    WHERE title LIKE 'Lab7 pattern e%: task %';

    IF v_exists = 0 THEN
        SELECT status_id
        INTO v_status_completed
        FROM Status
        WHERE LOWER(status_name) = 'completed';

        FOR employee_id IN 3 .. 12 LOOP
            FOR i IN 1 .. 14 LOOP
                INSERT INTO Task (
                    task_id,
                    title,
                    description,
                    creator_id,
                    executor_id,
                    status_id,
                    priority,
                    start_date,
                    due_date,
                    created_at,
                    task_type
                )
                VALUES (
                    seq_task.NEXTVAL,
                    'Lab7 pattern e' || employee_id || ': task ' || i,
                    'Task for lab 7 pattern.',
                    1,
                    employee_id,
                    v_status_completed,
                    2,
                    CASE
                        WHEN i <= 4 THEN DATE '2026-01-05' + (i - 1) * 4
                        WHEN i <= 6 THEN DATE '2026-02-05' + (i - 5) * 10
                        WHEN i <= 11 THEN DATE '2026-03-03' + (i - 7) * 5
                        ELSE DATE '2026-04-05' + (i - 12) * 9
                    END - 5,
                    CASE
                        WHEN i <= 4 THEN DATE '2026-01-10' + (i - 1) * 4
                        WHEN i <= 6 THEN DATE '2026-02-08' + (i - 5) * 13
                        WHEN i <= 11 THEN DATE '2026-03-03' + (i - 7) * 5
                        ELSE DATE '2026-04-05' + (i - 12) * 9
                    END,
                    CASE
                        WHEN i <= 4 THEN DATE '2026-01-10' + (i - 1) * 4
                        WHEN i <= 6 THEN DATE '2026-02-08' + (i - 5) * 13
                        WHEN i <= 11 THEN DATE '2026-03-03' + (i - 7) * 5
                        ELSE DATE '2026-04-05' + (i - 12) * 9
                    END - 5,
                    'Backend'
                );
            END LOOP;
        END LOOP;

        INSERT INTO TaskHistory (
            history_id,
            task_id,
            old_status_id,
            new_status_id,
            changed_by,
            changed_at
        )
        SELECT
            seq_history.NEXTVAL,
            task_id,
            NULL,
            v_status_completed,
            executor_id,
            due_date + (10 / 24)
        FROM Task
        WHERE title LIKE 'Lab7 pattern e%: task %';
    END IF;
END;
/










------------------------ 1
WITH base_year AS (
    SELECT MAX(EXTRACT(YEAR FROM th.changed_at)) AS year_num
    FROM TaskHistory th
    JOIN Status s ON s.status_id = th.new_status_id
    WHERE LOWER(s.status_name) = 'completed'
),
completed_by_employee AS (
    SELECT
        t.executor_id,
        COUNT(DISTINCT t.task_id) AS completed_count
    FROM Task t
    JOIN TaskHistory th ON th.task_id = t.task_id
    JOIN Status s ON s.status_id = th.new_status_id
    CROSS JOIN base_year y
    WHERE LOWER(s.status_name) = 'completed'
      AND EXTRACT(YEAR FROM th.changed_at) = y.year_num
    GROUP BY t.executor_id
),
employee_stats AS (
    SELECT
        u.user_id,
        u.full_name,
        y.year_num + 1 AS plan_year,
        CASE
            WHEN LOWER(r.role_name) = 'administrator' THEN 3000
            WHEN LOWER(r.role_name) = 'manager' THEN 2400
            ELSE 1800
        END AS base_salary,
        NVL(c.completed_count, 0) AS completed_count,
        ROUND(
            CASE
                WHEN SUM(NVL(c.completed_count, 0)) OVER () = 0 THEN 0
                ELSE NVL(c.completed_count, 0) * 100
                     / SUM(NVL(c.completed_count, 0)) OVER ()
            END,
            2
        ) AS performance_pct
    FROM Users u
    JOIN Role r ON r.role_id = u.role_id
    CROSS JOIN base_year y
    LEFT JOIN completed_by_employee c ON c.executor_id = u.user_id
),
month_grid AS (
    SELECT
        e.user_id,
        e.full_name,
        e.plan_year,
        m.month_no,
        e.base_salary,
        e.performance_pct
    FROM employee_stats e
    CROSS JOIN (
        SELECT LEVEL AS month_no
        FROM dual
        CONNECT BY LEVEL <= 12
    ) m
)
SELECT
    user_id,
    full_name,
    plan_year,
    month_no,
    performance_pct,
    planned_salary
FROM month_grid
MODEL
    PARTITION BY (user_id, full_name, plan_year)
    DIMENSION BY (month_no)
    MEASURES (
        base_salary,
        performance_pct,
        CAST(0 AS NUMBER(12, 2)) AS planned_salary
    )
    RULES SEQUENTIAL ORDER (
        planned_salary[1] =
            ROUND(base_salary[1] * (1 + performance_pct[1] / 1000), 2),
        planned_salary[FOR month_no FROM 2 TO 12 INCREMENT 1] =
            ROUND(planned_salary[CV(month_no) - 1] * (1 + performance_pct[1] / 1000), 2)
    )
ORDER BY full_name, month_no;









------------------------ 2
WITH month_bounds AS (
    SELECT
        MIN(TRUNC(th.changed_at, 'MM')) AS min_month,
        MAX(TRUNC(th.changed_at, 'MM')) AS max_month
    FROM TaskHistory th
    JOIN Status s ON s.status_id = th.new_status_id
    WHERE LOWER(s.status_name) = 'completed'
),
months AS (
    SELECT ADD_MONTHS(b.min_month, LEVEL - 1) AS month_start
    FROM month_bounds b
    CONNECT BY ADD_MONTHS(b.min_month, LEVEL - 1) <= b.max_month
),
completed_by_month AS (
    SELECT
        t.executor_id,
        TRUNC(th.changed_at, 'MM') AS month_start,
        COUNT(DISTINCT t.task_id) AS completed_count
    FROM Task t
    JOIN TaskHistory th ON th.task_id = t.task_id
    JOIN Status s ON s.status_id = th.new_status_id
    WHERE LOWER(s.status_name) = 'completed'
    GROUP BY
        t.executor_id,
        TRUNC(th.changed_at, 'MM')
),
employee_months AS (
    SELECT
        u.user_id,
        u.full_name,
        c.month_start,
        c.completed_count
    FROM completed_by_month c
    JOIN Users u ON u.user_id = c.executor_id
)
SELECT
    user_id,
    full_name,
    start_month,
    fall_month,
    rise_month,
    second_fall_month,
    start_count,
    fall_count,
    rise_count,
    second_fall_count
FROM employee_months
MATCH_RECOGNIZE (
    PARTITION BY user_id, full_name
    ORDER BY month_start
    MEASURES
        A.month_start AS start_month,
        B.month_start AS fall_month,
        C.month_start AS rise_month,
        D.month_start AS second_fall_month,
        A.completed_count AS start_count,
        B.completed_count AS fall_count,
        C.completed_count AS rise_count,
        D.completed_count AS second_fall_count
    ONE ROW PER MATCH
    PATTERN (A B C D)
    DEFINE
        B AS B.completed_count < PREV(B.completed_count),
        C AS C.completed_count > PREV(C.completed_count),
        D AS D.completed_count < PREV(D.completed_count)
)
ORDER BY full_name, start_month;
