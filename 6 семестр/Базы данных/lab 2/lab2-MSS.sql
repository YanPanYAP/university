CREATE DATABASE TaskControlDB;
GO

USE TaskControlDB;
GO

CREATE TABLE Role (
    role_id INT PRIMARY KEY IDENTITY(1,1),
    role_name NVARCHAR(100) NOT NULL UNIQUE,
    description NVARCHAR(255)
);

CREATE TABLE Department (
    department_id INT PRIMARY KEY IDENTITY(1,1),
    department_name NVARCHAR(200) NOT NULL,
    parent_department_id INT NULL,
    CONSTRAINT FK_Department_Parent FOREIGN KEY (parent_department_id)
        REFERENCES Department(department_id)
);

CREATE TABLE [User] (
    user_id INT PRIMARY KEY IDENTITY(1,1),
    full_name NVARCHAR(200) NOT NULL,
    email NVARCHAR(200) NOT NULL UNIQUE,
    password NVARCHAR(255) NOT NULL,
    role_id INT NOT NULL,
    department_id INT,
    CONSTRAINT FK_User_Role FOREIGN KEY (role_id)
        REFERENCES Role(role_id),
    CONSTRAINT FK_User_Department FOREIGN KEY (department_id)
        REFERENCES Department(department_id)
);

CREATE TABLE Status (
    status_id INT PRIMARY KEY IDENTITY(1,1),
    status_name NVARCHAR(100) NOT NULL UNIQUE
);

CREATE TABLE Task (
    task_id INT PRIMARY KEY IDENTITY(1,1),
    title NVARCHAR(255) NOT NULL,
    description NVARCHAR(MAX),
    creator_id INT NOT NULL,
    executor_id INT NOT NULL,
    status_id INT NOT NULL,
    priority INT,
    start_date DATE,
    due_date DATE,
    created_at DATETIME DEFAULT GETDATE(),
    CONSTRAINT FK_Task_Creator FOREIGN KEY (creator_id)
        REFERENCES [User](user_id),
    CONSTRAINT FK_Task_Executor FOREIGN KEY (executor_id)
        REFERENCES [User](user_id),
    CONSTRAINT FK_Task_Status FOREIGN KEY (status_id)
        REFERENCES Status(status_id)
);

CREATE TABLE Comment (
    comment_id INT PRIMARY KEY IDENTITY(1,1),
    task_id INT NOT NULL,
    user_id INT NOT NULL,
    comment_text NVARCHAR(MAX) NOT NULL,
    created_at DATETIME DEFAULT GETDATE(),
    CONSTRAINT FK_Comment_Task FOREIGN KEY (task_id)
        REFERENCES Task(task_id) ON DELETE CASCADE,
    CONSTRAINT FK_Comment_User FOREIGN KEY (user_id)
        REFERENCES [User](user_id)
);

CREATE TABLE Attachment (
    attachment_id INT PRIMARY KEY IDENTITY(1,1),
    task_id INT NOT NULL,
    file_name NVARCHAR(255),
    file_path NVARCHAR(500),
    uploaded_at DATETIME DEFAULT GETDATE(),
    CONSTRAINT FK_Attachment_Task FOREIGN KEY (task_id)
        REFERENCES Task(task_id) ON DELETE CASCADE
);

CREATE TABLE TaskHistory (
    history_id INT PRIMARY KEY IDENTITY(1,1),
    task_id INT NOT NULL,
    old_status_id INT,
    new_status_id INT,
    changed_by INT,
    changed_at DATETIME DEFAULT GETDATE(),
    CONSTRAINT FK_History_Task FOREIGN KEY (task_id)
        REFERENCES Task(task_id) ON DELETE CASCADE,
    CONSTRAINT FK_History_OldStatus FOREIGN KEY (old_status_id)
        REFERENCES Status(status_id),
    CONSTRAINT FK_History_NewStatus FOREIGN KEY (new_status_id)
        REFERENCES Status(status_id),
    CONSTRAINT FK_History_User FOREIGN KEY (changed_by)
        REFERENCES [User](user_id)
);

