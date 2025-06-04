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
    E.Enterprise_Name AS 'Enterprise/@Name',
    E.INN AS 'Enterprise/@INN',
    E.OGRN AS 'Enterprise/@OGRN',
    (
        SELECT 
            B.Bank_Name AS 'Bank_Name',
            B.BIC AS 'BIC',
            B.Settlement_Account AS 'Settlement_Account'
        FROM Bank_Details B
        WHERE B.Bank_ID = E.Bank_ID
        FOR XML PATH('Bank'), TYPE
    )
FROM Enterprises E
FOR XML PATH('Enterprise'), ROOT('Enterprises');


--2
SELECT 
    E.Enterprise_Name,
    E.INN,
    B.Bank_Name,
    C.Last_Name,
    C.Position
FROM Enterprises E
JOIN Bank_Details B ON E.Bank_ID = B.Bank_ID
JOIN Contacts C ON E.Contact_ID = C.Contact_ID
FOR XML AUTO, ROOT('EnterpriseData');


--3
DECLARE @xml XML = N'
<Indicators>
    <Indicator 
        Indicator_Name="AAAA" 
        Description="Earnings before interest, taxes, depreciation and amortization" 
        Unit="RUB" 
        Importance="High" 
        Category="Finance"
        Min_Value="0"
        Max_Value="1000000" 
        Created_At="2024-01-01" />
</Indicators>';

DECLARE @doc INT;
EXEC sp_xml_preparedocument @doc OUTPUT, @xml;

INSERT INTO Indicators (
    Indicator_Name, 
    Description, 
    Unit, 
    Importance, 
    Category, 
    Min_Value, 
    Max_Value, 
    Created_At
)
SELECT 
    Indicator_Name,
    Description,
    Unit,
    Importance,
    Category,
    Min_Value,
    Max_Value,
    Created_At
FROM OPENXML(@doc, '/Indicators/Indicator', 1)
WITH (
    Indicator_Name NVARCHAR(100) '@Indicator_Name',
    Description NVARCHAR(255) '@Description',
    Unit NVARCHAR(50) '@Unit',
    Importance NVARCHAR(50) '@Importance',
    Category NVARCHAR(50) '@Category',
    Min_Value DECIMAL(18,2) '@Min_Value',
    Max_Value DECIMAL(18,2) '@Max_Value',
    Created_At DATE '@Created_At'
);

EXEC sp_xml_removedocument @doc;

SELECT * FROM Indicators;


--4
CREATE TABLE Bank_Info_XML (
    Bank_Name NVARCHAR(100) PRIMARY KEY,
    Bank_Info XML
);

DECLARE @xml XML = 
N'<info>
    <CorrespondentAccount>30101810400000000225</CorrespondentAccount>
    <SettlementAccount>40702810400000012345</SettlementAccount>
    <Currency>RUB</Currency>
    <OpenDate>2020-01-15</OpenDate>
    <Country>Russia</Country>
</info>';

INSERT INTO Bank_Info_XML (Bank_Name, Bank_Info)
VALUES ('Sberbank', @xml);

DECLARE @xml2 XML = 
N'<info>
    <CorrespondentAccount>30101810700000000187</CorrespondentAccount>
    <SettlementAccount>40702810700000023456</SettlementAccount>
    <Currency>RUB</Currency>
    <OpenDate>2019-05-20</OpenDate>
    <Country>Russia</Country>
</info>';

INSERT INTO Bank_Info_XML (Bank_Name, Bank_Info)
VALUES ('VTB', @xml2);

UPDATE Bank_Info_XML
SET Bank_Info = 
N'<info>
    <CorrespondentAccount>30101810700000000187</CorrespondentAccount>
    <SettlementAccount>40702810700000099999</SettlementAccount>
    <Currency>RUB</Currency>
    <OpenDate>2019-05-20</OpenDate>
    <Country>Russia</Country>
</info>'
WHERE Bank_Info.value('(/info/SettlementAccount)[1]', 'VARCHAR(30)') = '40702810700000023456';

SELECT 
    Bank_Name,
    Bank_Info.value('(/info/Country)[1]', 'VARCHAR(20)') AS Country,
    CAST(Bank_Info AS NVARCHAR(MAX)) AS RawXml
FROM Bank_Info_XML;


--5 
CREATE XML SCHEMA COLLECTION ContactInfoSchema AS
N'
<xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema"
            targetNamespace="http://example.com/contactinfo"
            xmlns="http://example.com/contactinfo"
            elementFormDefault="qualified">

  <xsd:element name="AdditionalInfo">
    <xsd:complexType>
      <xsd:sequence>
        <xsd:element name="Hobby" type="xsd:string" minOccurs="0" maxOccurs="unbounded"/>
        <xsd:element name="Languages" minOccurs="0">
          <xsd:complexType>
            <xsd:sequence>
              <xsd:element name="Language" type="xsd:string" minOccurs="0" maxOccurs="unbounded"/>
            </xsd:sequence>
          </xsd:complexType>
        </xsd:element>
        <xsd:element name="SocialProfiles" minOccurs="0">
          <xsd:complexType>
            <xsd:sequence>
              <xsd:element name="LinkedInProfile" type="xsd:string" minOccurs="0"/>
              <xsd:element name="TwitterHandle" type="xsd:string" minOccurs="0"/>
            </xsd:sequence>
          </xsd:complexType>
        </xsd:element>
      </xsd:sequence>
      <xsd:attribute name="Updated" type="xsd:date" use="optional"/>
    </xsd:complexType>
  </xsd:element>

</xsd:schema>
';

ALTER TABLE Contacts
ADD AdditionalInfo XML (ContactInfoSchema) NULL;

INSERT INTO Contacts 
    (Last_Name, First_Name, Phone, Email, Position, Hire_Date, AdditionalInfo)
VALUES 
    ('Petrova', 'Anna', '+79161230002', 'petrova@mail.com', 'Manager', '2023-01-15',
    N'<AdditionalInfo xmlns="http://example.com/contactinfo" Updated="2025-05-28">
        <Hobby>Swimming</Hobby>
        <Languages>
          <Language>English</Language>
          <Language>French</Language>
        </Languages>
        <SocialProfiles>
          <LinkedInProfile>https://linkedin.com/in/anna-petrova</LinkedInProfile>
          <TwitterHandle>@annapetrova</TwitterHandle>
        </SocialProfiles>
      </AdditionalInfo>'
    );


UPDATE Contacts
SET AdditionalInfo = 
N'<AdditionalInfo xmlns="http://example.com/contactinfo" Updated="2025-05-28">
    <Hobby>Running</Hobby>
  </AdditionalInfo>'
WHERE Last_Name = 'Ivanov' AND First_Name = 'Ivan';

--
UPDATE Contacts
SET AdditionalInfo = 
N'<AdditionalInfo xmlns="http://example.com/contactinfo" Updated="2025-05-28">
    <FavoriteColor>Blue</FavoriteColor>
  </AdditionalInfo>'
WHERE Last_Name = 'Ivanov' AND First_Name = 'Ivan';



