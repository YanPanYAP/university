USE TaskControlDB;

SET NOCOUNT ON;

IF COL_LENGTH('dbo.Task', 'task_type') IS NULL
BEGIN
    ALTER TABLE dbo.Task
    ADD task_type NVARCHAR(100) NULL;
END;

UPDATE dbo.Task
SET task_type = CASE
    WHEN title LIKE N'%auth%' OR title LIKE N'%password%' THEN N'Security'
    WHEN title LIKE N'%test%' OR title LIKE N'%Testing%' THEN N'Testing'
    WHEN title LIKE N'%analytics%' OR title LIKE N'%dashboard%' THEN N'Analytics'
    WHEN title LIKE N'%database%' OR title LIKE N'%SQL%' THEN N'Database'
    WHEN title LIKE N'%CI/CD%' OR title LIKE N'%monitoring%' OR title LIKE N'%logging%' OR title LIKE N'%backup%' THEN N'Infrastructure'
    WHEN title LIKE N'%API%' OR title LIKE N'%documentation%' OR title LIKE N'%Document%' THEN N'Documentation'
    WHEN title LIKE N'%UI%' OR title LIKE N'%header%' OR title LIKE N'%dashboard%' THEN N'Frontend'
    ELSE N'Backend'
END
WHERE task_type IS NULL;

DECLARE @completed_status_id INT = (
    SELECT status_id
    FROM dbo.Status
    WHERE status_name = N'Completed'
);

DECLARE @new_status_id INT = (
    SELECT status_id
    FROM dbo.Status
    WHERE status_name = N'New'
);

DECLARE @in_progress_status_id INT = (
    SELECT status_id
    FROM dbo.Status
    WHERE status_name = N'In Progress'
);

DECLARE @review_status_id INT = (
    SELECT status_id
    FROM dbo.Status
    WHERE status_name = N'Review'
);

DECLARE @on_hold_status_id INT = (
    SELECT status_id
    FROM dbo.Status
    WHERE status_name = N'On Hold'
);

DECLARE @cancelled_status_id INT = (
    SELECT status_id
    FROM dbo.Status
    WHERE status_name = N'Cancelled'
);

IF @new_status_id IS NULL OR @completed_status_id IS NULL OR @in_progress_status_id IS NULL OR @review_status_id IS NULL
BEGIN
    THROW 50020, N'В таблице Status не хватает нужных статусов для лабораторной работы 5.', 1;
END;

