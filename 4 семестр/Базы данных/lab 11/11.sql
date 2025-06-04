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
DECLARE @bankName CHAR(50), @result VARCHAR(MAX) = '';
DECLARE BankCursor CURSOR
FOR SELECT Bank_Name FROM Bank_Details;

OPEN BankCursor;
FETCH NEXT FROM BankCursor INTO @bankName;

PRINT 'Bank list:';
WHILE @@FETCH_STATUS = 0
BEGIN
    SET @result = RTRIM(@bankName) + ',' + @result;
    FETCH NEXT FROM BankCursor INTO @bankName;
END;

IF RIGHT(@result, 1) = ',' 
    SET @result = LEFT(@result, LEN(@result) - 1);

PRINT @result;

CLOSE BankCursor;
DEALLOCATE BankCursor;

--2
DECLARE BankCursor CURSOR LOCAL
FOR SELECT Bank_Name, Account_Currency FROM Bank_Details;

DECLARE @name NVARCHAR(100), @currency CHAR(3);
OPEN BankCursor;
FETCH NEXT FROM BankCursor INTO @name, @currency;
PRINT '1. ' + @name + ' — ' + @currency;

GO

BEGIN TRY
    DECLARE @name NVARCHAR(100), @currency CHAR(3);
    FETCH NEXT FROM BankCursor INTO @name, @currency;
    PRINT '2. ' + @name + ' — ' + @currency;
END TRY
BEGIN CATCH
    PRINT 'Error: ' + ERROR_MESSAGE();
END CATCH
GO

--
DECLARE BankCursor CURSOR GLOBAL
FOR SELECT Bank_Name, Account_Currency FROM Bank_Details;

DECLARE @name NVARCHAR(100), @currency CHAR(3);
OPEN BankCursor;
FETCH NEXT FROM BankCursor INTO @name, @currency;
PRINT '1. ' + @name + ' — ' + @currency;
GO
DECLARE @name NVARCHAR(100), @currency CHAR(3);
FETCH NEXT FROM BankCursor INTO @name, @currency;
PRINT '2. ' + @name + ' — ' + @currency;
GO
CLOSE BankCursor;
DEALLOCATE BankCursor;
GO


--
BEGIN TRY
    DECLARE BankCursor CURSOR LOCAL
    FOR SELECT Bank_Name, Account_Currency FROM Bank_Details;

    DECLARE @name1 NVARCHAR(100), @currency1 CHAR(3);
    OPEN BankCursor;
    FETCH NEXT FROM BankCursor INTO @name1, @currency1;
    PRINT '1. ' + @name1 + ' - ' + @currency1;

    CLOSE BankCursor;
    DEALLOCATE BankCursor;

    DECLARE @name2 NVARCHAR(100), @currency2 CHAR(3);
    FETCH NEXT FROM BankCursor INTO @name2, @currency2;
    PRINT '2. ' + @name2 + ' - ' + @currency2;
END TRY
BEGIN CATCH
    PRINT 'Error: ' + ERROR_MESSAGE();
END CATCH

--
BEGIN TRY
    DECLARE BankCursor CURSOR GLOBAL
    FOR SELECT Bank_Name, Account_Currency FROM Bank_Details;

    DECLARE @gname1 NVARCHAR(100), @gcur1 CHAR(3);
    OPEN BankCursor;
    FETCH NEXT FROM BankCursor INTO @gname1, @gcur1;
    PRINT '1. ' + @gname1 + ' - ' + @gcur1;

    DECLARE @gname2 NVARCHAR(100), @gcur2 CHAR(3);
    FETCH NEXT FROM BankCursor INTO @gname2, @gcur2;
    PRINT '2. ' + @gname2 + ' - ' + @gcur2;

    CLOSE BankCursor;
    DEALLOCATE BankCursor;
END TRY
BEGIN CATCH
    PRINT 'Error: ' + ERROR_MESSAGE();
END CATCH

--3
DECLARE @bname NVARCHAR(100), @currency CHAR(3);

DECLARE bank_cursor CURSOR LOCAL STATIC
FOR
    SELECT Bank_Name, Account_Currency FROM Bank_Details;

OPEN bank_cursor;

PRINT 'STATIC cursor row count: ' + CAST(@@CURSOR_ROWS AS VARCHAR(5));

UPDATE Bank_Details SET Account_Currency = 'EUR' WHERE Bank_Name = 'Sberbank';
DELETE FROM Bank_Details WHERE Bank_Name = 'Tinkoff';
INSERT INTO Bank_Details (Bank_Name, BIC, Correspondent_Account, Settlement_Account, Account_Currency, Open_Date)
VALUES ('NewBank_Static', '000000000', '00000000000000000000', '00000000000000000000', 'USD', GETDATE());

PRINT 'Current data in Bank_Details after STATIC cursor changes:';
SELECT * FROM Bank_Details;

FETCH NEXT FROM bank_cursor INTO @bname, @currency;
WHILE @@FETCH_STATUS = 0
BEGIN
    PRINT @bname + ' - ' + @currency;
    FETCH NEXT FROM bank_cursor INTO @bname, @currency;
END

CLOSE bank_cursor;
DEALLOCATE bank_cursor;


