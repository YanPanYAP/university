USE TaskControlDB;

SET ANSI_NULLS ON;

SET QUOTED_IDENTIFIER ON;


/* drop */
IF OBJECT_ID(N'dbo.usp_lab12_extract_by_status', N'P') IS NOT NULL
    DROP PROCEDURE dbo.usp_lab12_extract_by_status;


IF OBJECT_ID(N'dbo.usp_lab12_insert_report', N'P') IS NOT NULL
    DROP PROCEDURE dbo.usp_lab12_insert_report;


IF OBJECT_ID(N'dbo.usp_lab12_generate_report_xml', N'P') IS NOT NULL
    DROP PROCEDURE dbo.usp_lab12_generate_report_xml;


IF EXISTS (
    SELECT 1
    FROM sys.xml_indexes
    WHERE name = N'IX_Report_report_xml_primary'
      AND object_id = OBJECT_ID(N'dbo.Report')
)
    DROP INDEX IX_Report_report_xml_primary ON dbo.Report;


IF OBJECT_ID(N'dbo.Report', N'U') IS NOT NULL
    DROP TABLE dbo.Report;


-- 1
CREATE TABLE dbo.Report
(
    id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
    report_xml XML NOT NULL
);


-- 2
CREATE PROCEDURE dbo.usp_lab12_generate_report_xml
    @date_from DATE,
    @date_to DATE,
    @result_xml XML OUTPUT
AS
BEGIN
    SET NOCOUNT ON;

    ;WITH base_data AS
    (
        SELECT
            t.task_id,
            t.title,
            CAST(t.description AS NVARCHAR(4000)) AS description,
            t.priority,
            t.start_date,
            t.due_date,
            t.created_at,
            t.status_id,
            s.status_name,
            t.creator_id,
            uc.full_name AS creator_name,
            t.executor_id,
            ue.full_name AS executor_name
        FROM dbo.Task t
        JOIN dbo.[User] uc ON uc.user_id = t.creator_id
        JOIN dbo.[User] ue ON ue.user_id = t.executor_id
        JOIN dbo.Status s ON s.status_id = t.status_id
        WHERE t.created_at >= CAST(@date_from AS DATETIME)
          AND t.created_at < DATEADD(DAY, 1, CAST(@date_to AS DATETIME))
    )
    SELECT
        @result_xml =
        (
            SELECT
                CONVERT(VARCHAR(19), SYSDATETIME(), 120) AS [@generated_at],
                CONVERT(VARCHAR(10), @date_from, 120) AS [@date_from],
                CONVERT(VARCHAR(10), @date_to, 120) AS [@date_to],
                (
                    SELECT
                        b.task_id AS [@task_id],
                        b.status_id AS [@status_id],
                        b.creator_id AS [@creator_id],
                        b.executor_id AS [@executor_id],
                        b.title AS [title],
                        b.description AS [description],
                        b.status_name AS [status_name],
                        b.creator_name AS [creator_name],
                        b.executor_name AS [executor_name],
                        CAST(ROUND(CAST(b.priority AS FLOAT), 1) AS DECIMAL(10,1)) AS [priority],
                        CONVERT(VARCHAR(10), b.start_date, 120) AS [start_date],
                        CONVERT(VARCHAR(10), b.due_date, 120) AS [due_date],
                        CONVERT(VARCHAR(19), b.created_at, 120) AS [created_at]
                    FROM base_data b
                    ORDER BY b.created_at, b.task_id
                    FOR XML PATH('task'), TYPE
                ) AS [tasks],
                (
                    SELECT
                        s.status_id AS [@status_id],
                        s.status_name AS [@status_name],
                        COUNT(*) AS [@task_count],
                        CAST(ROUND(AVG(CAST(s.priority AS FLOAT)), 1) AS DECIMAL(10,1)) AS [@avg_priority]
                    FROM base_data s
                    GROUP BY s.status_id, s.status_name
                    FOR XML PATH('status_total'), TYPE
                ) AS [status_totals],
                (
                    SELECT
                        e.executor_id AS [@executor_id],
                        e.executor_name AS [@executor_name],
                        COUNT(*) AS [@task_count]
                    FROM base_data e
                    GROUP BY e.executor_id, e.executor_name
                    FOR XML PATH('executor_total'), TYPE
                ) AS [executor_totals],
                (
                    SELECT
                        COUNT(*) AS [@task_count],
                        CAST(ROUND(AVG(CAST(o.priority AS FLOAT)), 1) AS DECIMAL(10,1)) AS [@avg_priority]
                    FROM base_data o
                    FOR XML PATH('overall_total'), TYPE
                ) AS [overall_total]
            FOR XML PATH('task_report'), TYPE
        );
