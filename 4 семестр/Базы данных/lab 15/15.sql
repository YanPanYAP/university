-- Insert sample bank details
INSERT INTO Bank_Details (Bank_Name, BIC, Correspondent_Account, Settlement_Account, Account_Currency, Open_Date) VALUES 
('Sberbank', '044525225', '30101810400000000225', '40702810400000012345', 'RUB', '2020-01-15'),
('VTB', '044525187', '30101810700000000187', '40702810700000023456', 'RUB', '2019-05-20'),
('Alfa-Bank', '044525593', '30101810200000000593', '40702810200000034567', 'RUB', '2021-03-10'),
('Gazprombank', '044525823', '30101810800000000823', '40702810800000045678', 'USD', '2018-11-05'),
('Tinkoff', '044525974', '30101810900000000974', '40702810900000056789', 'RUB', '2022-02-28'),
('Raiffeisenbank', '044525201', '30101810100000000201', '40702810100000067890', 'EUR', '2020-07-15'),
('Otkritie Bank', '044525111', '30101810500000000111', '40702810500000078901', 'RUB', '2019-09-01');

-- Insert sample contacts
INSERT INTO Contacts (Last_Name, First_Name, Middle_Name, Position, Phone, Email, Birth_Date, Hire_Date) VALUES 
('Ivanov', 'Ivan', 'Ivanovich', 'CEO', '+79161234567', 'ivanov@mail.com', '1980-05-15', '2015-06-10'),
('Petrov', 'Petr', 'Petrovich', 'CFO', '+79167654321', 'petrov@mail.com', '1975-08-20', '2016-03-15'),
('Sidorova', 'Maria', 'Sergeevna', 'Chief Accountant', '+79169876543', 'sidorova@mail.com', '1985-02-28', '2018-11-01'),
('Kuznetsov', 'Alexey', 'Vladimirovich', 'Development Director', '+79161112233', 'kuznetsov@mail.com', '1978-07-10', '2017-05-20'),
('Smirnova', 'Elena', 'Andreevna', 'Commercial Director', '+79163334455', 'smirnova@mail.com', '1982-11-25', '2019-02-15'),
('Vasiliev', 'Dmitry', 'Olegovich', 'Technical Director', '+79165556677', 'vasiliev@mail.com', '1973-04-05', '2014-08-01'),
('Nikolaeva', 'Olga', 'Igorevna', 'HR Director', '+79167778899', 'nikolaeva@mail.com', '1988-09-12', '2020-07-10');

-- Insert sample enterprises
INSERT INTO Enterprises (Enterprise_Name, Legal_Address, Actual_Address, INN, KPP, OGRN, Registration_Date, Bank_ID, Contact_ID) VALUES 
('Alpha LLC', 'Moscow, Lenin St. 1', 'Moscow, Lenin St. 1, Office 101', '7701123456', '770101001', '1157746123456', '2015-01-20', 1, 1),
('Beta Inc.', 'Saint Petersburg, Nevsky Ave. 10', 'Saint Petersburg, Nevsky Ave. 10, Office 5', '7802987654', '780201001', '1157846987654', '2016-03-15', 2, 2),
('Gamma LLC', 'Yekaterinburg, Malysheva St. 25', 'Yekaterinburg, Malysheva St. 25, Office 30', '6605123456', '660501001', '1167746123456', '2017-05-10', 3, 3),
('Delta JSC', 'Novosibirsk, Kirova St. 15', 'Novosibirsk, Kirova St. 15, Office 12', '5406987654', '540601001', '1177846987654', '2018-07-20', 4, 4),
('Epsilon LLC', 'Kazan, Bauman St. 5', 'Kazan, Bauman St. 5, Office 8', '1605123456', '160501001', '1187746123456', '2019-09-05', 5, 5),
('Zeta PJSC', 'Sochi, Navaginskaya St. 7', 'Sochi, Navaginskaya St. 7, Office 3', '2306987654', '230601001', '1197846987654', '2020-11-15', 6, 6),
('Eta LLC', 'Vladivostok, Svetlanskaya St. 20', 'Vladivostok, Svetlanskaya St. 20, Office 15', '2505123456', '250501001', '1207746123456', '2021-01-30', 7, 7);

