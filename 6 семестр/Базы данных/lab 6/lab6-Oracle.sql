BEGIN
    EXECUTE IMMEDIATE 'ALTER TABLE Task ADD (task_type VARCHAR2(100))';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE != -1430 THEN
            RAISE;
        END IF;
END;
/





------------------------ 3
WITH completed_tasks AS (
    SELECT
        th.task_id,
        MIN(th.changed_at) AS completed_at
    FROM TaskHistory th
    JOIN Status s ON s.status_id = th.new_status_id
    WHERE LOWER(s.status_name) = 'completed'
    GROUP BY th.task_id
),
periodized AS (
    SELECT
        EXTRACT(YEAR FROM completed_at) AS year_num,
        EXTRACT(MONTH FROM completed_at) AS month_num,
        CEIL(EXTRACT(MONTH FROM completed_at) / 3) AS quarter_num,
        CASE
            WHEN EXTRACT(MONTH FROM completed_at) <= 6 THEN 1
            ELSE 2
        END AS half_year_num
    FROM completed_tasks
)
SELECT
    CASE
        WHEN GROUPING(year_num) = 0 AND GROUPING(month_num) = 0 THEN 'Месяц'
        WHEN GROUPING(year_num) = 0 AND GROUPING(quarter_num) = 0 THEN 'Квартал'
        WHEN GROUPING(year_num) = 0 AND GROUPING(half_year_num) = 0 THEN 'Полугодие'
        ELSE 'Год'
    END AS period_type,
    CASE
        WHEN GROUPING(year_num) = 0 AND GROUPING(month_num) = 0
            THEN TO_CHAR(year_num) || '-' || LPAD(TO_CHAR(month_num), 2, '0')
        WHEN GROUPING(year_num) = 0 AND GROUPING(quarter_num) = 0
            THEN TO_CHAR(year_num) || ' Q' || TO_CHAR(quarter_num)
        WHEN GROUPING(year_num) = 0 AND GROUPING(half_year_num) = 0
            THEN TO_CHAR(year_num) || ' H' || TO_CHAR(half_year_num)
        ELSE TO_CHAR(year_num)
    END AS period_label,
    COUNT(*) AS completed_tasks_count
FROM periodized
GROUP BY GROUPING SETS (
    (year_num, month_num),
    (year_num, quarter_num),
    (year_num, half_year_num),
    (year_num)
)
ORDER BY
    year_num,
    CASE
        WHEN GROUPING(month_num) = 0 THEN month_num
        WHEN GROUPING(quarter_num) = 0 THEN quarter_num * 10
        WHEN GROUPING(half_year_num) = 0 THEN half_year_num * 100
        ELSE 999
    END,
    period_type;







------------------------ 4
WITH params AS (
    SELECT
        DATE '2025-10-01' AS period_start,
        DATE '2025-12-31' AS period_end
    FROM dual
),
completed_in_period AS (
    SELECT
        COUNT(DISTINCT th.task_id) AS completed_in_period_count
    FROM TaskHistory th
    JOIN Status s ON s.status_id = th.new_status_id
    CROSS JOIN params p
    WHERE LOWER(s.status_name) = 'completed'
      AND TRUNC(th.changed_at) BETWEEN p.period_start AND p.period_end
),
completed_total AS (
    SELECT
        COUNT(DISTINCT th.task_id) AS completed_total_count
    FROM TaskHistory th
    JOIN Status s ON s.status_id = th.new_status_id
    WHERE LOWER(s.status_name) = 'completed'
),
not_completed_total AS (
    SELECT
        COUNT(*) AS not_completed_total_count
    FROM Task t
    WHERE NOT EXISTS (
        SELECT 1
        FROM TaskHistory th
        JOIN Status s ON s.status_id = th.new_status_id
        WHERE th.task_id = t.task_id
          AND LOWER(s.status_name) = 'completed'
    )
)
SELECT
    p.period_start,
    p.period_end,
    cip.completed_in_period_count AS completed_tasks_count,
    ROUND(cip.completed_in_period_count * 100 / NULLIF(ct.completed_total_count, 0), 2) AS percent_of_all_completed,
    ROUND(cip.completed_in_period_count * 100 / NULLIF(nct.not_completed_total_count, 0), 2) AS percent_of_all_not_completed