DECLARE bank_cursor CURSOR LOCAL DYNAMIC
FOR
    SELECT Bank_Name, Account_Currency FROM Bank_Details;

OPEN bank_cursor;

PRINT 'DYNAMIC cursor row count: ' + CAST(@@CURSOR_ROWS AS VARCHAR(5));

UPDATE Bank_Details SET Account_Currency = 'USD' WHERE Bank_Name = 'Sberbank';
DELETE FROM Bank_Details WHERE Bank_Name = 'NewBank_Static';
INSERT INTO Bank_Details (Bank_Name, BIC, Correspondent_Account, Settlement_Account, Account_Currency, Open_Date)
VALUES ('NewBank_Dynamic', '111111111', '11111111111111111111', '11111111111111111111', 'RUB', GETDATE());

PRINT 'Current data in Bank_Details after DYNAMIC cursor changes:';
SELECT * FROM Bank_Details;

PRINT 'Fetching rows from dynamic cursor...';
FETCH NEXT FROM bank_cursor INTO @bname, @currency;
WHILE @@FETCH_STATUS = 0
BEGIN
    PRINT @bname + ' - ' + @currency;
    FETCH NEXT FROM bank_cursor INTO @bname, @currency;
END

CLOSE bank_cursor;
DEALLOCATE bank_cursor;

--4 
DECLARE @row_number INT, @bank_name NVARCHAR(100), @account_currency CHAR(3);

DECLARE bank_cursor CURSOR LOCAL SCROLL
FOR
    SELECT ROW_NUMBER() OVER (ORDER BY Bank_Name) AS N,
           Bank_Name, Account_Currency
    FROM Bank_Details;

OPEN bank_cursor;

FETCH FIRST FROM bank_cursor INTO @row_number, @bank_name, @account_currency;
PRINT 'FIRST: Row ' + CAST(@row_number AS VARCHAR(5)) + ' - ' + @bank_name + ' - ' + @account_currency;

FETCH NEXT FROM bank_cursor INTO @row_number, @bank_name, @account_currency;
PRINT 'NEXT: Row ' + CAST(@row_number AS VARCHAR(5)) + ' - ' + @bank_name + ' - ' + @account_currency;

FETCH PRIOR FROM bank_cursor INTO @row_number, @bank_name, @account_currency;
PRINT 'PRIOR: Row ' + CAST(@row_number AS VARCHAR(5)) + ' - ' + @bank_name + ' - ' + @account_currency;

FETCH LAST FROM bank_cursor INTO @row_number, @bank_name, @account_currency;
PRINT 'LAST: Row ' + CAST(@row_number AS VARCHAR(5)) + ' - ' + @bank_name + ' - ' + @account_currency;

FETCH ABSOLUTE 3 FROM bank_cursor INTO @row_number, @bank_name, @account_currency;
PRINT 'ABSOLUTE 3: Row ' + CAST(@row_number AS VARCHAR(5)) + ' - ' + @bank_name + ' - ' + @account_currency;

FETCH ABSOLUTE -3 FROM bank_cursor INTO @row_number, @bank_name, @account_currency;
PRINT 'ABSOLUTE -3: Row ' + CAST(@row_number AS VARCHAR(5)) + ' - ' + @bank_name + ' - ' + @account_currency;

FETCH RELATIVE 5 FROM bank_cursor INTO @row_number, @bank_name, @account_currency;
PRINT 'RELATIVE 5: Row ' + CAST(@row_number AS VARCHAR(5)) + ' - ' + @bank_name + ' - ' + @account_currency;

FETCH RELATIVE -5 FROM bank_cursor INTO @row_number, @bank_name, @account_currency;
PRINT 'RELATIVE -5: Row ' + CAST(@row_number AS VARCHAR(5)) + ' - ' + @bank_name + ' - ' + @account_currency;

CLOSE bank_cursor;
DEALLOCATE bank_cursor;

--5
DECLARE @Enterprise_Name NVARCHAR(100), @Bank_ID INT, @Staff_Count INT;

DECLARE EnterpriseCursor CURSOR LOCAL DYNAMIC
FOR
    SELECT E.Enterprise_Name, E.Bank_ID, I.Min_Value
    FROM Enterprises E
    JOIN Indicators I ON I.Category = 'HR'
    WHERE E.Bank_ID = 1;

OPEN EnterpriseCursor;

FETCH NEXT FROM EnterpriseCursor INTO @Enterprise_Name, @Bank_ID, @Staff_Count;

DELETE FROM Enterprises
WHERE CURRENT OF EnterpriseCursor;

FETCH NEXT FROM EnterpriseCursor INTO @Enterprise_Name, @Bank_ID, @Staff_Count;

UPDATE Enterprises
SET Bank_ID = Bank_ID + 1
WHERE CURRENT OF EnterpriseCursor;

CLOSE EnterpriseCursor;
DEALLOCATE EnterpriseCursor;

--6
DELETE E
FROM Enterprises E
JOIN Bank_Details BD ON E.Bank_ID = BD.Bank_ID
WHERE BD.Account_Currency < 'RUB';

UPDATE Enterprises
SET Bank_ID = Bank_ID + 1
WHERE Enterprise_ID = 3;