-- Insert sample indicators
INSERT INTO Indicators (Indicator_Name, Description, Unit, Importance, Category, Min_Value, Max_Value, Created_At) VALUES 
('Profit', 'Net profit over a period', 'RUB', 'High', 'Finance', NULL, NULL, '2020-01-01'),
('Cost of Goods Sold', 'Total product cost', 'RUB', 'Medium', 'Finance', NULL, NULL, '2020-01-01'),
('Revenue', 'Sales volume', 'RUB', 'High', 'Finance', 0, NULL, '2020-01-01'),
('ROE', 'Return on equity', '%', 'High', 'Finance', NULL, NULL, '2020-02-15'),
('ROA', 'Return on assets', '%', 'Medium', 'Finance', NULL, NULL, '2020-02-15'),
('Staff Count', 'Average headcount', 'people', 'Medium', 'HR', 1, NULL, '2020-03-10'),
('Payroll Fund', 'Salary expenses', 'RUB', 'Medium', 'HR', NULL, NULL, '2020-03-10'),
('Labor Productivity', 'Revenue per employee', 'RUB/employee', 'Medium', 'HR', NULL, NULL, '2020-04-01');

-- 1
CREATE TABLE TR_AUDIT (
    ID INT IDENTITY PRIMARY KEY,
    STMT VARCHAR(10),
    TRNAME VARCHAR(50),
    CC VARCHAR(MAX),
    EventDate DATETIME DEFAULT GETDATE()
);

CREATE TRIGGER TR_ENTERPRISE_INS
ON Enterprises
AFTER INSERT
AS
BEGIN
    DECLARE @data VARCHAR(MAX);
    
    SELECT @data = 'Added: ' + Enterprise_Name + ', INN: ' + INN + ', Address: ' + Legal_Address
    FROM inserted;
    
    INSERT INTO TR_AUDIT (STMT, TRNAME, CC)
    VALUES ('INS', 'TR_ENTERPRISE_INS', @data);
END;

INSERT INTO Enterprises (Enterprise_Name, Legal_Address, Actual_Address, INN, KPP, OGRN, Registration_Date, Bank_ID, Contact_ID) VALUES 
('Alpha', 'Moscow, Lenin St. 1', 'Moscow, Lenin St. 1, Office 101', '7704123450', '770151001', '1157746123450', '2015-01-20', 1, 1);

-- 2 
CREATE TRIGGER TR_ENTERPRISE_DEL
ON Enterprises
AFTER DELETE
AS
BEGIN
    DECLARE @data VARCHAR(MAX);
    
    SELECT @data = 'Deleted: ' + Enterprise_Name + ', INN: ' + INN
    FROM deleted;
    
    INSERT INTO TR_AUDIT (STMT, TRNAME, CC)
    VALUES ('DEL', 'TR_ENTERPRISE_DEL', @data);
END;

DELETE FROM Enterprises WHERE Enterprise_Name = 'Updated Alpha';

-- 3 
CREATE TRIGGER TR_ENTERPRISE_UPD
ON Enterprises
AFTER UPDATE
AS
BEGIN
    DECLARE @data VARCHAR(MAX);
    
    SELECT @data = 'Before: ' + d.Enterprise_Name + ', ' + d.Legal_Address + 
                  ' | After: ' + i.Enterprise_Name + ', ' + i.Legal_Address
    FROM deleted d
    JOIN inserted i ON d.Enterprise_ID = i.Enterprise_ID;
    
    INSERT INTO TR_AUDIT (STMT, TRNAME, CC)
    VALUES ('UPD', 'TR_ENTERPRISE_UPD', @data);
END;

UPDATE Enterprises SET Enterprise_Name = 'Updated Alpha' 
WHERE Enterprise_Name = 'Alpha';

