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

--1 
SET NOCOUNT ON;

IF EXISTS (SELECT * FROM SYS.OBJECTS WHERE OBJECT_ID = OBJECT_ID(N'DBO.Test_Enterprises'))
    DROP TABLE Test_Enterprises;

DECLARE @c INT, @flag CHAR = 'r';

SET IMPLICIT_TRANSACTIONS ON;

CREATE TABLE Test_Enterprises (
    Enterprise_ID INT IDENTITY(1,1) PRIMARY KEY,
    Enterprise_Name NVARCHAR(100),
    Legal_Address NVARCHAR(200),
    Actual_Address NVARCHAR(200),
    INN VARCHAR(12),
    KPP VARCHAR(9),
    OGRN VARCHAR(13),
    Registration_Date DATE
);

INSERT INTO Test_Enterprises (Enterprise_Name, Legal_Address, Actual_Address, INN, KPP, OGRN, Registration_Date)
VALUES 
('Alpha LLC', 'Moscow, Lenin St. 1', 'Moscow, Lenin St. 1, Office 101', '7701123456', '770101001', '1157746123456', '2015-01-20'),
('Beta Inc.', 'Saint Petersburg, Nevsky Ave. 10', 'Saint Petersburg, Nevsky Ave. 10, Office 5', '7802987654', '780201001', '1157846987654', '2016-03-15'),
('Gamma LLC', 'Yekaterinburg, Malysheva St. 25', 'Yekaterinburg, Malysheva St. 25, Office 30', '6605123456', '660501001', '1167746123456', '2017-05-10');

SET @c = (SELECT COUNT(*) FROM Test_Enterprises);
PRINT 'Number of rows in the Test_Enterprises table: ' + CAST(@c AS VARCHAR(2));

IF @flag = 'c'
    COMMIT;
ELSE
    ROLLBACK;

SET IMPLICIT_TRANSACTIONS OFF;

IF EXISTS (SELECT * FROM SYS.OBJECTS WHERE OBJECT_ID = OBJECT_ID(N'DBO.Test_Enterprises'))
    PRINT 'Table Test_Enterprises exists';
ELSE
    PRINT 'Table Test_Enterprises does not exist';

--2

SELECT * FROM Bank_Details;
BEGIN TRY
    BEGIN TRANSACTION;
    DELETE FROM Bank_Details WHERE Bank_Name = 'NewBank_Static';  
    INSERT INTO Bank_Details (Bank_Name, BIC, Correspondent_Account, Settlement_Account, Account_Currency, Open_Date)
    VALUES ('NewBank', '034525000', '30101811000000000000', '40702820000000000000', 'USD', '2023-05-10');
    COMMIT TRANSACTION;
END TRY
BEGIN CATCH
    PRINT 'Error: ' + 
          CASE
              WHEN ERROR_NUMBER() = 2627 AND PATINDEX('%PK_Bank_Details%', ERROR_MESSAGE()) > 0
              THEN 'Duplicate Bank Entry'
              ELSE 'Unknown error: ' + CAST(ERROR_NUMBER() AS VARCHAR(5)) + ' - ' + ERROR_MESSAGE()
          END;
    IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
END CATCH;

--3

SELECT * FROM Bank_Details;
SELECT * FROM Contacts;
DECLARE @point VARCHAR(32);

BEGIN TRY
    BEGIN TRANSACTION;
    
    DELETE FROM Bank_Details WHERE Bank_Name = 'NewBank';
    SET @point = 'p1'; SAVE TRANSACTION @point;
    INSERT INTO Contacts (Last_Name, First_Name, Middle_Name, Position, Phone, Email, Birth_Date, Hire_Date)
    VALUES ('Test3', 'Nested3', 'Transaction3', 'Tester3', '+69000000000', 'test3@example.com', '1990-01-01', '2024-01-01');
    SET @point = 'p2'; SAVE TRANSACTION @point;
    INSERT INTO Contacts (Last_Name, First_Name, Middle_Name, Position, Phone, Email, Birth_Date, Hire_Date)
    VALUES ('Smirnova', 'Elena', 'Andreevna', 'Commercial Director', '+79161234567', 'smirnova@mail.com', '1982-11-25', '2019-02-15');
    
    COMMIT TRANSACTION;
