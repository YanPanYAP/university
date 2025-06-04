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
CREATE FUNCTION COUNT_ENT(@bank VARCHAR(20))
RETURNS INT
AS
BEGIN
    DECLARE @count INT;
    
    SELECT @count = COUNT(*)
    FROM Enterprises e
    JOIN Bank_Details b ON e.Bank_ID = b.Bank_ID
    WHERE b.Bank_ID = @bank OR b.Bank_Name LIKE '%' + @bank + '%';
    
    RETURN @count;
END;

SELECT dbo.COUNT_ENT('1') AS sberbank_count;
SELECT dbo.COUNT_ENT(NULL) AS total_count;

CREATE OR ALTER FUNCTION COUNT_ENT(
    @bank VARCHAR(20) = NULL,
    @prof VARCHAR(20) = NULL
)
RETURNS INT
AS
BEGIN
    DECLARE @count INT;
    
    SELECT @count = COUNT(*)
    FROM Enterprises e
    JOIN Bank_Details b ON e.Bank_ID = b.Bank_ID
    WHERE 
        (@bank IS NULL 
         OR TRY_CONVERT(INT, @bank) = b.Bank_ID 
         OR b.Bank_Name LIKE '%' + @bank + '%')
        AND
        (@prof IS NULL 
         OR e.Enterprise_Name LIKE '%' + @prof + '%' 
         OR e.Legal_Address LIKE '%' + @prof + '%');
    
    RETURN @count;
END;

SELECT dbo.COUNT_ENT(NULL, NULL) AS total_enterprises;
SELECT dbo.COUNT_ENT('1', NULL) AS sberbank_by_id;
SELECT dbo.COUNT_ENT('Sberbank', NULL) AS sberbank_by_name;
SELECT dbo.COUNT_ENT(NULL, 'LLC') AS all_llc_enterprises;
SELECT dbo.COUNT_ENT('4', 'JSC') AS gazprom_jsc_count;
SELECT dbo.COUNT_ENT(NULL, 'Moscow') AS moscow_enterprises;

--2 
CREATE FUNCTION FIndicators(@p varchar(20)) 
RETURNS varchar(300) 
AS
BEGIN  
    DECLARE @indicator_name varchar(100);  
    DECLARE @result varchar(300) = 'Indicators: ';  
    
    DECLARE IndicatorCursor CURSOR LOCAL 
    FOR SELECT Indicator_Name 
        FROM Indicators 
        WHERE Category = @p;
    
    OPEN IndicatorCursor;	  
    FETCH IndicatorCursor INTO @indicator_name;   	 
    
    WHILE @@FETCH_STATUS = 0                                     
    BEGIN 
        SET @result = @result + ', ' + RTRIM(@indicator_name);         
        FETCH IndicatorCursor INTO @indicator_name; 
    END;    
    
    CLOSE IndicatorCursor;
    DEALLOCATE IndicatorCursor;
    
    RETURN @result;
END;

SELECT DISTINCT Category, dbo.FIndicators(Category) AS Indicator_List
FROM Indicators
ORDER BY Category;

--3
CREATE FUNCTION FBankIndic(
    @bank_name varchar(100) = NULL, 
    @indicator_name varchar(100) = NULL
)
RETURNS TABLE
AS
RETURN
    SELECT 
        b.Bank_Name,
        i.Indicator_Name,
        i.Description,
        i.Unit,
        i.Importance,
        i.Category
    FROM 
        Bank_Details b
    LEFT OUTER JOIN 
        Indicators i ON 1=1
    WHERE 
        (@bank_name IS NULL OR b.Bank_Name = @bank_name)
        AND 
        (@indicator_name IS NULL OR i.Indicator_Name = @indicator_name);

SELECT * FROM dbo.FBankIndic('VTB', NULL);
SELECT * FROM dbo.FBankIndic(NULL, 'VTB');
SELECT * FROM dbo.FBankIndic(NULL, NULL);
SELECT * FROM dbo.FBankIndic('VTB', 'Revenue');

--4 
CREATE FUNCTION FCTEACHER(@enterprise_name varchar(100))
RETURNS INT
AS
BEGIN
    DECLARE @count INT;
    
    IF @enterprise_name IS NULL
    BEGIN
        SELECT @count = COUNT(*) 
        FROM Contacts;
    END
    ELSE
    BEGIN
        SELECT @count = COUNT(c.Contact_ID)
        FROM Contacts c
        JOIN Enterprises e ON c.Contact_ID = e.Contact_ID
        WHERE e.Enterprise_Name = @enterprise_name;
    END
    
    RETURN @count;
END;

SELECT dbo.FCTEACHER(NULL) AS [Общее количество сотрудников];
SELECT dbo.FCTEACHER('Epsilon LLC') AS [Количество сотрудников в Epsilon LLC];
SELECT 
    e.Enterprise_Name AS [Предприятие],
    dbo.FCTEACHER(e.Enterprise_Name) AS [Количество сотрудников]
FROM 
    Enterprises e;

--6 
CREATE FUNCTION COUNT_ENTERPRISES(@bank_name varchar(100))
RETURNS INT
AS
BEGIN
    DECLARE @count INT;
    SELECT @count = COUNT(*) 
    FROM Enterprises 
    WHERE Bank_ID = (SELECT Bank_ID FROM Bank_Details WHERE Bank_Name = @bank_name);
    RETURN @count;
END;

CREATE FUNCTION COUNT_CONTACTS(@bank_name varchar(100))
RETURNS INT
AS
BEGIN
    DECLARE @count INT;
    SELECT @count = COUNT(*) 
    FROM Contacts c
    JOIN Enterprises e ON c.Contact_ID = e.Contact_ID
    WHERE e.Bank_ID = (SELECT Bank_ID FROM Bank_Details WHERE Bank_Name = @bank_name);
    RETURN @count;
END;

CREATE FUNCTION COUNT_INDICATORS(@bank_name varchar(100))
RETURNS INT
AS
BEGIN
    DECLARE @count INT;
    SELECT @count = COUNT(*) FROM Indicators;
    RETURN @count;
END;

CREATE FUNCTION BANK_REPORT(@min_enterprises int)
RETURNS @br TABLE (
    [Банк] varchar(50),
    [Количество предприятий] int,
    [Количество контактов] int,
    [Количество показателей] int
)
AS
BEGIN
    DECLARE bank_cursor CURSOR STATIC FOR 
    SELECT Bank_Name FROM Bank_Details 
    WHERE dbo.COUNT_ENTERPRISES(Bank_Name) > @min_enterprises;
    
    DECLARE @bank varchar(50);
    OPEN bank_cursor;
    FETCH bank_cursor INTO @bank;
    
    WHILE @@FETCH_STATUS = 0
    BEGIN
        INSERT @br VALUES(
            @bank,
            dbo.COUNT_ENTERPRISES(@bank),
            dbo.COUNT_CONTACTS(@bank),
            dbo.COUNT_INDICATORS(@bank)
        );
        FETCH bank_cursor INTO @bank;
    END;
    
    CLOSE bank_cursor;
    DEALLOCATE bank_cursor;
    RETURN;
END;

SELECT * FROM dbo.BANK_REPORT(1);