END;

-- 3
CREATE PROCEDURE dbo.usp_lab12_insert_report
    @date_from DATE,
    @date_to DATE,
    @new_report_id INT OUTPUT
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @x XML;

    EXEC dbo.usp_lab12_generate_report_xml
        @date_from = @date_from,
        @date_to = @date_to,
        @result_xml = @x OUTPUT;

    INSERT INTO dbo.Report(report_xml)
    VALUES (@x);

    SET @new_report_id = SCOPE_IDENTITY();
END;

-- 4
CREATE PRIMARY XML INDEX IX_Report_report_xml_primary
ON dbo.Report(report_xml);

-- 5
CREATE PROCEDURE dbo.usp_lab12_extract_by_status
    @status_name NVARCHAR(100)
AS
BEGIN
    SET NOCOUNT ON;

    SELECT
        r.id AS report_id,
        x.n.value('@task_id', 'INT') AS task_id,
        x.n.value('(title/text())[1]', 'NVARCHAR(255)') AS title,
        x.n.value('(status_name/text())[1]', 'NVARCHAR(100)') AS status_name,
        x.n.value('(executor_name/text())[1]', 'NVARCHAR(200)') AS executor_name,
        x.n.value('(priority/text())[1]', 'DECIMAL(10,1)') AS priority,
        x.n.value('(due_date/text())[1]', 'NVARCHAR(10)') AS due_date
    FROM dbo.Report r
    CROSS APPLY r.report_xml.nodes('/task_report/tasks/task') AS x(n)
    WHERE x.n.value('(status_name/text())[1]', 'NVARCHAR(100)') = @status_name
    ORDER BY r.id DESC, task_id;
END;

-- 
DECLARE @xml_preview XML;
EXEC dbo.usp_lab12_generate_report_xml
    @date_from = '2024-01-01',
    @date_to = '2026-12-31',
    @result_xml = @xml_preview OUTPUT;
SELECT CONVERT(NVARCHAR(MAX), @xml_preview) AS generated_xml_preview;

DECLARE @inserted_id INT;
EXEC dbo.usp_lab12_insert_report
    @date_from = '2024-01-01',
    @date_to = '2026-12-31',
    @new_report_id = @inserted_id OUTPUT;
SELECT @inserted_id AS inserted_report_id;

SELECT TOP (10)
    r.id,
    r.report_xml.value('(/task_report/@generated_at)[1]', 'NVARCHAR(19)') AS generated_at,
    r.report_xml.value('count(/task_report/tasks/task)', 'INT') AS task_count
FROM dbo.Report r
ORDER BY r.id DESC;

DECLARE @status_sample NVARCHAR(100) = (
    SELECT TOP (1)
        x.n.value('(status_name/text())[1]', 'NVARCHAR(100)')
    FROM dbo.Report r
    CROSS APPLY r.report_xml.nodes('/task_report/tasks/task') AS x(n)
    ORDER BY r.id DESC
);
SELECT @status_sample AS selected_status;
EXEC dbo.usp_lab12_extract_by_status @status_name = @status_sample;