FROM params p
CROSS JOIN completed_in_period cip
CROSS JOIN completed_total ct
CROSS JOIN not_completed_total nct;






------------------------ 5
WITH months AS (
    SELECT ADD_MONTHS(TRUNC(SYSDATE, 'MM'), -6 + LEVEL) AS month_start
    FROM dual
    CONNECT BY LEVEL <= 6
),
completed_tasks AS (
    SELECT
        t.executor_id,
        TRUNC(th.changed_at, 'MM') AS month_start,
        COUNT(DISTINCT t.task_id) AS completed_count
    FROM Task t
    JOIN TaskHistory th ON th.task_id = t.task_id
    JOIN Status s ON s.status_id = th.new_status_id
    WHERE LOWER(s.status_name) = 'completed'
      AND th.changed_at >= ADD_MONTHS(TRUNC(SYSDATE, 'MM'), -5)
    GROUP BY
        t.executor_id,
        TRUNC(th.changed_at, 'MM')
),
not_completed_tasks AS (
    SELECT
        t.executor_id,
        m.month_start,
        COUNT(*) AS not_completed_count
    FROM Task t
    CROSS JOIN months m
    WHERE t.created_at < ADD_MONTHS(m.month_start, 1)
      AND NOT EXISTS (
          SELECT 1
          FROM TaskHistory th
          JOIN Status s ON s.status_id = th.new_status_id
          WHERE th.task_id = t.task_id
            AND LOWER(s.status_name) = 'completed'
            AND th.changed_at < ADD_MONTHS(m.month_start, 1)
      )
    GROUP BY
        t.executor_id,
        m.month_start
)
SELECT
    u.user_id,
    u.full_name,
    m.month_start,
    NVL(ct.completed_count, 0) AS completed_count,
    NVL(nct.not_completed_count, 0) AS not_completed_count
FROM Users u
CROSS JOIN months m
LEFT JOIN completed_tasks ct
    ON ct.executor_id = u.user_id
   AND ct.month_start = m.month_start
LEFT JOIN not_completed_tasks nct
    ON nct.executor_id = u.user_id
   AND nct.month_start = m.month_start
ORDER BY u.full_name, m.month_start;






------------------------ 6
WITH completed_tasks AS (
    SELECT
        t.task_type,
        t.executor_id,
        COUNT(DISTINCT t.task_id) AS completed_count
    FROM Task t
    JOIN TaskHistory th ON th.task_id = t.task_id
    JOIN Status s ON s.status_id = th.new_status_id
    WHERE LOWER(s.status_name) = 'completed'
    GROUP BY
        t.task_type,
        t.executor_id
),
ranked_executors AS (
    SELECT
        ct.task_type,
        ct.executor_id,
        u.full_name,
        ct.completed_count,
        ROW_NUMBER() OVER (
            PARTITION BY ct.task_type
            ORDER BY ct.completed_count DESC, u.full_name
        ) AS rn
    FROM completed_tasks ct
    JOIN Users u ON u.user_id = ct.executor_id
)
SELECT
    task_type,
    executor_id,
    full_name,
    completed_count
FROM ranked_executors
WHERE rn = 1
ORDER BY task_type;

UPDATE Task
SET task_type = CASE
    WHEN LOWER(title) LIKE '%auth%' OR LOWER(title) LIKE '%password%' THEN 'Security'
    WHEN LOWER(title) LIKE '%test%' THEN 'Testing'
    WHEN LOWER(title) LIKE '%analytics%' OR LOWER(title) LIKE '%dashboard%' THEN 'Analytics'
    WHEN LOWER(title) LIKE '%database%' OR LOWER(title) LIKE '%sql%' THEN 'Database'
    WHEN LOWER(title) LIKE '%ci/cd%' OR LOWER(title) LIKE '%monitoring%' OR LOWER(title) LIKE '%logging%' OR LOWER(title) LIKE '%backup%' THEN 'Infrastructure'
    WHEN LOWER(title) LIKE '%api%' OR LOWER(title) LIKE '%document%' THEN 'Documentation'
    WHEN LOWER(title) LIKE '%ui%' OR LOWER(title) LIKE '%header%' THEN 'Frontend'
    ELSE 'Backend'
END
WHERE task_type IS NULL;

