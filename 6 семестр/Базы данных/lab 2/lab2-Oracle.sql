//system
CREATE USER task_control IDENTIFIED BY MyPass123
DEFAULT TABLESPACE users
QUOTA UNLIMITED ON users;

GRANT CONNECT TO task_control;
GRANT CREATE TABLE TO task_control;
GRANT CREATE VIEW TO task_control;
GRANT CREATE SEQUENCE TO task_control;
GRANT CREATE TRIGGER TO task_control;
GRANT CREATE PROCEDURE TO task_control;
GRANT CREATE TYPE TO task_control;
GRANT CREATE SYNONYM TO task_control;
GRANT CREATE MATERIALIZED VIEW TO task_control;
GRANT UNLIMITED TABLESPACE TO task_control;

//task_control
CREATE SEQUENCE seq_role START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE seq_department START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE seq_user START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE seq_status START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE seq_task START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE seq_comment START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE seq_attachment START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE seq_history START WITH 1 INCREMENT BY 1;

CREATE TABLE Role (
    role_id NUMBER PRIMARY KEY,
    role_name VARCHAR2(100) UNIQUE NOT NULL,
    description VARCHAR2(255)
);

CREATE TABLE Department (
    department_id NUMBER PRIMARY KEY,
    department_name VARCHAR2(200) NOT NULL,
    parent_department_id NUMBER,
    CONSTRAINT FK_Department_Parent
        FOREIGN KEY (parent_department_id)
        REFERENCES Department(department_id)
);

CREATE TABLE Users (
    user_id NUMBER PRIMARY KEY,
    full_name VARCHAR2(200) NOT NULL,
    email VARCHAR2(200) UNIQUE NOT NULL,
    password VARCHAR2(255) NOT NULL,
    role_id NUMBER NOT NULL,
    department_id NUMBER,
    CONSTRAINT FK_User_Role FOREIGN KEY (role_id)
        REFERENCES Role(role_id),
    CONSTRAINT FK_User_Department FOREIGN KEY (department_id)
        REFERENCES Department(department_id)
);

CREATE TABLE Status (
    status_id NUMBER PRIMARY KEY,
    status_name VARCHAR2(100) UNIQUE NOT NULL
);

CREATE TABLE Task (
    task_id NUMBER PRIMARY KEY,
    title VARCHAR2(255) NOT NULL,
    description CLOB,
    creator_id NUMBER NOT NULL,
    executor_id NUMBER NOT NULL,
    status_id NUMBER NOT NULL,
    priority NUMBER,
    start_date DATE,
    due_date DATE,
    created_at DATE DEFAULT SYSDATE,
    CONSTRAINT FK_Task_Creator FOREIGN KEY (creator_id)
        REFERENCES Users(user_id),
    CONSTRAINT FK_Task_Executor FOREIGN KEY (executor_id)
        REFERENCES Users(user_id),
    CONSTRAINT FK_Task_Status FOREIGN KEY (status_id)
        REFERENCES Status(status_id)
);

CREATE TABLE Task_Comment (
    comment_id NUMBER PRIMARY KEY,
    task_id NUMBER NOT NULL,
    user_id NUMBER NOT NULL,
    comment_text CLOB,
    created_at DATE DEFAULT SYSDATE,
    CONSTRAINT FK_TaskComment_Task FOREIGN KEY (task_id)
        REFERENCES Task(task_id) ON DELETE CASCADE,
    CONSTRAINT FK_TaskComment_User FOREIGN KEY (user_id)
        REFERENCES Users(user_id)
);

CREATE TABLE Attachment (
    attachment_id NUMBER PRIMARY KEY,
    task_id NUMBER NOT NULL,
    file_name VARCHAR2(255),
    file_path VARCHAR2(500),
    uploaded_at DATE DEFAULT SYSDATE,
    CONSTRAINT FK_Attachment_Task FOREIGN KEY (task_id)
        REFERENCES Task(task_id) ON DELETE CASCADE
);

CREATE TABLE TaskHistory (
    history_id NUMBER PRIMARY KEY,
    task_id NUMBER NOT NULL,
    old_status_id NUMBER,
    new_status_id NUMBER,
    changed_by NUMBER,
    changed_at DATE DEFAULT SYSDATE,
    CONSTRAINT FK_History_Task FOREIGN KEY (task_id)
        REFERENCES Task(task_id) ON DELETE CASCADE,
    CONSTRAINT FK_History_User FOREIGN KEY (changed_by)
        REFERENCES Users(user_id)
);

CREATE INDEX IX_Task_Status ON Task(status_id);
CREATE INDEX IX_Task_Executor ON Task(executor_id);
CREATE INDEX IX_Comment_Task ON Task_Comment(task_id);

CREATE OR REPLACE VIEW vw_task_report AS
SELECT 
    t.task_id,
    t.title,
    s.status_name,
    u1.full_name AS creator_name,
    u2.full_name AS executor_name
FROM Task t
JOIN Status s ON t.status_id = s.status_id
JOIN Users u1 ON t.creator_id = u1.user_id
JOIN Users u2 ON t.executor_id = u2.user_id;