-- Bank Details
CREATE TABLE Bank_Details (
    Bank_ID INT IDENTITY(1,1) PRIMARY KEY,
    Bank_Name NVARCHAR(100) NOT NULL,
    BIC NVARCHAR(20) NOT NULL,
    Correspondent_Account NVARCHAR(20) NOT NULL,
    Settlement_Account NVARCHAR(20) NOT NULL UNIQUE,
    Account_Currency NVARCHAR(10) NOT NULL DEFAULT 'RUB',
    Open_Date DATE NOT NULL,
    CONSTRAINT unique_bank_account UNIQUE (BIC, Settlement_Account)
);

-- Contacts
CREATE TABLE Contacts (
    Contact_ID INT IDENTITY(1,1) PRIMARY KEY,
    Last_Name NVARCHAR(100) NOT NULL,
    First_Name NVARCHAR(100) NOT NULL,
    Middle_Name NVARCHAR(100),
    Position NVARCHAR(100) NOT NULL,
    Phone NVARCHAR(20) NOT NULL UNIQUE,
    Email NVARCHAR(100) UNIQUE,
    Birth_Date DATE,
    Hire_Date DATE NOT NULL
);

-- Enterprises
CREATE TABLE Enterprises (
    Enterprise_ID INT IDENTITY(1,1) PRIMARY KEY,
    Enterprise_Name NVARCHAR(255) NOT NULL,
    Legal_Address NVARCHAR(MAX) NOT NULL,
    Actual_Address NVARCHAR(MAX),
    INN NVARCHAR(12) NOT NULL UNIQUE,
    KPP NVARCHAR(9) NOT NULL,
    OGRN NVARCHAR(15) NOT NULL UNIQUE,
    Registration_Date DATE NOT NULL,
    Bank_ID INT,
    Contact_ID INT,
    FOREIGN KEY (Bank_ID) REFERENCES Bank_Details(Bank_ID),
    FOREIGN KEY (Contact_ID) REFERENCES Contacts(Contact_ID)
);

-- Indicators
CREATE TABLE Indicators (
    Indicator_ID INT IDENTITY(1,1) PRIMARY KEY,
    Indicator_Name NVARCHAR(255) NOT NULL UNIQUE,
    Description NVARCHAR(MAX),
    Unit NVARCHAR(20) NOT NULL,
    Importance NVARCHAR(10) NOT NULL CHECK (Importance IN ('High', 'Medium', 'Low')),
    Category NVARCHAR(100) NOT NULL,
    Min_Value DECIMAL(15,2),
    Max_Value DECIMAL(15,2),
    Created_At DATE NOT NULL
);

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
DECLARE @charVar CHAR(10) = 'CharData',
        @varcharVar VARCHAR(50) = 'VarcharData',
        @dateVar DATETIME,
        @timeVar TIME,
        @intVar INT,
        @smallIntVar SMALLINT,
        @tinyIntVar TINYINT,
        @numericVar NUMERIC(12,5);

SET @dateVar = GETDATE();
SET @timeVar = CAST(GETDATE() AS TIME);
SET @intVar = 100;
SET @smallIntVar = 50;
SET @tinyIntVar = 25;
SET @numericVar = 12345.67890;

SELECT @intVar = 200, @smallIntVar = 75;

SELECT @charVar AS CharVariable,
       @varcharVar AS VarcharVariable,
       @dateVar AS DateVariable,
       @timeVar AS TimeVariable;

PRINT 'Int Variable: ' + CAST(@intVar AS VARCHAR);
PRINT 'SmallInt Variable: ' + CAST(@smallIntVar AS VARCHAR);
PRINT 'TinyInt Variable: ' + CAST(@tinyIntVar AS VARCHAR);
PRINT 'Numeric Variable: ' + CAST(@numericVar AS VARCHAR);

-- 2
DECLARE @totalEnterprises INT;
SELECT @totalEnterprises = COUNT(*) FROM Enterprises;

IF @totalEnterprises > 5
BEGIN
    DECLARE @avgEnterprises FLOAT;
    SELECT @avgEnterprises = AVG(CAST(Enterprise_ID AS FLOAT)) FROM Enterprises;

    SELECT Enterprise_ID, Enterprise_Name
    FROM Enterprises
    WHERE Enterprise_ID < @avgEnterprises;

    DECLARE @belowAvgCount INT;
    SELECT @belowAvgCount = COUNT(*) FROM Enterprises WHERE Enterprise_ID < @avgEnterprises;

    PRINT 'Total Enterprises: ' + CAST(@totalEnterprises AS VARCHAR);
    PRINT 'Average Enterprise ID: ' + CAST(@avgEnterprises AS VARCHAR);
    PRINT 'Enterprises below average ID: ' + CAST(@belowAvgCount AS VARCHAR);
    PRINT 'Percentage below average: ' + CAST((@belowAvgCount * 100.0 / @totalEnterprises) AS VARCHAR) + '%';
END
ELSE
BEGIN
    PRINT 'Total number of enterprises is less than or equal to 5.';
END