CREATE INDEX IX_Task_Status ON Task(status_id);
CREATE INDEX IX_Task_Executor ON Task(executor_id);
CREATE INDEX IX_Comment_Task ON Comment(task_id);
CREATE INDEX IX_Attachment_Task ON Attachment(task_id);

CREATE VIEW vw_TaskReport AS
SELECT 
    t.task_id,
    t.title,
    t.priority,
    t.start_date,
    t.due_date,
    s.status_name,
    u1.full_name AS creator_name,
    u2.full_name AS executor_name
FROM Task t
JOIN Status s ON t.status_id = s.status_id
JOIN [User] u1 ON t.creator_id = u1.user_id
JOIN [User] u2 ON t.executor_id = u2.user_id;

CREATE ROLE Administrator;
CREATE ROLE Executor;
CREATE ROLE Manager;

GRANT SELECT, INSERT, UPDATE, DELETE ON Task TO Executor;
GRANT SELECT, UPDATE ON Task TO Manager;
GRANT SELECT ON Task TO Administrator;

-- создание задачи
CREATE PROCEDURE sp_CreateTask
    @title NVARCHAR(255),
    @description NVARCHAR(MAX),
    @creator_id INT,
    @executor_id INT,
    @priority INT,
    @due_date DATE
AS
BEGIN
    INSERT INTO Task (title, description, creator_id, executor_id,
                      status_id, priority, start_date, due_date)
    VALUES (@title, @description, @creator_id, @executor_id,
            1, @priority, GETDATE(), @due_date);
END;


-- смена статуса + запись истории
CREATE PROCEDURE sp_ChangeTaskStatus
    @task_id INT,
    @new_status_id INT,
    @changed_by INT
AS
BEGIN
    DECLARE @old_status INT;

    SELECT @old_status = status_id
    FROM Task WHERE task_id = @task_id;

    UPDATE Task
    SET status_id = @new_status_id
    WHERE task_id = @task_id;

    INSERT INTO TaskHistory(task_id, old_status_id, new_status_id, changed_by)
    VALUES (@task_id, @old_status, @new_status_id, @changed_by);
END;

-- переназначение исполнителя
CREATE PROCEDURE sp_ReassignExecutor
    @task_id INT,
    @new_executor_id INT
AS
BEGIN
    UPDATE Task
    SET executor_id = @new_executor_id
    WHERE task_id = @task_id;
END;

-- удаление задачи
CREATE PROCEDURE sp_DeleteTask
    @task_id INT
AS
BEGIN
    DELETE FROM Task WHERE task_id = @task_id;
END;

-- добавление комментария
CREATE PROCEDURE sp_AddComment
    @task_id INT,
    @user_id INT,
    @text NVARCHAR(MAX)
AS
BEGIN
    INSERT INTO [Comment](task_id, user_id, comment_text)
    VALUES (@task_id, @user_id, @text);
END;

-- повысить приоритет задачи
CREATE PROCEDURE sp_IncreasePriority
    @task_id INT,
    @increment INT
AS
BEGIN
    UPDATE Task
    SET priority = ISNULL(priority,0) + @increment
    WHERE task_id = @task_id;
END;

-- создать напоминание
CREATE PROCEDURE sp_SetTaskReminder
    @task_id INT,
    @reminder_date DATETIME
AS
BEGIN
    INSERT INTO TaskHistory(task_id, old_status_id, new_status_id, changed_by, changed_at)
    VALUES (@task_id, NULL, NULL, NULL, @reminder_date);
END;

-- массовое назначение задач через список ID в виде строки
CREATE PROCEDURE sp_AssignMultipleTasks
    @executor_id INT,
    @task_ids NVARCHAR(MAX)
AS
BEGIN
    UPDATE Task
    SET executor_id = @executor_id
    WHERE task_id IN (
        SELECT CAST(value AS INT) 
        FROM STRING_SPLIT(@task_ids, ',')
    );