-- 4 
CREATE TRIGGER TR_ENTERPRISE_ALL
ON Enterprises
AFTER INSERT, UPDATE, DELETE
AS
BEGIN
    DECLARE @event VARCHAR(10);
    DECLARE @data VARCHAR(MAX);
    
    IF EXISTS (SELECT * FROM inserted) AND NOT EXISTS (SELECT * FROM deleted)
    BEGIN
        SET @event = 'INS';
        SELECT @data = 'Added: ' + Enterprise_Name + ', INN: ' + INN
        FROM inserted;
    END
    ELSE IF EXISTS (SELECT * FROM deleted) AND NOT EXISTS (SELECT * FROM inserted)
    BEGIN
        SET @event = 'DEL';
        SELECT @data = 'Deleted: ' + Enterprise_Name + ', INN: ' + INN
        FROM deleted;
    END
    ELSE
    BEGIN
        SET @event = 'UPD';
        SELECT @data = 'Before: ' + d.Enterprise_Name + ' | After: ' + i.Enterprise_Name
        FROM deleted d
        JOIN inserted i ON d.Enterprise_ID = i.Enterprise_ID;
    END
    
    INSERT INTO TR_AUDIT (STMT, TRNAME, CC)
    VALUES (@event, 'TR_ENTERPRISE_ALL', @data);
END;

INSERT INTO Enterprises (Enterprise_Name, Legal_Address, Actual_Address, INN, KPP, OGRN, Registration_Date, Bank_ID, Contact_ID) VALUES 
('Alpha', 'Moscow, Lenin St. 1', 'Moscow, Lenin St. 1, Office 101', '7704123450', '770151001', '1157746123450', '2015-01-20', 1, 1);

UPDATE Enterprises SET Enterprise_Name = 'Updated Alpha' 
WHERE Enterprise_Name = 'Alpha';

DELETE FROM Enterprises WHERE Enterprise_Name = 'Updated Alpha';

-- 5 
ALTER TABLE Enterprises
ADD CONSTRAINT CHK_INN CHECK (LEN(INN) = 10);

BEGIN TRY
    INSERT INTO Enterprises (Enterprise_Name, Legal_Address, Actual_Address, INN, KPP, OGRN, Registration_Date, Bank_ID, Contact_ID) VALUES 
('Alpha', 'Moscow, Lenin St. 1', 'Moscow, Lenin St. 1, Office 101', '77041234565', '770151001', '1157746123459', '2015-01-20', 1, 1)
END TRY
BEGIN CATCH
    PRINT 'Error: ' + ERROR_MESSAGE();
    SELECT COUNT(*) AS 'List TR_AUDIT' FROM TR_AUDIT 
    WHERE CC LIKE '%Test Corp%';
END CATCH;

INSERT INTO Enterprises (Enterprise_Name, Legal_Address, Actual_Address, INN, KPP, OGRN, Registration_Date, Bank_ID, Contact_ID) VALUES 
('Alpha', 'Moscow, Lenin St. 1', 'Moscow, Lenin St. 1, Office 101', '7704123450', '770151001', '1157746123450', '2015-01-20', 1, 1);

DELETE FROM Enterprises WHERE Enterprise_Name = 'Updated Alpha';

-- 6
CREATE TRIGGER TR_ENTERPRISE_DEL1
ON Enterprises
AFTER DELETE
AS
BEGIN
    INSERT INTO TR_AUDIT (STMT, TRNAME, CC)
    SELECT 'DEL', 'TR_ENTERPRISE_DEL1', 'DEL1: ' + Enterprise_Name
    FROM deleted;
END;

CREATE TRIGGER TR_ENTERPRISE_DEL2
ON Enterprises
AFTER DELETE
AS
BEGIN
    INSERT INTO TR_AUDIT (STMT, TRNAME, CC)
    SELECT 'DEL', 'TR_ENTERPRISE_DEL2', 'DEL2: ' + Enterprise_Name
    FROM deleted;
END;

CREATE TRIGGER TR_ENTERPRISE_DEL3
ON Enterprises
AFTER DELETE
AS
BEGIN
    INSERT INTO TR_AUDIT (STMT, TRNAME, CC)
    SELECT 'DEL', 'TR_ENTERPRISE_DEL3', 'DEL3: ' + Enterprise_Name
    FROM deleted;
END;

