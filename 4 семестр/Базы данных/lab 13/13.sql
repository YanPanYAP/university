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
CREATE PROCEDURE PSUBJECT
AS
BEGIN
    DECLARE @RowCount INT = (SELECT COUNT(*) FROM Enterprises);
    SELECT * FROM Enterprises;
    RETURN @RowCount;
END

DECLARE @Count INT;
EXEC @Count = PSUBJECT;
PRINT 'Enterprises Count = ' + CAST(@Count AS VARCHAR(10));

--2 
ALTER PROCEDURE PSUBJECT
    @p VARCHAR(20) = NULL,
    @c INT OUTPUT
AS
BEGIN
    SELECT Enterprise_ID,Enterprise_Name,Legal_Address,Actual_Address,INN,KPP,OGRN,Registration_Date,Bank_ID,Contact_ID
    FROM Enterprises
    WHERE @p IS NULL OR Bank_ID = CASE WHEN ISNUMERIC(@p) = 1 THEN CAST(@p AS INT) ELSE NULL END;
    
    SELECT @c = COUNT(*)
    FROM Enterprises
    WHERE @p IS NULL OR Bank_ID = CASE WHEN ISNUMERIC(@p) = 1 THEN CAST(@p AS INT) ELSE NULL END;
    
    RETURN (SELECT COUNT(*) FROM Enterprises);
END;


DECLARE @rowCount INT, @returnValue INT;
EXEC @returnValue = PSUBJECT @c = @rowCount OUTPUT;
SELECT 
    'rowCount' = @rowCount,
    'Ent' = @returnValue;

DECLARE @rowCount INT, @returnValue INT;
EXEC @returnValue = PSUBJECT @p = '1', @c = @rowCount OUTPUT;
SELECT 
    'rowCount' = @rowCount,
    'Ent' = @returnValue;

--3
CREATE TABLE #SUBJECT (
    Enterprise_ID INT,
    Enterprise_Name NVARCHAR(100),
    Legal_Address NVARCHAR(200),
    Actual_Address NVARCHAR(200),
    INN VARCHAR(12),
    KPP VARCHAR(9),
    OGRN VARCHAR(15),
    Registration_Date DATE,
    Bank_ID INT,
    Contact_ID INT
);

ALTER PROCEDURE PSUBJECT
    @p VARCHAR(20) = NULL
AS
BEGIN
    SELECT Enterprise_ID,Enterprise_Name,Legal_Address,Actual_Address,INN,KPP,OGRN,Registration_Date,Bank_ID,Contact_ID
    FROM Enterprises
    WHERE @p IS NULL OR Bank_ID = CASE WHEN ISNUMERIC(@p) = 1 THEN CAST(@p AS INT) ELSE NULL END;
    
    RETURN (SELECT COUNT(*) FROM Enterprises);
END;

INSERT #SUBJECT EXEC PSUBJECT @p = '1';
INSERT #SUBJECT EXEC PSUBJECT @p = '2';
SELECT * FROM #SUBJECT;
DROP TABLE #SUBJECT;

--4 
CREATE PROCEDURE PAUDITORIUM_INSERT
    @a CHAR(20),
    @n VARCHAR(50),
    @c INT = NULL,
    @t CHAR(10)
AS
BEGIN
    DECLARE @rc INT = 1;
    
    BEGIN TRY
        INSERT INTO Enterprises (INN, Enterprise_Name, Bank_ID, Legal_Address,Actual_Address,KPP,OGRN,Registration_Date,Contact_ID)
        VALUES (@a,@n,@c,@t,@t,'000000000','0000000000000',GETDATE(),1);
        RETURN @rc;
    END TRY
    BEGIN CATCH
        PRINT 'Error adding enterprise:';
        PRINT 'Error number: ' + CAST(ERROR_NUMBER() AS VARCHAR(6));
        PRINT 'Severity level: ' + CAST(ERROR_SEVERITY() AS VARCHAR(6));
        PRINT 'Error message: ' + ERROR_MESSAGE();
        SET @rc = -1;
        RETURN @rc;
    END CATCH
END;

