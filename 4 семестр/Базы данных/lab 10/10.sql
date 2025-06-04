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

--1
SELECT
    t.name AS TableName,
    i.name AS IndexName,
    i.type_desc AS IndexType,
    i.is_primary_key,
    i.is_unique,
    i.is_unique_constraint
FROM
    sys.tables t
INNER JOIN
    sys.indexes i ON t.object_id = i.object_id
WHERE
    t.is_ms_shipped = 0 AND t.name IN ('Bank_Details', 'Contacts', 'Enterprises', 'Indicators')
ORDER BY
    t.name, i.name;
--
CREATE TABLE #EXPLRE (
    TIND INT,
    TName NVARCHAR(100),
    TValue DECIMAL(15,2),
    TDate DATE
);

DECLARE @i INT = 1;
WHILE @i <= 1000
BEGIN
    INSERT INTO #EXPLRE (TIND, TName, TValue, TDate)
    VALUES (
        @i,
        'Name_' + CAST(@i AS NVARCHAR(10)),
        ROUND(RAND() * 1000, 2),
        DATEADD(DAY, -CAST(RAND() * 365 AS INT), GETDATE())
    );
    SET @i = @i + 1;
END;

SELECT COUNT(*) FROM #EXPLRE;

SELECT * FROM #EXPLRE WHERE TIND BETWEEN 150 AND 250 ORDER BY TIND;

DROP TABLE #EXPLRE;

--
CREATE CLUSTERED INDEX #EXPLRE_CL ON #EXPLRE(TIND ASC);

CHECKPOINT;
DBCC DROPCLEANBUFFERS;

SELECT * FROM #EXPLRE WHERE TIND BETWEEN 150 AND 250 ORDER BY TIND;

-- 2
CREATE TABLE #EX (
    TKEY INT,
    CC INT IDENTITY(1, 1),
    TF VARCHAR(100)
);

SET NOCOUNT ON;
DECLARE @i INT = 0;
WHILE @i < 2000
BEGIN
    INSERT #EX(TKEY, TF)
    SELECT Contact_ID, CONCAT('Contact_', Contact_ID)
    FROM Contacts
    WHERE Contact_ID = @i;
    SET @i = @i + 1;
END;

SELECT COUNT(*) AS [Количество строк] FROM #EX;

CREATE INDEX #EX_NONCLU ON #EX(TKEY, CC);

SELECT * FROM #EX WHERE TKEY > 1500 AND CC < 4500;
SELECT * FROM #EX ORDER BY TKEY, CC;
SELECT * FROM #EX WHERE TKEY = 556 AND CC > 3;

CHECKPOINT;
DBCC DROPCLEANBUFFERS;

SELECT * FROM #EX WHERE TKEY = 556 AND CC > 3;

DROP TABLE #EX;

--3
CREATE TABLE #EX (
    TKEY INT,
    CC INT IDENTITY(1, 1),
    TF VARCHAR(100),
    TDATE DATETIME DEFAULT GETDATE(),
    TVALUE DECIMAL(15,2)
);

SET NOCOUNT ON;
DECLARE @i INT = 0;
WHILE @i < 1000
BEGIN
    INSERT INTO #EX(TKEY, TF, TVALUE)
    SELECT Enterprise_ID, Enterprise_Name, ROUND(RAND() * 10000, 2)
    FROM Enterprises
    WHERE Enterprise_ID = @i;
    SET @i = @i + 1;
END;

CREATE INDEX #EX_TKEY_X ON #EX(TKEY) INCLUDE (CC);

CHECKPOINT;
DBCC DROPCLEANBUFFERS;

SELECT CC FROM #EX WHERE TKEY > 15000;
SELECT CC FROM #EX WHERE TKEY BETWEEN 10000 AND 20000;
SELECT CC FROM #EX WHERE TKEY = 15555;

DROP TABLE #EX;

--4 
CREATE TABLE #EX (
    TKEY INT,
    CC INT IDENTITY(1,1),
    TF VARCHAR(100),
    TDATE DATETIME DEFAULT GETDATE(),
    TVALUE DECIMAL(15,2)
);

SET NOCOUNT ON;
DECLARE @i INT = 0;
WHILE @i < 20000
BEGIN
    INSERT INTO #EX(TKEY, TF, TVALUE)
    SELECT Indicator_ID, Indicator_Name, ROUND(RAND() * 1000, 2)
    FROM Indicators
    WHERE Indicator_ID = @i;
    SET @i = @i + 1;
