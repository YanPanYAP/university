USE TaskControlDB;

DECLARE @completed_status_id INT = (
    SELECT status_id
    FROM dbo.Status
    WHERE status_name = N'Completed'
);

IF NOT EXISTS (
    SELECT 1
    FROM dbo.Task
    WHERE title LIKE N'Lab7 pattern e%: task %'
)
BEGIN
    ;WITH employees AS (
        SELECT 3 AS executor_id UNION ALL
        SELECT 4 UNION ALL
        SELECT 5 UNION ALL
        SELECT 6 UNION ALL
        SELECT 7 UNION ALL
        SELECT 8 UNION ALL
        SELECT 9 UNION ALL
        SELECT 10 UNION ALL
        SELECT 11 UNION ALL
        SELECT 12
    ),
    pattern_tasks AS (
        SELECT 1 AS task_no, CAST('2026-01-10' AS DATETIME) AS completed_at UNION ALL
        SELECT 2, CAST('2026-01-12' AS DATETIME) UNION ALL
        SELECT 3, CAST('2026-01-18' AS DATETIME) UNION ALL
        SELECT 4, CAST('2026-01-24' AS DATETIME) UNION ALL
        SELECT 5, CAST('2026-02-08' AS DATETIME) UNION ALL
        SELECT 6, CAST('2026-02-21' AS DATETIME) UNION ALL
        SELECT 7, CAST('2026-03-03' AS DATETIME) UNION ALL
        SELECT 8, CAST('2026-03-07' AS DATETIME) UNION ALL
        SELECT 9, CAST('2026-03-12' AS DATETIME) UNION ALL
        SELECT 10, CAST('2026-03-18' AS DATETIME) UNION ALL
        SELECT 11, CAST('2026-03-25' AS DATETIME) UNION ALL
        SELECT 12, CAST('2026-04-05' AS DATETIME) UNION ALL
        SELECT 13, CAST('2026-04-14' AS DATETIME) UNION ALL
        SELECT 14, CAST('2026-04-22' AS DATETIME)
    )
    INSERT INTO dbo.Task (
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
    SELECT
        N'Lab7 pattern e' + CAST(e.executor_id AS NVARCHAR(10)) + N': task ' + CAST(p.task_no AS NVARCHAR(10)),
        N'Task for lab 7 pattern.',
        1,
        e.executor_id,
        @completed_status_id,
        2,
        CAST(DATEADD(DAY, -5, p.completed_at) AS DATE),
        CAST(p.completed_at AS DATE),
        DATEADD(DAY, -5, p.completed_at),
        N'Backend'
    FROM employees e
    CROSS JOIN pattern_tasks p;

    INSERT INTO dbo.TaskHistory (
        task_id,
        old_status_id,
        new_status_id,
        changed_by,
        changed_at
    )
    SELECT
        t.task_id,
        NULL,
        @completed_status_id,
        t.executor_id,
        DATEADD(HOUR, 10, CAST(t.due_date AS DATETIME))
    FROM dbo.Task t
    WHERE t.title LIKE N'Lab7 pattern e%: task %';
END;
GO









------------------------ 1
;WITH base_year AS (
    SELECT MAX(YEAR(th.changed_at)) AS year_num
    FROM dbo.TaskHistory th
    JOIN dbo.Status s ON s.status_id = th.new_status_id
    WHERE s.status_name = N'Completed'
),
completed_by_employee AS (
    SELECT
        t.executor_id,
        COUNT(DISTINCT t.task_id) AS completed_count
    FROM dbo.Task t
    JOIN dbo.TaskHistory th ON th.task_id = t.task_id
    JOIN dbo.Status s ON s.status_id = th.new_status_id
    CROSS JOIN base_year y
    WHERE s.status_name = N'Completed'
      AND YEAR(th.changed_at) = y.year_num
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
        ISNULL(c.completed_count, 0) AS completed_count,
        CAST(
            ROUND(
                CASE
                    WHEN SUM(ISNULL(c.completed_count, 0)) OVER () = 0 THEN 0
                    ELSE ISNULL(c.completed_count, 0) * 100.0
                         / SUM(ISNULL(c.completed_count, 0)) OVER ()
                END,
                2
            ) AS DECIMAL(10, 2)
        ) AS performance_pct
    FROM dbo.[User] u
    JOIN dbo.Role r ON r.role_id = u.role_id
    CROSS JOIN base_year y
    LEFT JOIN completed_by_employee c ON c.executor_id = u.user_id
),
salary_plan AS (
    SELECT
        user_id,
        full_name,
        plan_year,
        1 AS month_no,
        base_salary,
        performance_pct,
        CAST(ROUND(base_salary * (1 + performance_pct / 1000.0), 2) AS DECIMAL(12, 2)) AS planned_salary
    FROM employee_stats

    UNION ALL

    SELECT
        user_id,
        full_name,
        plan_year,
        month_no + 1,
        base_salary,
        performance_pct,
        CAST(ROUND(planned_salary * (1 + performance_pct / 1000.0), 2) AS DECIMAL(12, 2)) AS planned_salary
    FROM salary_plan
    WHERE month_no < 12
)
SELECT
    user_id,
    full_name,
    plan_year,
    month_no,
    performance_pct,
    planned_salary
FROM salary_plan
ORDER BY full_name, month_no
OPTION (MAXRECURSION 12);










------------------------ 2
;WITH month_bounds AS (
    SELECT
        DATEFROMPARTS(YEAR(MIN(th.changed_at)), MONTH(MIN(th.changed_at)), 1) AS min_month,
        DATEFROMPARTS(YEAR(MAX(th.changed_at)), MONTH(MAX(th.changed_at)), 1) AS max_month
    FROM dbo.TaskHistory th
    JOIN dbo.Status s ON s.status_id = th.new_status_id
    WHERE s.status_name = N'Completed'
),
months AS (
    SELECT min_month AS month_start, max_month
    FROM month_bounds

    UNION ALL

    SELECT DATEADD(MONTH, 1, month_start), max_month
    FROM months
    WHERE month_start < max_month
),
completed_by_month AS (
    SELECT
        t.executor_id,
        DATEFROMPARTS(YEAR(th.changed_at), MONTH(th.changed_at), 1) AS month_start,
        COUNT(DISTINCT t.task_id) AS completed_count
    FROM dbo.Task t
    JOIN dbo.TaskHistory th ON th.task_id = t.task_id
    JOIN dbo.Status s ON s.status_id = th.new_status_id
    WHERE s.status_name = N'Completed'
    GROUP BY
        t.executor_id,
        DATEFROMPARTS(YEAR(th.changed_at), MONTH(th.changed_at), 1)
),
pattern_rows AS (
    SELECT
        u.user_id,
        u.full_name,
        month_start AS start_month,
        LEAD(month_start, 1) OVER (PARTITION BY u.user_id ORDER BY month_start) AS fall_month,
        LEAD(month_start, 2) OVER (PARTITION BY u.user_id ORDER BY month_start) AS rise_month,
        LEAD(month_start, 3) OVER (PARTITION BY u.user_id ORDER BY month_start) AS second_fall_month,
        completed_count AS start_count,
        LEAD(completed_count, 1) OVER (PARTITION BY u.user_id ORDER BY month_start) AS fall_count,
        LEAD(completed_count, 2) OVER (PARTITION BY u.user_id ORDER BY month_start) AS rise_count,
        LEAD(completed_count, 3) OVER (PARTITION BY u.user_id ORDER BY month_start) AS second_fall_count
    FROM completed_by_month c
    JOIN dbo.[User] u ON u.user_id = c.executor_id
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
FROM pattern_rows
WHERE fall_count < start_count
  AND rise_count > fall_count
  AND second_fall_count < rise_count
ORDER BY full_name, start_month
OPTION (MAXRECURSION 500);