DECLARE @result INT;
EXEC @result = PAUDITORIUM_INSERT 
    @a = '123456789012', 
    @n = 'New Enterprise', 
    @c = 1, 
    @t = 'Moscow';
PRINT 'Execution result: ' + CAST(@result AS VARCHAR(2));
SELECT * FROM Enterprises WHERE INN = '123456789012';

DECLARE @result INT;
EXEC @result = PAUDITORIUM_INSERT 
    @a = '7701123456',
    @n = 'Duplicate', 
    @c = 1, 
    @t = 'Moscow';
PRINT 'Execution result: ' + CAST(@result AS VARCHAR(2));

DECLARE @result INT;
EXEC @result = PAUDITORIUM_INSERT 
    @a = '987654321098', 
    @n = 'Another Enterprise2', 
    @t = 'SPbb';
PRINT 'Execution result: ' + CAST(@result AS VARCHAR(2));
SELECT * FROM Enterprises WHERE INN = '987654321098';

DECLARE @result INT;
EXEC @result = PAUDITORIUM_INSERT 
    @a = 'invalid', 
    @n = 'Invalid Enterprise', 
    @c = 'not_a_number', 
    @t = 'Test';
PRINT 'Execution result: ' + CAST(@result AS VARCHAR(2));

DROP PROCEDURE PAUDITORIUM_INSERT;
DELETE FROM Enterprises WHERE INN IN ('123456789012', '987654321098');

--5
CREATE OR ALTER PROCEDURE SUBJECT_REPORT @p VARCHAR(10)
AS
BEGIN
    DECLARE @rc INT = 0;
    DECLARE @enterprise_name VARCHAR(50);
    DECLARE @result VARCHAR(MAX) = '';
    DECLARE @bank_id INT;
    
    BEGIN TRY
        -- Проверяем, является ли параметр числом
        IF ISNUMERIC(@p) = 0 RAISERROR('Parameter must be numeric', 11, 1);
        
        SET @bank_id = CAST(@p AS INT);
        
        IF NOT EXISTS (SELECT 1 FROM Bank_Details WHERE Bank_ID = @bank_id) 
            RAISERROR('Bank ID not found', 11, 1);
            
        PRINT 'Enterprises for Bank ID: ' + @p;
        PRINT '------------------------';
        
        DECLARE enterprise_cursor CURSOR FOR
        SELECT Enterprise_Name 
        FROM Enterprises 
        WHERE Bank_ID = @bank_id
        ORDER BY Enterprise_Name;
        
        OPEN enterprise_cursor;
        FETCH NEXT FROM enterprise_cursor INTO @enterprise_name;
        
        WHILE @@FETCH_STATUS = 0
        BEGIN
            SET @result = @result + RTRIM(@enterprise_name) + ', ';
            SET @rc = @rc + 1;
            FETCH NEXT FROM enterprise_cursor INTO @enterprise_name;
        END;
        
        IF LEN(@result) > 0
            SET @result = LEFT(@result, LEN(@result) - 1);
        PRINT @result;
        
        CLOSE enterprise_cursor;
        DEALLOCATE enterprise_cursor;
        
        RETURN @rc;
    END TRY
    BEGIN CATCH
        PRINT 'Error: ' + ERROR_MESSAGE();

        IF ERROR_PROCEDURE() IS NOT NULL
            PRINT 'Procedure name: ' + ERROR_PROCEDURE();
        
        IF CURSOR_STATUS('global', 'enterprise_cursor') >= 0
        BEGIN
            CLOSE enterprise_cursor;
            DEALLOCATE enterprise_cursor;
        END;
        
        RETURN -1; -- Возвращаем -1 при ошибке
    END CATCH;
END;

DECLARE @count INT;
EXEC @count = SUBJECT_REPORT @p = '1';
PRINT 'Number of enterprises: ' + CAST(@count AS VARCHAR(10));

DECLARE @count INT;
EXEC @count = SUBJECT_REPORT @p = '99';
PRINT 'Number of enterprises: ' + CAST(@count AS VARCHAR(10));

DECLARE @count INT;
EXEC @count = SUBJECT_REPORT @p = 'INVALID';
PRINT 'Number of enterprises: ' + CAST(@count AS VARCHAR(10));