END;

-- архивирование завершенной задачи
CREATE PROCEDURE sp_ArchiveTask
    @task_id INT
AS
BEGIN
    UPDATE Task
    SET status_id = 4
    WHERE task_id = @task_id;
END;

-- добавить отметку важности задачи
CREATE PROCEDURE sp_MarkTaskImportant
    @task_id INT
AS
BEGIN
    UPDATE Task
    SET priority = 100
    WHERE task_id = @task_id;
END;

--------

-- количество задач пользователя
CREATE FUNCTION fn_CountUserTasks (@user_id INT)
RETURNS INT
AS
BEGIN
    DECLARE @cnt INT;
    SELECT @cnt = COUNT(*) FROM Task WHERE executor_id = @user_id;
    RETURN @cnt;
END;

-- количество просроченных задач
CREATE FUNCTION fn_CountOverdueTasks ()
RETURNS INT
AS
BEGIN
    DECLARE @cnt INT;
    SELECT @cnt = COUNT(*)
    FROM Task
    WHERE due_date < GETDATE() AND status_id <> 3;
    RETURN @cnt;
END;

-- средний приоритет задач пользователя
CREATE FUNCTION fn_AvgPriority (@user_id INT)
RETURNS FLOAT
AS
BEGIN
    DECLARE @avg FLOAT;
    SELECT @avg = AVG(CAST(priority AS FLOAT))
    FROM Task WHERE executor_id = @user_id;
    RETURN ISNULL(@avg, 0);
END;

-- дата последнего комментария по задаче
CREATE FUNCTION fn_LastCommentDate (@task_id INT)
RETURNS DATETIME
AS
BEGIN
    DECLARE @d DATETIME;
    SELECT @d = MAX(created_at)
    FROM [Comment] WHERE task_id = @task_id;
    RETURN @d;
END;

-- проверка просрочки задачи
CREATE FUNCTION fn_IsTaskOverdue (@task_id INT)
RETURNS BIT
AS
BEGIN
    DECLARE @res BIT = 0;

    IF EXISTS (
        SELECT 1 FROM Task
        WHERE task_id = @task_id
          AND due_date < GETDATE()
          AND status_id <> 3
    )
        SET @res = 1;

    RETURN @res;
END;

-- есть ли важные задачи
CREATE FUNCTION fn_HasImportantTasks (@user_id INT)
RETURNS BIT
AS
BEGIN
    DECLARE @res BIT = 0;

    IF EXISTS(SELECT 1 FROM Task WHERE executor_id = @user_id AND priority = 100)
        SET @res = 1;

    RETURN @res;
END;

-- ID последнего комментария пользователя
CREATE FUNCTION fn_LastUserComment (@user_id INT)
RETURNS INT
AS
BEGIN
    DECLARE @res INT;
    SELECT @res = MAX(comment_id) FROM Comment WHERE user_id = @user_id;
    RETURN @res;
END;

-- количество задач по статусу
CREATE FUNCTION fn_CountTasksByStatus (@status_id INT)
RETURNS INT
AS
BEGIN
    DECLARE @cnt INT;
    SELECT @cnt = COUNT(*) FROM Task WHERE status_id = @status_id;
    RETURN @cnt;
END;

-- все ли задачи завершены
CREATE FUNCTION fn_AllTasksCompleted (@user_id INT)
RETURNS BIT
AS
BEGIN
    DECLARE @res BIT = 0;

    IF NOT EXISTS (SELECT 1 FROM Task WHERE executor_id = @user_id AND status_id <> 3)
        SET @res = 1;

    RETURN @res;
END;

-- среднее количество комментариев на задачу
CREATE FUNCTION fn_AvgCommentsPerTask (@user_id INT)
RETURNS FLOAT
AS
BEGIN
    DECLARE @res FLOAT;
    SELECT @res = ISNULL(AVG(c),0)
    FROM (SELECT COUNT(*) AS c
          FROM Comment c
          JOIN Task t ON c.task_id = t.task_id
          WHERE t.executor_id = @user_id
          GROUP BY c.task_id) AS sub;
    RETURN @res;