END TRY
BEGIN CATCH
    PRINT 'Error: ' + CASE
        WHEN ERROR_NUMBER() = 2627 THEN 'Duplicate unique value (key violation).'
        ELSE 'Unknown error: ' + CAST(ERROR_NUMBER() AS VARCHAR(5)) + ' - ' + ERROR_MESSAGE()
    END;
    
    IF @@TRANCOUNT > 0
    BEGIN
        PRINT 'Rolling back transaction to checkpoint: ' + @point;
        ROLLBACK TRANSACTION @point;
    END
    COMMIT TRANSACTION;
END CATCH;

--4 
SELECT * FROM Enterprises;
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
BEGIN TRANSACTION;

SELECT @@SPID AS SessionID, 'READ UNCOMMITTED - Enterprises', * 
FROM Enterprises 
WHERE Enterprise_Name = 'Eta LLC';

COMMIT;

BEGIN TRANSACTION;

UPDATE Enterprises 
SET Enterprise_Name = 'Eta New LLC'
WHERE Enterprise_Name = 'Eta LLC';

ROLLBACK;

--5
SELECT * FROM Enterprises;
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRANSACTION;

SELECT @@SPID AS SessionID, 'READ COMMITTED - Enterprises', * 
FROM Enterprises 
WHERE Enterprise_Name = 'Eta LLC';

COMMIT;

BEGIN TRANSACTION;

UPDATE Enterprises 
SET Enterprise_Name = 'Eta New LLC'
WHERE Enterprise_Name = 'Eta LLC';

ROLLBACK;

--6
-- A
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
BEGIN TRANSACTION;

SELECT * FROM Enterprises WHERE Legal_Address LIKE 'Kazan%';

COMMIT;
--
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRANSACTION;

INSERT INTO Enterprises (
    Enterprise_Name, Legal_Address, Actual_Address,
    INN, KPP, OGRN, Registration_Date, Bank_ID, Contact_ID
) VALUES (
    'AAAAAA', 'Kazan, Kremlin St. 2', 'Kazan, Kremlin St. 2, Office 101',
    '1605999999', '160599001', '1187746999999', GETDATE(), 1, 1
);

COMMIT;
ROLLBACK;

--7 
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
BEGIN TRANSACTION;

SELECT * FROM Enterprises WHERE Legal_Address LIKE 'Kazan%';

COMMIT;
--
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRANSACTION;

INSERT INTO Enterprises (
    Enterprise_Name, Legal_Address, Actual_Address,
    INN, KPP, OGRN, Registration_Date, Bank_ID, Contact_ID
) VALUES (
    'AAAAAA', 'Kazan, Kremlin St. 2', 'Kazan, Kremlin St. 2, Office 101',
    '1605999999', '160599001', '1187746999999', GETDATE(), 1, 1
);

UPDATE Enterprises
SET 
    Enterprise_Name = 'AAAAAA',
    Legal_Address = 'Kazan, Kremlin St. 2',
    Actual_Address = 'Kazan, Kremlin St. 2, Office 101',
    INN = '1605999999',
    KPP = '160599001',
    OGRN = '1187746999999',
    Registration_Date = GETDATE(),
    Bank_ID = 1,
    Contact_ID = 1
WHERE Enterprise_ID = 3018;

COMMIT;
ROLLBACK;

--8
DELETE FROM Bank_Details WHERE Enterprise_Name = 'New Bank';
DELETE FROM Contacts WHERE Last_Name = 'Sergey';

BEGIN TRANSACTION
    DECLARE @NewBankID INT, @NewContactID INT;
    
    BEGIN TRANSACTION
        INSERT INTO Bank_Details VALUES ('New Bank', '044525999', '30101810900000000999', '40702810900000099999', 'RUB', GETDATE());
        SET @NewBankID = SCOPE_IDENTITY();
    COMMIT;

    BEGIN TRANSACTION
        INSERT INTO Contacts VALUES ('Novikov', 'Sergey', 'Viktorovich', 'IT Director', '+79169998877', 'novikov@mail.com', '1987-04-18', GETDATE());
        SET @NewContactID = SCOPE_IDENTITY();
    COMMIT;

    IF @@TRANCOUNT > 0 ROLLBACK;

SELECT 
    (SELECT COUNT(*) FROM Bank_Details WHERE Bank_ID = @NewBankID) AS 'Banks',
    (SELECT COUNT(*) FROM Contacts WHERE Contact_ID = @NewContactID) AS 'Contacts';