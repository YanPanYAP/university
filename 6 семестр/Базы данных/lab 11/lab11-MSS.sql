USE TaskControlDB;


-- 1
CREATE OR ALTER FUNCTION dbo.fn_lab11_tasks_period
(
    @date_from DATE,
    @date_to   DATE
)
RETURNS TABLE
AS
RETURN
(
    SELECT
        t.task_id,
        t.title,
        t.description,
        t.creator_id,
        u_creator.full_name  AS creator_name,
        t.executor_id,
        u_executor.full_name AS executor_name,
        t.status_id,
        s.status_name,
        CAST(ROUND(CAST(t.priority AS FLOAT), 1) AS DECIMAL(10,1)) AS priority,
        t.start_date,
        t.due_date,
        t.created_at
    FROM dbo.Task t
    JOIN dbo.[User] u_creator ON u_creator.user_id = t.creator_id
    JOIN dbo.[User] u_executor ON u_executor.user_id = t.executor_id
    JOIN dbo.Status s ON s.status_id = t.status_id
    WHERE t.created_at >= CAST(@date_from AS DATETIME)
      AND t.created_at < DATEADD(DAY, 1, CAST(@date_to AS DATETIME))
);


SELECT *
FROM dbo.fn_lab11_tasks_period('2024-01-15', '2024-02-15')
ORDER BY created_at, task_id;


-- 2

-- 3
SELECT user_id, full_name
FROM dbo.[User]
WHERE user_id IN (1, 2, 3, 4, 5)
ORDER BY user_id;

SELECT status_id, status_name
FROM dbo.Status
WHERE status_id IN (1, 2, 3, 4)
ORDER BY status_id;

-- 4
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
FROM dbo.Task
WHERE title LIKE 'MSS IMPORT%'
ORDER BY task_id DESC;