END;

DELETE FROM TaskHistory;
DELETE FROM Attachment;
DELETE FROM Comment;
DELETE FROM Task;
DELETE FROM [User];
DELETE FROM Department;
DELETE FROM Status;
DELETE FROM Role;

-- Сброс Identity счетчиков
DBCC CHECKIDENT ('Role', RESEED, 0);
DBCC CHECKIDENT ('Department', RESEED, 0);
DBCC CHECKIDENT ('User', RESEED, 0);
DBCC CHECKIDENT ('Status', RESEED, 0);
DBCC CHECKIDENT ('Task', RESEED, 0);
DBCC CHECKIDENT ('Comment', RESEED, 0);
DBCC CHECKIDENT ('Attachment', RESEED, 0);
DBCC CHECKIDENT ('TaskHistory', RESEED, 0);

INSERT INTO Role (role_name, description) VALUES
('Administrator', 'Full system access'),
('Manager', 'Task and report management'),
('Executor', 'Task execution');

INSERT INTO Department (department_name, parent_department_id) VALUES
('Head Office', NULL),
('Development Department', 1),
('Testing Department', 1),
('Analytics Department', 1),
('Mobile Development', 2),
('Web Development', 2),
('Manual Testing', 3),
('Automation Testing', 3),
('Business Analytics', 4),
('System Analytics', 4),
('DevOps Team', 2),
('UI/UX Design', 2),
('Product Management', 1),
('Customer Support', 1),
('Sales Department', 1);

INSERT INTO [User] (full_name, email, password, role_id, department_id) VALUES
('John Smith', 'john.smith@company.com', 'pass123', 1, 1),
('Sarah Johnson', 'sarah.johnson@company.com', 'pass123', 2, 2),
('Michael Brown', 'michael.brown@company.com', 'pass123', 3, 5),
('Emily Davis', 'emily.davis@company.com', 'pass123', 3, 6),
('Robert Wilson', 'robert.wilson@company.com', 'pass123', 2, 3),
('Jennifer Lee', 'jennifer.lee@company.com', 'pass123', 3, 7),
('David Miller', 'david.miller@company.com', 'pass123', 3, 8),
('Lisa Anderson', 'lisa.anderson@company.com', 'pass123', 2, 4),
('James Taylor', 'james.taylor@company.com', 'pass123', 3, 9),
('Maria Garcia', 'maria.garcia@company.com', 'pass123', 3, 10),
('William Jones', 'william.jones@company.com', 'pass123', 1, 1),
('Elizabeth Martin', 'elizabeth.martin@company.com', 'pass123', 2, 11),
('Charles White', 'charles.white@company.com', 'pass123', 3, 5),
('Patricia Moore', 'patricia.moore@company.com', 'pass123', 3, 6),
('Thomas Jackson', 'thomas.jackson@company.com', 'pass123', 2, 12),
('Susan Thompson', 'susan.thompson@company.com', 'pass123', 3, 7),
('Joseph Harris', 'joseph.harris@company.com', 'pass123', 3, 8),
('Margaret Clark', 'margaret.clark@company.com', 'pass123', 1, 13),
('Daniel Lewis', 'daniel.lewis@company.com', 'pass123', 2, 14),
('Nancy Walker', 'nancy.walker@company.com', 'pass123', 3, 15),
('Christopher Hall', 'christopher.hall@company.com', 'pass123', 3, 5),
('Karen Young', 'karen.young@company.com', 'pass123', 3, 6),
('Matthew King', 'matthew.king@company.com', 'pass123', 2, 2),
('Betty Wright', 'betty.wright@company.com', 'pass123', 3, 9),
('Anthony Scott', 'anthony.scott@company.com', 'pass123', 3, 10);

INSERT INTO Status (status_name) VALUES
('New'),
('In Progress'),
('Review'),
('Completed'),
('On Hold'),
('Cancelled'),
('Need Clarification'),
('Ready for Testing'),
('Testing'),
('Reopened');