DECLARE
    v_exists NUMBER := 0;
    v_status_completed NUMBER;
    v_status_new NUMBER;
    v_status_progress NUMBER;
    v_status_review NUMBER;
    v_status_hold NUMBER;
    v_title VARCHAR2(255);
    v_type VARCHAR2(100);
    v_action VARCHAR2(30);
    v_start_date DATE;
    v_due_date DATE;
    v_created_at DATE;
    v_creator_id NUMBER;
    v_executor_id NUMBER;
    v_status_id NUMBER;
    v_priority NUMBER;
    v_task_id NUMBER;
BEGIN
    SELECT COUNT(*)
    INTO v_exists
    FROM Task
    WHERE title LIKE 'Lab5:%';

    IF v_exists = 0 THEN
        SELECT status_id INTO v_status_completed FROM Status WHERE LOWER(status_name) = 'completed';
        SELECT status_id INTO v_status_new FROM Status WHERE LOWER(status_name) = 'new';
        SELECT status_id INTO v_status_progress FROM Status WHERE LOWER(status_name) = 'in progress';
        SELECT status_id INTO v_status_review FROM Status WHERE LOWER(status_name) = 'review';

        BEGIN
            SELECT status_id INTO v_status_hold FROM Status WHERE LOWER(status_name) = 'on hold';
        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                v_status_hold := v_status_review;
        END;

        FOR i IN 1 .. 180 LOOP
            v_type := CASE MOD(i - 1, 6)
                WHEN 0 THEN 'Backend'
                WHEN 1 THEN 'Frontend'
                WHEN 2 THEN 'Testing'
                WHEN 3 THEN 'Analytics'
                WHEN 4 THEN 'Infrastructure'
                ELSE 'Documentation'
            END;

            v_action := CASE MOD(i, 3)
                WHEN 0 THEN 'Prepare'
                WHEN 1 THEN 'Do'
                ELSE 'Check'
            END;

            v_start_date := ADD_MONTHS(DATE '2025-01-03', MOD(i - 1, 15)) + MOD((i - 1) * 2, 24);
            v_due_date := v_start_date + 5 + MOD(i, 8);
            v_created_at := v_start_date + ((9 + MOD(i, 7)) / 24);
            v_creator_id := MOD(i + 2, 25) + 1;
            v_executor_id := MOD(i + 9, 25) + 1;
            v_priority := MOD(i, 3) + 1;

            v_status_id := CASE
                WHEN MOD(i, 10) IN (1, 2, 3, 4, 5, 6) THEN v_status_completed
                WHEN MOD(i, 10) IN (7, 8) THEN v_status_progress
                WHEN MOD(i, 10) = 9 THEN v_status_review
                ELSE v_status_hold
            END;

            v_title := 'Lab5 task #' || i || ' - ' || v_type;

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
                v_title,
                'Task for lab 5.',
                v_creator_id,
                v_executor_id,
                v_status_id,
                v_priority,
                v_start_date,
                v_due_date,
                v_created_at,
                v_type
            )
            RETURNING task_id INTO v_task_id;

            INSERT INTO TaskHistory (
                history_id,
                task_id,
                old_status_id,
                new_status_id,
                changed_by,
                changed_at
            )
            VALUES (
                seq_history.NEXTVAL,
                v_task_id,
                v_status_new,
                v_status_progress,
                v_creator_id,
                v_created_at + 1
            );

            IF v_status_id = v_status_completed THEN
                INSERT INTO TaskHistory (
                    history_id,
                    task_id,
                    old_status_id,
                    new_status_id,
                    changed_by,
                    changed_at
                )
                VALUES (
                    seq_history.NEXTVAL,
                    v_task_id,
                    v_status_progress,
                    v_status_completed,
                    v_executor_id,
                    v_created_at + 2 + MOD(i, 6)
                );
            ELSIF v_status_id = v_status_review THEN
                INSERT INTO TaskHistory (
                    history_id,
                    task_id,
                    old_status_id,
                    new_status_id,
                    changed_by,
                    changed_at
                )
                VALUES (
                    seq_history.NEXTVAL,
                    v_task_id,
                    v_status_progress,
                    v_status_review,
                    v_executor_id,
                    v_created_at + 2 + MOD(i, 4)
                );
            END IF;
        END LOOP;
    END IF;
END;
/