IF NOT EXISTS (
    SELECT 1
    FROM dbo.Task
    WHERE title LIKE N'Lab5:%'
)
BEGIN
    WITH numbers AS (
        SELECT TOP (180)
            ROW_NUMBER() OVER (ORDER BY (SELECT NULL)) AS n
        FROM sys.all_objects
    ),
    generated_tasks AS (
        SELECT
            n,
            CASE (n - 1) % 6
                WHEN 0 THEN N'Backend'
                WHEN 1 THEN N'Frontend'
                WHEN 2 THEN N'Testing'
                WHEN 3 THEN N'Analytics'
                WHEN 4 THEN N'Infrastructure'
                ELSE N'Documentation'
            END AS task_type,
            CASE
                WHEN n % 3 = 0 THEN N'Подготовить'
                WHEN n % 3 = 1 THEN N'Выполнить'
                ELSE N'Проверить'
            END AS action_name,
            DATEADD(
                DAY,
                ((n - 1) * 2) % 24,
                DATEADD(
                    MONTH,
                    (n - 1) % 15,
                    DATEFROMPARTS(2025, 1, 3)
                )
            ) AS start_date,
            ((n + 2) % 25) + 1 AS creator_id,
            ((n + 9) % 25) + 1 AS executor_id,
            CASE
                WHEN n % 10 IN (1, 2, 3, 4, 5, 6) THEN @completed_status_id
                WHEN n % 10 IN (7, 8) THEN @in_progress_status_id
                WHEN n % 10 = 9 THEN @review_status_id
                ELSE ISNULL(@on_hold_status_id, @cancelled_status_id)
            END AS status_id,
            (n % 3) + 1 AS priority
        FROM numbers
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
        N'Lab5: ' + action_name + N' задачу № ' + CAST(n AS NVARCHAR(20)) + N' по направлению ' + task_type,
        N'Поручение для лабораторной работы 5.',
        creator_id,
        executor_id,
        status_id,
        priority,
        CAST(start_date AS DATE),
        DATEADD(DAY, 5 + (n % 8), CAST(start_date AS DATE)),
        DATEADD(HOUR, 9 + (n % 7), CAST(start_date AS DATETIME)),
        task_type
    FROM generated_tasks;
END;

IF NOT EXISTS (
    SELECT 1
    FROM dbo.TaskHistory th
    JOIN dbo.Task t ON t.task_id = th.task_id
    WHERE t.title LIKE N'Lab5:%'
)
BEGIN
    WITH task_source AS (
        SELECT
            t.task_id,
            t.creator_id,
            t.executor_id,
            t.status_id,
            t.created_at,
            ROW_NUMBER() OVER (ORDER BY t.task_id) AS seq_no
        FROM dbo.Task t
        WHERE t.title LIKE N'Lab5:%'
    ),
    history_rows AS (
        SELECT
            task_id,
            @new_status_id AS old_status_id,
            @in_progress_status_id AS new_status_id,
            creator_id AS changed_by,
            DATEADD(DAY, 1, CAST(created_at AS DATETIME)) AS changed_at
        FROM task_source

        UNION ALL

        SELECT
            task_id,
            @in_progress_status_id AS old_status_id,
            @completed_status_id AS new_status_id,
            executor_id AS changed_by,
            DATEADD(DAY, 2 + (seq_no % 6), CAST(created_at AS DATETIME)) AS changed_at
        FROM task_source
        WHERE status_id = @completed_status_id

        UNION ALL

        SELECT
            task_id,
            @in_progress_status_id AS old_status_id,
            @review_status_id AS new_status_id,
            executor_id AS changed_by,
            DATEADD(DAY, 2 + (seq_no % 4), CAST(created_at AS DATETIME)) AS changed_at
        FROM task_source
        WHERE status_id = @review_status_id
    )
    INSERT INTO dbo.TaskHistory (
        task_id,
        old_status_id,
        new_status_id,
        changed_by,
        changed_at
    )
    SELECT
        task_id,
        old_status_id,
        new_status_id,
        changed_by,
        changed_at
    FROM history_rows;
END;

;WITH parsed_lab5_tasks AS (
    SELECT
        t.task_id,
        t.title,
        TRY_CAST(
            SUBSTRING(
                t.title,
                CHARINDEX(N'№ ', t.title) + 2,
                CHARINDEX(N' по направлению', t.title) - (CHARINDEX(N'№ ', t.title) + 2)
            ) AS INT
        ) AS task_no
    FROM dbo.Task t
    WHERE t.title LIKE N'Lab5:%'
      AND CHARINDEX(N'№ ', t.title) > 0
      AND CHARINDEX(N' по направлению', t.title) > CHARINDEX(N'№ ', t.title)
),
status_ids AS (
    SELECT
        MAX(CASE WHEN status_name = N'Completed' THEN status_id END) AS completed_status_id,
        MAX(CASE WHEN status_name = N'New' THEN status_id END) AS new_status_id,
        MAX(CASE WHEN status_name = N'In Progress' THEN status_id END) AS in_progress_status_id,
        MAX(CASE WHEN status_name = N'Review' THEN status_id END) AS review_status_id,
        MAX(CASE WHEN status_name = N'On Hold' THEN status_id END) AS on_hold_status_id,
        MAX(CASE WHEN status_name = N'Cancelled' THEN status_id END) AS cancelled_status_id
    FROM dbo.Status
),
fixed_values AS (
    SELECT
        p.task_id,
        p.task_no,
        CASE (p.task_no - 1) % 6
            WHEN 0 THEN N'Backend'
            WHEN 1 THEN N'Frontend'
            WHEN 2 THEN N'Testing'
            WHEN 3 THEN N'Analytics'
            WHEN 4 THEN N'Infrastructure'
            ELSE N'Documentation'
        END AS fixed_task_type,
        ((p.task_no + 2) % 25) + 1 AS fixed_creator_id,
        ((p.task_no + 9) % 25) + 1 AS fixed_executor_id,
        CASE
            WHEN p.task_no % 10 IN (1, 2, 3, 4, 5, 6) THEN s.completed_status_id
            WHEN p.task_no % 10 IN (7, 8) THEN s.in_progress_status_id
            WHEN p.task_no % 10 = 9 THEN s.review_status_id
            ELSE ISNULL(s.on_hold_status_id, s.cancelled_status_id)
        END AS fixed_status_id,
        (p.task_no % 3) + 1 AS fixed_priority,
        DATEADD(
            DAY,
            ((p.task_no - 1) * 2) % 24,
            DATEADD(
                MONTH,
                (p.task_no - 1) % 15,
                DATEFROMPARTS(2025, 1, 3)
            )
        ) AS fixed_start_date
    FROM parsed_lab5_tasks p
    CROSS JOIN status_ids s
    WHERE p.task_no IS NOT NULL
)
UPDATE t
SET
    task_type = ISNULL(t.task_type, f.fixed_task_type),
    creator_id = ISNULL(t.creator_id, f.fixed_creator_id),
    executor_id = ISNULL(t.executor_id, f.fixed_executor_id),
    status_id = ISNULL(t.status_id, f.fixed_status_id),
    priority = ISNULL(t.priority, f.fixed_priority),
    start_date = ISNULL(t.start_date, CAST(f.fixed_start_date AS DATE)),
    due_date = ISNULL(t.due_date, DATEADD(DAY, 5 + (f.task_no % 8), CAST(f.fixed_start_date AS DATE))),
    created_at = ISNULL(t.created_at, DATEADD(HOUR, 9 + (f.task_no % 7), CAST(f.fixed_start_date AS DATETIME))),
    description = ISNULL(t.description, N'Поручение для лабораторной работы 5.')
FROM dbo.Task t
JOIN fixed_values f ON f.task_id = t.task_id;

;WITH lab5_without_history AS (
    SELECT
        t.task_id,
        TRY_CAST(
            SUBSTRING(
                t.title,
                CHARINDEX(N'№ ', t.title) + 2,
                CHARINDEX(N' по направлению', t.title) - (CHARINDEX(N'№ ', t.title) + 2)
            ) AS INT
        ) AS task_no,
        t.creator_id,
        t.executor_id,
        t.status_id,
        t.created_at
    FROM dbo.Task t
    WHERE t.title LIKE N'Lab5:%'
      AND NOT EXISTS (
          SELECT 1
          FROM dbo.TaskHistory th
          WHERE th.task_id = t.task_id
      )
),
status_ids AS (
    SELECT
        MAX(CASE WHEN status_name = N'Completed' THEN status_id END) AS completed_status_id,
        MAX(CASE WHEN status_name = N'New' THEN status_id END) AS new_status_id,
        MAX(CASE WHEN status_name = N'In Progress' THEN status_id END) AS in_progress_status_id,
        MAX(CASE WHEN status_name = N'Review' THEN status_id END) AS review_status_id
    FROM dbo.Status
),
history_rows AS (
    SELECT
        task_id,
        s.new_status_id AS old_status_id,
        s.in_progress_status_id AS new_status_id,
        creator_id AS changed_by,
        DATEADD(DAY, 1, CAST(created_at AS DATETIME)) AS changed_at
    FROM lab5_without_history
    CROSS JOIN status_ids s
    WHERE task_no IS NOT NULL

    UNION ALL

    SELECT
        task_id,
        s.in_progress_status_id AS old_status_id,
        s.completed_status_id AS new_status_id,
        executor_id AS changed_by,
        DATEADD(DAY, 2 + (task_no % 6), CAST(created_at AS DATETIME)) AS changed_at
    FROM lab5_without_history
    CROSS JOIN status_ids s
    WHERE task_no IS NOT NULL
      AND status_id = s.completed_status_id

    UNION ALL

    SELECT
        task_id,
        s.in_progress_status_id AS old_status_id,
        s.review_status_id AS new_status_id,
        executor_id AS changed_by,
        DATEADD(DAY, 2 + (task_no % 4), CAST(created_at AS DATETIME)) AS changed_at
    FROM lab5_without_history
    CROSS JOIN status_ids s
    WHERE task_no IS NOT NULL
      AND status_id = s.review_status_id
)
INSERT INTO dbo.TaskHistory (
    task_id,
    old_status_id,
    new_status_id,
    changed_by,
    changed_at
)
SELECT
    task_id,
    old_status_id,
    new_status_id,
    changed_by,
    changed_at
FROM history_rows;







------------------------ 3
;WITH completed_tasks AS (
    SELECT
        th.task_id,
        MIN(th.changed_at) AS completed_at
    FROM dbo.TaskHistory th
    JOIN dbo.Status s ON s.status_id = th.new_status_id
    WHERE s.status_name = N'Completed'
    GROUP BY th.task_id
),
periodized AS (
    SELECT
        task_id,
        completed_at,
        YEAR(completed_at) AS year_num,
        MONTH(completed_at) AS month_num,
        DATEPART(QUARTER, completed_at) AS quarter_num,
        CASE WHEN MONTH(completed_at) <= 6 THEN 1 ELSE 2 END AS half_year_num
    FROM completed_tasks
)
SELECT
    CASE
        WHEN GROUPING(year_num) = 0 AND GROUPING(month_num) = 0 THEN N'Месяц'
        WHEN GROUPING(year_num) = 0 AND GROUPING(quarter_num) = 0 THEN N'Квартал'
        WHEN GROUPING(year_num) = 0 AND GROUPING(half_year_num) = 0 THEN N'Полугодие'
        ELSE N'Год'
    END AS period_type,
    CASE
        WHEN GROUPING(year_num) = 0 AND GROUPING(month_num) = 0
            THEN CONCAT(year_num, N'-', RIGHT(N'0' + CAST(month_num AS NVARCHAR(2)), 2))
        WHEN GROUPING(year_num) = 0 AND GROUPING(quarter_num) = 0
            THEN CONCAT(year_num, N' Q', quarter_num)
        WHEN GROUPING(year_num) = 0 AND GROUPING(half_year_num) = 0
            THEN CONCAT(year_num, N' H', half_year_num)
        ELSE CAST(year_num AS NVARCHAR(4))
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
;WITH params AS (
    SELECT
        CAST('2025-10-01' AS DATE) AS period_start,
        CAST('2025-12-31' AS DATE) AS period_end
),
completed_in_period AS (
    SELECT
        COUNT(DISTINCT th.task_id) AS completed_in_period_count
    FROM dbo.TaskHistory th
    JOIN dbo.Status s ON s.status_id = th.new_status_id
    CROSS JOIN params p
    WHERE s.status_name = N'Completed'
      AND CAST(th.changed_at AS DATE) BETWEEN p.period_start AND p.period_end
),
completed_total AS (
    SELECT
        COUNT(DISTINCT th.task_id) AS completed_total_count
    FROM dbo.TaskHistory th
    JOIN dbo.Status s ON s.status_id = th.new_status_id
    WHERE s.status_name = N'Completed'
),
not_completed_total AS (
    SELECT
        COUNT(*) AS not_completed_total_count
    FROM dbo.Task t
    WHERE NOT EXISTS (
        SELECT 1
        FROM dbo.TaskHistory th
        JOIN dbo.Status s ON s.status_id = th.new_status_id
        WHERE th.task_id = t.task_id
          AND s.status_name = N'Completed'
    )
)
SELECT
    p.period_start,
    p.period_end,
    cip.completed_in_period_count AS completed_tasks_count,
    CAST(cip.completed_in_period_count * 100.0 / NULLIF(ct.completed_total_count, 0) AS DECIMAL(10, 2)) AS percent_of_all_completed,
    CAST(cip.completed_in_period_count * 100.0 / NULLIF(nct.not_completed_total_count, 0) AS DECIMAL(10, 2)) AS percent_of_all_not_completed
FROM params p
CROSS JOIN completed_in_period cip
CROSS JOIN completed_total ct
CROSS JOIN not_completed_total nct;





------------------------ 5
;WITH params AS (
    SELECT
        2 AS page_number,
        20 AS page_size
),
numbered_tasks AS (
    SELECT
        ROW_NUMBER() OVER (ORDER BY t.created_at DESC, t.task_id DESC) AS row_num,
        t.task_id,
        t.title,
        t.task_type,
        t.priority,
        s.status_name,
        u.full_name AS executor_name,
        t.created_at
    FROM dbo.Task t
    JOIN dbo.Status s ON s.status_id = t.status_id
    JOIN dbo.[User] u ON u.user_id = t.executor_id
)
SELECT
    row_num,
    task_id,
    title,
    task_type,
    priority,
    status_name,
    executor_name,
    created_at
FROM numbered_tasks n
CROSS JOIN params p
WHERE n.row_num BETWEEN (p.page_number - 1) * p.page_size + 1
                    AND p.page_number * p.page_size
ORDER BY n.row_num;






------------------------ 6
IF OBJECT_ID(N'dbo.Lab5DuplicateDemo', N'U') IS NOT NULL
BEGIN
    DROP TABLE dbo.Lab5DuplicateDemo;
END;


CREATE TABLE dbo.Lab5DuplicateDemo (
    duplicate_id INT IDENTITY(1,1) PRIMARY KEY,
    task_id INT NOT NULL,
    executor_id INT NOT NULL,
    note NVARCHAR(100) NOT NULL
);

INSERT INTO dbo.Lab5DuplicateDemo (task_id, executor_id, note)
VALUES
    (1, 3, N'duplicate'),
    (1, 3, N'duplicate'),
    (2, 4, N'duplicate'),
    (2, 4, N'duplicate'),
    (2, 4, N'duplicate'),
    (3, 5, N'unique'),
    (4, 6, N'duplicate'),
    (4, 6, N'duplicate');

;WITH duplicates AS (
    SELECT
        duplicate_id,
        ROW_NUMBER() OVER (
            PARTITION BY task_id, executor_id, note
            ORDER BY duplicate_id
        ) AS rn
    FROM dbo.Lab5DuplicateDemo
)
DELETE FROM duplicates
WHERE rn > 1;

SELECT *
FROM dbo.Lab5DuplicateDemo
ORDER BY duplicate_id;







------------------------ 7
;WITH months AS (
    SELECT DATEFROMPARTS(YEAR(DATEADD(MONTH, -5, GETDATE())), MONTH(DATEADD(MONTH, -5, GETDATE())), 1) AS month_start
    UNION ALL
    SELECT DATEADD(MONTH, 1, month_start)
    FROM months
    WHERE month_start < DATEFROMPARTS(YEAR(GETDATE()), MONTH(GETDATE()), 1)
),
completed_tasks AS (
    SELECT
        t.executor_id,
        DATEFROMPARTS(YEAR(th.changed_at), MONTH(th.changed_at), 1) AS month_start,
        COUNT(DISTINCT t.task_id) AS completed_count
    FROM dbo.Task t
    JOIN dbo.TaskHistory th ON th.task_id = t.task_id
    JOIN dbo.Status s ON s.status_id = th.new_status_id
    WHERE s.status_name = N'Completed'
      AND th.changed_at >= DATEADD(MONTH, -5, DATEFROMPARTS(YEAR(GETDATE()), MONTH(GETDATE()), 1))
    GROUP BY
        t.executor_id,
        DATEFROMPARTS(YEAR(th.changed_at), MONTH(th.changed_at), 1)
),
not_completed_tasks AS (
    SELECT
        t.executor_id,
        m.month_start,
        COUNT(*) AS not_completed_count
    FROM dbo.Task t
    CROSS JOIN months m
    WHERE NOT EXISTS (
        SELECT 1
        FROM dbo.TaskHistory th
        JOIN dbo.Status s ON s.status_id = th.new_status_id
        WHERE th.task_id = t.task_id
          AND s.status_name = N'Completed'
          AND th.changed_at < DATEADD(MONTH, 1, m.month_start)
    )
      AND t.created_at < DATEADD(MONTH, 1, m.month_start)
    GROUP BY
        t.executor_id,
        m.month_start
)
SELECT
    u.user_id,
    u.full_name,
    m.month_start,
    ISNULL(ct.completed_count, 0) AS completed_count,
    ISNULL(nct.not_completed_count, 0) AS not_completed_count
FROM dbo.[User] u
CROSS JOIN months m
LEFT JOIN completed_tasks ct
    ON ct.executor_id = u.user_id
   AND ct.month_start = m.month_start
LEFT JOIN not_completed_tasks nct
    ON nct.executor_id = u.user_id
   AND nct.month_start = m.month_start
ORDER BY u.full_name, m.month_start
OPTION (MAXRECURSION 6);






------------------------ 8
;WITH completed_tasks AS (
    SELECT
        t.task_type,
        t.executor_id,
        COUNT(DISTINCT t.task_id) AS completed_count
    FROM dbo.Task t
    JOIN dbo.TaskHistory th ON th.task_id = t.task_id
    JOIN dbo.Status s ON s.status_id = th.new_status_id
    WHERE s.status_name = N'Completed'
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
            ORDER BY ct.completed_count DESC, u.full_name ASC
        ) AS rn
    FROM completed_tasks ct
    JOIN dbo.[User] u ON u.user_id = ct.executor_id
)
SELECT
    task_type,
    executor_id,
    full_name,
    completed_count
FROM ranked_executors
WHERE rn = 1
ORDER BY task_type;