INSERT INTO Task (title, description, creator_id, executor_id, status_id, priority, start_date, due_date, created_at) VALUES
('Implement user authentication', 'Create login/register functionality with JWT tokens', 1, 3, 2, 1, '2024-01-15', '2024-01-30', '2024-01-15 09:00:00'),
('Design database schema', 'Create ERD and implement tables for the new project', 2, 4, 3, 2, '2024-01-16', '2024-01-25', '2024-01-16 10:30:00'),
('Create responsive header', 'Implement mobile-friendly navigation bar', 3, 5, 1, 3, '2024-01-17', '2024-01-28', '2024-01-17 11:15:00'),
('Write unit tests for API', 'Cover all endpoints with unit tests', 4, 6, 2, 1, '2024-01-18', '2024-01-22', '2024-01-18 14:20:00'),
('Optimize SQL queries', 'Improve performance of slow database queries', 5, 7, 4, 2, '2024-01-19', '2024-02-05', '2024-01-19 08:45:00'),
('Setup CI/CD pipeline', 'Configure Jenkins for automated deployment', 6, 8, 5, 1, '2024-01-20', '2024-01-27', '2024-01-20 13:10:00'),
('Create user dashboard', 'Design and implement user profile page', 7, 9, 6, 3, '2024-01-21', '2024-01-26', '2024-01-21 15:30:00'),
('Document REST API', 'Write API documentation using Swagger', 8, 10, 7, 2, '2024-01-22', '2024-02-10', '2024-01-22 09:55:00'),
('Fix payment gateway bug', 'Resolve issues with payment processing', 9, 11, 2, 1, '2024-01-23', '2024-01-24', '2024-01-23 16:40:00'),
('Setup monitoring system', 'Install and configure Prometheus and Grafana', 10, 12, 3, 3, '2024-01-24', '2024-02-15', '2024-01-24 11:25:00'),
('Implement search functionality', 'Add full-text search to the application', 11, 13, 8, 2, '2024-02-01', '2024-02-20', '2024-01-25 10:00:00'),
('Add email notifications', 'Implement email alerts for task updates', 12, 14, 9, 1, '2024-02-02', '2024-02-18', '2024-01-26 14:15:00'),
('Create export feature', 'Add ability to export data to Excel', 13, 15, 10, 2, '2024-02-03', '2024-02-12', '2024-01-27 12:30:00'),
('Update UI components', 'Refactor frontend components with new design', 14, 16, 2, 3, '2024-02-04', '2024-02-25', '2024-01-28 09:20:00'),
('Implement caching', 'Add Redis caching for frequently accessed data', 15, 17, 3, 1, '2024-02-05', '2024-02-09', '2024-01-29 15:45:00'),
('Create analytics dashboard', 'Build charts for business metrics', 16, 18, 1, 2, '2024-02-06', '2024-02-16', '2024-01-30 11:10:00'),
('Setup logging system', 'Implement structured logging with ELK stack', 17, 19, 4, 3, '2024-02-07', '2024-02-11', '2024-01-31 13:35:00'),
('Configure backup strategy', 'Automate database backups', 18, 20, 5, 1, '2024-02-08', '2024-02-14', '2024-02-01 08:50:00'),
('Load testing', 'Perform load tests using JMeter', 19, 21, 2, 2, '2024-02-09', '2024-02-13', '2024-02-02 16:05:00'),
('Add multi-language support', 'Implement i18n for English and Spanish', 20, 22, 6, 3, '2024-02-10', '2024-02-28', '2024-02-03 10:40:00'),
('Implement file upload', 'Add attachment functionality to tasks', 21, 23, 7, 2, '2024-02-11', '2024-02-19', '2024-02-04 12:15:00'),
('Create user activity log', 'Track and display user actions', 22, 24, 8, 1, '2024-02-12', '2024-02-17', '2024-02-05 14:30:00'),
('Implement password reset', 'Add forgot password functionality', 23, 25, 9, 2, '2024-02-13', '2024-02-16', '2024-02-06 09:55:00'),
('Add data validation', 'Implement server-side validation for all forms', 24, 3, 10, 3, '2024-02-14', '2024-02-21', '2024-02-07 11:20:00'),
('Create error handling', 'Implement global error handling middleware', 25, 4, 1, 1, '2024-02-15', '2024-02-22', '2024-02-08 15:10:00');