--6 
CREATE PROCEDURE PAUDITORIUM_INSERT
    @a CHAR(20), @n VARCHAR(50), @c INT = NULL, @t CHAR(10)
AS
BEGIN
    DECLARE @rc INT = 1;
    BEGIN TRY
        IF @c IS NOT NULL AND NOT EXISTS (SELECT 1 FROM Bank_Details WHERE Bank_ID = @c)
        BEGIN
            PRINT 'Error: Bank_ID ' + CAST(@c AS VARCHAR(10)) + ' does not exist';
            RETURN -1;
        END
        INSERT INTO Enterprises (INN, Enterprise_Name, Bank_ID, Legal_Address, Actual_Address, KPP, OGRN, Registration_Date, Contact_ID)
        VALUES (@a, @n, @c, @t, @t, '000000000', '0000000000000', GETDATE(), 1);
        RETURN @rc;
    END TRY
    BEGIN CATCH
        PRINT 'Error adding enterprise:';
        PRINT 'Error number: ' + CAST(ERROR_NUMBER() AS VARCHAR(6));
        PRINT 'Error message: ' + ERROR_MESSAGE();
        RETURN -1;
    END CATCH;
END;

CREATE PROCEDURE PAUDITORIUM_INSERTX
    @a CHAR(20), @n VARCHAR(50), @c INT = NULL, @t CHAR(10), @tn VARCHAR(50)
AS
BEGIN
    DECLARE @rc INT = 1;
    BEGIN TRY
        SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
        BEGIN TRANSACTION;
        IF @c IS NULL OR NOT EXISTS (SELECT 1 FROM Bank_Details WHERE Bank_ID = @c)
        BEGIN
            DECLARE @new_bank_id INT;
            SELECT @new_bank_id = ISNULL(MAX(Bank_ID), 0) + 1 FROM Bank_Details;
            INSERT INTO Bank_Details (Bank_ID, Bank_Name, BIC, Correspondent_Account, Settlement_Account, Account_Currency, Open_Date)
            VALUES (@new_bank_id, @tn, '000000000', '00000000000000000000', '00000000000000000000', 'RUB', GETDATE());
            SET @c = @new_bank_id;
        END;
        DECLARE @insert_result INT;
        EXEC @insert_result = PAUDITORIUM_INSERT @a, @n, @c, @t;
        IF @insert_result = -1 RAISERROR('Error in PAUDITORIUM_INSERT', 16, 1);
        COMMIT TRANSACTION;
        RETURN @rc;
    END TRY
    BEGIN CATCH
        PRINT 'Error number: ' + CAST(ERROR_NUMBER() AS VARCHAR(6));
        PRINT 'Message: ' + ERROR_MESSAGE();
        PRINT 'Severity: ' + CAST(ERROR_SEVERITY() AS VARCHAR(6));
        IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
        RETURN -1;
    END CATCH;
END;

DECLARE @result INT; 
EXEC @result = PAUDITORIUM_INSERTX @a = '123456789012', @n = 'New Enterprise', @t = 'Moscow', @tn = 'Test Bank'; 
PRINT 'Result: ' + CAST(@result AS VARCHAR(2));
SELECT * FROM Bank_Details WHERE Bank_Name = 'Test Bank';
SELECT * FROM Enterprises WHERE INN = '123456789012';

DECLARE @result INT; 
EXEC @result = PAUDITORIUM_INSERTX @a = '987654321098', @n = 'Another Enterprise', @c = 1, @t = 'Moscow', @tn = 'Sberbank'; 
PRINT 'Result: ' + CAST(@result AS VARCHAR(2));
SELECT * FROM Enterprises WHERE INN = '987654321098';

DECLARE @result INT; 
EXEC @result = PAUDITORIUM_INSERTX @a = '7701123456', @n = 'Duplicate', @t = 'Test', @tn = 'Test Bank'; 
PRINT 'Result: ' + CAST(@result AS VARCHAR(2));

DELETE FROM Enterprises WHERE INN IN ('123456789012', '987654321098');
DELETE FROM Bank_Details WHERE Bank_Name = 'Test Bank';