EXEC sp_settriggerorder 
    @triggername = 'TR_ENTERPRISE_DEL3',
    @order = 'First',
    @stmttype = 'DELETE';

EXEC sp_settriggerorder 
    @triggername = 'TR_ENTERPRISE_DEL2',
    @order = 'Last',
    @stmttype = 'DELETE';

SELECT t.name, e.type_desc,
    CASE 
        WHEN t.name = 'TR_ENTERPRISE_DEL3' THEN 'First'
        WHEN t.name = 'TR_ENTERPRISE_DEL2' THEN 'Last'
        ELSE 'None'
    END AS execution_order
FROM sys.triggers t 
JOIN sys.trigger_events e ON t.object_id = e.object_id  
WHERE OBJECT_NAME(t.parent_id) = 'Enterprises' AND e.type_desc = 'DELETE';

INSERT INTO Enterprises (Enterprise_Name, Legal_Address, Actual_Address, INN, KPP, OGRN, Registration_Date, Bank_ID, Contact_ID) VALUES 
('Alpha', 'Moscow, Lenin St. 1', 'Moscow, Lenin St. 1, Office 101', '7704123450', '770151001', '1157746123450', '2015-01-20', 1, 1);

DELETE FROM Enterprises WHERE Enterprise_Name = 'Alpha';

--7 
CREATE TRIGGER trg_CheckEnterprise
ON Enterprises
AFTER INSERT, UPDATE
AS
BEGIN
    DECLARE @Count INT = (SELECT COUNT(*) FROM Enterprises);
    
    IF (@Count > 6)
    BEGIN
        RAISERROR('No more Ent', 16, 1);
        ROLLBACK;
    END;
    RETURN;
END;

INSERT INTO Enterprises (
    Enterprise_Name, Legal_Address, Actual_Address, INN, KPP, OGRN,
    Registration_Date, Bank_ID, Contact_ID
) VALUES (
    'FailCorp', 'Moscow, Red St. 1', 'Moscow, Red St. 1, Office 13',
    '7701666777', '770101005', '1237746123456', '2023-02-01', 4, 4
);

--8
CREATE TRIGGER TR_BANK_DETAILS_DEL
ON Bank_Details
INSTEAD OF DELETE
AS
BEGIN
    RAISERROR('You can not delet!', 16, 1);
END;

DELETE FROM Bank_Details WHERE Bank_ID = 1;

--9
CREATE TRIGGER DDL_BANK_DB
ON DATABASE
FOR CREATE_TABLE, DROP_TABLE, ALTER_TABLE
AS
BEGIN
    DECLARE @event XML = EVENTDATA();
    DECLARE @type VARCHAR(100) = @event.value('(/EVENT_INSTANCE/EventType)[1]', 'VARCHAR(100)');
    DECLARE @object VARCHAR(100) = @event.value('(/EVENT_INSTANCE/ObjectName)[1]', 'VARCHAR(100)');
    
    PRINT 'not DDL';
    PRINT 'type: ' + @type;
    PRINT 'jbject: ' + @object;
    
    RAISERROR('can not DDL!', 16, 1);
    ROLLBACK;
END;

BEGIN TRY
    CREATE TABLE TestTable (ID INT);
END TRY
BEGIN CATCH
    PRINT 'error: ' + ERROR_MESSAGE();
END CATCH;

DROP TRIGGER IF EXISTS TR_ENTERPRISE_INS;
DROP TRIGGER IF EXISTS TR_ENTERPRISE_DEL;
DROP TRIGGER IF EXISTS TR_ENTERPRISE_UPD;
DROP TRIGGER IF EXISTS TR_ENTERPRISE_ALL;
DROP TRIGGER IF EXISTS TR_ENTERPRISE_DEL1;
DROP TRIGGER IF EXISTS TR_ENTERPRISE_DEL2;
DROP TRIGGER IF EXISTS TR_ENTERPRISE_DEL3;
DROP TRIGGER IF EXISTS trg_AfterInsert_Enterprise;
DROP TRIGGER IF EXISTS TR_BANK_DETAILS_DEL;
DROP TRIGGER IF EXISTS DDL_BANK_DB ON DATABASE;