INSERT INTO Comment (task_id, user_id, comment_text, created_at) VALUES
(1, 3, 'Started working on authentication endpoints', '2024-01-15 10:30:00'),
(1, 1, 'Please make sure to add refresh tokens', '2024-01-15 14:20:00'),
(2, 4, 'Created initial ERD, need review', '2024-01-17 11:45:00'),
(2, 2, 'Looks good, add indexing strategy', '2024-01-17 13:15:00'),
(3, 5, 'Header is responsive on all devices', '2024-01-20 16:30:00'),
(3, 3, 'Great work, approved', '2024-01-21 09:45:00'),
(4, 6, 'Unit tests completed for auth endpoints', '2024-01-19 14:20:00'),
(4, 4, 'Add more edge cases', '2024-01-19 15:40:00'),
(5, 7, 'Optimized main queries, 50% faster', '2024-01-25 12:10:00'),
(5, 5, 'Excellent performance improvement', '2024-01-26 10:30:00'),
(6, 8, 'Jenkins pipeline configured', '2024-01-24 15:55:00'),
(6, 6, 'Tested deployment, works fine', '2024-01-25 09:20:00'),
(7, 9, 'Dashboard UI ready for review', '2024-01-28 13:40:00'),
(7, 7, 'Minor adjustments needed', '2024-01-28 15:15:00'),
(8, 10, 'API documentation completed', '2024-01-29 11:30:00'),
(8, 8, 'Looks comprehensive, good job', '2024-01-30 10:45:00'),
(9, 11, 'Fixed payment gateway timeout issue', '2024-01-23 17:20:00'),
(9, 9, 'Verified fix, ready for production', '2024-01-24 09:15:00'),
(10, 12, 'Grafana dashboards created', '2024-01-30 14:50:00'),
(10, 10, 'Add CPU usage metrics', '2024-01-31 11:25:00'),
(11, 13, 'Search implementation in progress', '2024-02-05 10:10:00'),
(11, 11, 'Consider adding filters', '2024-02-05 14:30:00'),
(12, 14, 'Email templates created', '2024-02-06 13:20:00'),
(12, 12, 'Tested notifications, all good', '2024-02-07 09:55:00'),
(13, 15, 'Export feature ready for testing', '2024-02-08 15:40:00'),
(13, 13, 'Works with large datasets', '2024-02-09 11:15:00');