-- 3
PRINT 'Number of rows affected: ' + CAST(@@ROWCOUNT AS VARCHAR);
PRINT 'SQL Server version: ' + CAST(@@VERSION AS VARCHAR);
PRINT 'Session ID: ' + CAST(@@SPID AS VARCHAR);
PRINT 'Last error code: ' + CAST(@@ERROR AS VARCHAR);
PRINT 'Server name: ' + CAST(@@SERVERNAME AS VARCHAR);
PRINT 'Transaction count: ' + CAST(@@TRANCOUNT AS VARCHAR);
PRINT 'Fetch status: ' + CAST(@@FETCH_STATUS AS VARCHAR);
PRINT 'Nesting level: ' + CAST(@@NESTLEVEL AS VARCHAR);

-- 4
SELECT Last_Name + ' ' + LEFT(First_Name, 1) + '. ' + LEFT(Middle_Name, 1) + '.' AS ShortName
FROM Contacts;

-- 5
DECLARE @enterpriseCount INT;
SELECT @enterpriseCount = COUNT(*) FROM Enterprises;

IF @enterpriseCount > 5
    PRINT 'More than 5.';
ELSE
    PRINT 'Equal 5 or less.';

-- 6
SELECT Enterprise_Name,
       CASE
           WHEN Registration_Date < '2018-01-01' THEN 'Old'
           WHEN Registration_Date BETWEEN '2018-01-01' AND '2020-12-31' THEN 'Just'
           ELSE 'New'
       END AS EnterpriseAgeCategory
FROM Enterprises;
--
DECLARE @t FLOAT = 2.5, @x FLOAT = 2.0;
DECLARE @z FLOAT;

IF @t > @x
    SET @z = POWER(SIN(@t), 2);
ELSE IF @t < @x
    SET @z = 4 * (@t + @x);
ELSE
    SET @z = 1 - EXP(@x - 2);

PRINT 'z = ' + CAST(@z AS VARCHAR(50));
--
DECLARE @BaseDate DATE = '2025-06-12';
DECLARE @NextMonth INT = MONTH(DATEADD(MONTH, 1, @BaseDate));
DECLARE @CurrentYear INT = YEAR(@BaseDate);
SELECT 
    CONCAT(Last_Name, ' ', First_Name, ' ', Middle_Name) AS Full_Name,
    Birth_Date,
    DATEDIFF(YEAR, Birth_Date, @BaseDate)
        - CASE 
            WHEN MONTH(Birth_Date) > MONTH(@BaseDate)
                 OR (MONTH(Birth_Date) = MONTH(@BaseDate) AND DAY(Birth_Date) > DAY(@BaseDate))
            THEN 1 ELSE 0 
          END AS Age_On_BaseDate
FROM Contacts
WHERE MONTH(Birth_Date) = @NextMonth
ORDER BY DAY(Birth_Date);
--
SELECT 
    DATENAME(WEEKDAY, Hire_Date) AS HireDayOfWeek,
    COUNT(*) AS NumberOfHires,
    CONCAT(Last_Name, ' ', First_Name) AS EmployeeName,
    Position,
    Hire_Date
FROM 
    Contacts
GROUP BY 
    DATENAME(WEEKDAY, Hire_Date),
    Last_Name,
    First_Name,
    Position,
    Hire_Date
ORDER BY 
    CASE DATENAME(WEEKDAY, Hire_Date)
        WHEN 'Monday' THEN 1
        WHEN 'Tuesday' THEN 2
        WHEN 'Wednesday' THEN 3
        WHEN 'Thursday' THEN 4
        WHEN 'Friday' THEN 5
        WHEN 'Saturday' THEN 6
        WHEN 'Sunday' THEN 7
    END,
    Hire_Date;

-- 7
CREATE TABLE #TempTable (
    ID INT,
    Description VARCHAR(100),
    RandomValue FLOAT
);

DECLARE @counter INT = 1;

WHILE @counter <= 10
BEGIN
    INSERT INTO #TempTable (ID, Description, RandomValue)
    VALUES (
        @counter,
        'Description ' + CAST(@counter AS VARCHAR),
        RAND() * 100
    );

    SET @counter = @counter + 1;
END

SELECT * FROM #TempTable;
DROP TABLE #TempTable;

-- 8
DECLARE @enterpriseCount INT;
SELECT @enterpriseCount = COUNT(*) FROM Enterprises;
PRINT 'Amount: ' + CAST(@enterpriseCount AS VARCHAR);
RETURN;

PRINT 'Just a text.';
PRINT 'Another text.';

-- 9
BEGIN TRY
    INSERT INTO Bank_Details (Bank_Name, BIC, Correspondent_Account, Settlement_Account, Account_Currency, Open_Date)
    VALUES ('Test Bank', '044525225', '30101810400000000225', '40702810400000012345', 'RUB', GETDATE());
END TRY
BEGIN CATCH
    PRINT 'An error occurred:';
    PRINT 'Error number: ' + CAST(ERROR_NUMBER() AS VARCHAR);
    PRINT 'Error message: ' + ERROR_MESSAGE();
    PRINT 'Error line: ' + CAST(ERROR_LINE() AS VARCHAR);
    PRINT 'Procedure: ' + ISNULL(ERROR_PROCEDURE(), 'N/A');
    PRINT 'Severity level: ' + CAST(ERROR_SEVERITY() AS VARCHAR);
    PRINT 'Error state: ' + CAST(ERROR_STATE() AS VARCHAR);
END CATCH;