END;

SELECT COUNT(*) AS [Total Rows] FROM #EX;

SELECT TKEY FROM #EX WHERE TKEY BETWEEN 5000 AND 19999;
SELECT TKEY FROM #EX WHERE TKEY > 15000 AND TKEY < 20000;
SELECT TKEY FROM #EX WHERE TKEY = 17000;

CREATE INDEX #EX_WHERE ON #EX(TKEY) WHERE (TKEY >= 15000 AND TKEY < 20000);

CHECKPOINT;
DBCC DROPCLEANBUFFERS;

SELECT TKEY FROM #EX WHERE TKEY BETWEEN 5000 AND 19999;
SELECT TKEY FROM #EX WHERE TKEY > 15000 AND TKEY < 20000;
SELECT TKEY FROM #EX WHERE TKEY = 17000;

DROP TABLE #EX;

--5 
CREATE TABLE #EX (
    TKEY INT PRIMARY KEY,
    TF NVARCHAR(100)
);

DECLARE @i INT = 1;
WHILE @i <= 10000
BEGIN
    INSERT INTO #EX (TKEY, TF)
    SELECT Bank_ID, Bank_Name
    FROM Bank_Details
    WHERE Bank_ID = @i;
    SET @i += 1;
END;

CREATE INDEX #EX_TKEY ON #EX(TKEY);

use tempdb
SELECT
    name AS [Индекс],
    avg_fragmentation_in_percent AS [Фрагментация (%)],
    page_count
FROM sys.dm_db_index_physical_stats(DB_ID(N'tempdb'), OBJECT_ID(N'tempdb..#EX'), NULL, NULL, 'LIMITED') AS ss
JOIN sys.indexes ii ON ss.object_id = ii.object_id AND ss.index_id = ii.index_id
WHERE name IS NOT NULL;

INSERT TOP(10000) INTO #EX (TKEY, TF)
SELECT Bank_ID + 100000, Bank_Name FROM #EX;

SELECT
    name AS [Индекс],
    avg_fragmentation_in_percent AS [Фрагментация (%)],
    page_count
FROM sys.dm_db_index_physical_stats(DB_ID(N'tempdb'), OBJECT_ID(N'tempdb..#EX'), NULL, NULL, 'LIMITED') AS ss
JOIN sys.indexes ii ON ss.object_id = ii.object_id AND ss.index_id = ii.index_id
WHERE name IS NOT NULL;

ALTER INDEX #EX_TKEY ON #EX REORGANIZE;

SELECT
    name AS [Индекс],
    avg_fragmentation_in_percent AS [Фрагментация (%)],
    page_count
FROM sys.dm_db_index_physical_stats(DB_ID(N'tempdb'), OBJECT_ID(N'tempdb..#EX'), NULL, NULL, 'LIMITED') AS ss
JOIN sys.indexes ii ON ss.object_id = ii.object_id AND ss.index_id = ii.index_id
WHERE name IS NOT NULL;

ALTER INDEX #EX_TKEY ON #EX REBUILD WITH (ONLINE = OFF);

SELECT
    name AS [Индекс],
    avg_fragmentation_in_percent AS [Фрагментация (%)],
    page_count
FROM sys.dm_db_index_physical_stats(DB_ID(N'tempdb'), OBJECT_ID(N'tempdb..#EX'), NULL, NULL, 'LIMITED') AS ss
JOIN sys.indexes ii ON ss.object_id = ii.object_id AND ss.index_id = ii.index_id
WHERE name IS NOT NULL;

DROP TABLE #EX;

--6
DECLARE @obj_id INT = OBJECT_ID('tempdb..#EX');

IF @obj_id IS NOT NULL
BEGIN
    SELECT
        ii.name AS [Индекс],
        ss.avg_fragmentation_in_percent AS [Фрагментация (%)],
        ss.page_count
    FROM sys.dm_db_index_physical_stats(DB_ID('tempdb'), @obj_id, NULL, NULL, NULL) AS ss
    JOIN sys.indexes ii ON ss.object_id = ii.object_id AND ss.index_id = ii.index_id
    WHERE ii.name IS NOT NULL;
END
ELSE
BEGIN
    PRINT 'not found.';
END;

SELECT COUNT(*) FROM #EX;
SELECT TOP 10 * FROM #EX ORDER BY TKEY;