INSERT INTO Attachment (task_id, file_name, file_path, uploaded_at) VALUES
(1, 'auth_spec.docx', '/attachments/auth_spec.docx', '2024-01-15 11:30:00'),
(2, 'database_erd.png', '/attachments/database_erd.png', '2024-01-16 14:45:00'),
(3, 'header_mockup.fig', '/attachments/header_mockup.fig', '2024-01-17 10:20:00'),
(4, 'unit_tests.zip', '/attachments/unit_tests.zip', '2024-01-18 16:55:00'),
(5, 'query_optimization.sql', '/attachments/query_optimization.sql', '2024-01-19 13:10:00'),
(6, 'jenkins_config.xml', '/attachments/jenkins_config.xml', '2024-01-20 09:35:00'),
(7, 'dashboard_design.fig', '/attachments/dashboard_design.fig', '2024-01-21 12:25:00'),
(8, 'swagger_docs.json', '/attachments/swagger_docs.json', '2024-01-22 15:50:00'),
(9, 'payment_fix.patch', '/attachments/payment_fix.patch', '2024-01-23 11:40:00'),
(10, 'grafana_config.json', '/attachments/grafana_config.json', '2024-01-24 16:15:00'),
(11, 'search_algorithm.pdf', '/attachments/search_algorithm.pdf', '2024-02-02 10:30:00'),
(12, 'email_templates.zip', '/attachments/email_templates.zip', '2024-02-03 14:20:00'),
(13, 'export_module.cs', '/attachments/export_module.cs', '2024-02-04 13:45:00'),
(14, 'ui_components.zip', '/attachments/ui_components.zip', '2024-02-05 09:55:00'),
(15, 'redis_config.conf', '/attachments/redis_config.conf', '2024-02-06 15:30:00'),
(16, 'analytics_dashboard.json', '/attachments/analytics_dashboard.json', '2024-02-07 11:10:00'),
(17, 'logstash_config.yml', '/attachments/logstash_config.yml', '2024-02-08 16:40:00'),
(18, 'backup_script.sh', '/attachments/backup_script.sh', '2024-02-09 10:15:00'),
(19, 'jmeter_test.jmx', '/attachments/jmeter_test.jmx', '2024-02-10 14:50:00'),
(20, 'i18n_files.zip', '/attachments/i18n_files.zip', '2024-02-11 12:05:00'),
(21, 'file_upload.js', '/attachments/file_upload.js', '2024-02-12 09:25:00'),
(22, 'activity_log.sql', '/attachments/activity_log.sql', '2024-02-13 15:35:00'),
(23, 'password_reset.html', '/attachments/password_reset.html', '2024-02-14 11:50:00'),
(24, 'validation_rules.json', '/attachments/validation_rules.json', '2024-02-15 13:15:00'),
(25, 'error_handler.py', '/attachments/error_handler.py', '2024-02-16 10:40:00');

INSERT INTO TaskHistory (task_id, old_status_id, new_status_id, changed_by, changed_at) VALUES
(1, 1, 2, 1, '2024-01-15 10:35:00'),
(1, 2, 1, 3, '2024-01-16 09:20:00'),
(1, 1, 2, 1, '2024-01-17 11:45:00'),
(2, 1, 3, 2, '2024-01-18 14:30:00'),
(3, 1, 1, 3, '2024-01-19 08:15:00'),
(4, 1, 2, 4, '2024-01-20 10:50:00'),
(4, 2, 3, 6, '2024-01-21 13:25:00'),
(5, 1, 4, 5, '2024-01-22 15:40:00'),
(6, 1, 5, 6, '2024-01-23 09:55:00'),
(6, 5, 1, 8, '2024-01-24 11:10:00'),
(7, 1, 6, 7, '2024-01-25 14:35:00'),
(8, 1, 7, 8, '2024-01-26 16:50:00'),
(9, 1, 2, 9, '2024-01-27 10:05:00'),
(9, 2, 4, 11, '2024-01-28 12:20:00'),
(10, 1, 3, 10, '2024-01-29 15:45:00'),
(11, 1, 8, 11, '2024-02-02 09:30:00'),
(12, 1, 9, 12, '2024-02-03 13:15:00'),
(13, 1, 10, 13, '2024-02-04 11:40:00'),
(14, 1, 2, 14, '2024-02-05 14:55:00'),
(15, 1, 3, 15, '2024-02-06 16:25:00'),
(16, 1, 1, 16, '2024-02-07 10:10:00'),
(17, 1, 4, 17, '2024-02-08 12:30:00'),
(18, 1, 5, 18, '2024-02-09 15:45:00'),
(19, 1, 2, 19, '2024-02-10 09:20:00'),
(20, 1, 6, 20, '2024-02-11 14:35:00'),
(21, 1, 7, 21, '2024-02-12 11:50:00'),
(22, 1, 8, 22, '2024-02-13 13:10:00'),
(23, 1, 9, 23, '2024-02-14 16:30:00'),
(24, 1, 10, 24, '2024-02-15 10:55:00'),
(25, 1, 1, 25, '2024-02-16 12:15:00');

SELECT @@SERVERNAME AS ServerName;