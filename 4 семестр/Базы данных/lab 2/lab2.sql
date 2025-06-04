CREATE TABLE Enterprises (
    ID_предприятия INT AUTO_INCREMENT PRIMARY KEY,
    Название_предприятия VARCHAR(255) NOT NULL,
    ID_банка INT,
    ID_контакта INT,
    FOREIGN KEY (ID_банка) REFERENCES Bank_Details(ID_банка),
    FOREIGN KEY (ID_контакта) REFERENCES Contacts(ID_контакта)
);

CREATE TABLE Bank_Details (
    ID_банка INT AUTO_INCREMENT PRIMARY KEY,
    Банковские_реквизиты VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE Contacts (
    ID_контакта INT AUTO_INCREMENT PRIMARY KEY,
    ФИО VARCHAR(255) NOT NULL,
    Телефон VARCHAR(20) NOT NULL UNIQUE
);

CREATE TABLE Indicators (
    ID_показателя INT AUTO_INCREMENT PRIMARY KEY,
    Название_показателя VARCHAR(255) NOT NULL UNIQUE,
    Важность ENUM('Высокая', 'Средняя', 'Низкая') NOT NULL
);

CREATE TABLE Indicator_Values (
    ID_значения INT AUTO_INCREMENT PRIMARY KEY,
    ID_показателя INT,
    ID_предприятия INT,
    Дата DATE NOT NULL,
    Значение DECIMAL(15,2) NOT NULL,
    FOREIGN KEY (ID_показателя) REFERENCES Indicators(ID_показателя),
    FOREIGN KEY (ID_предприятия) REFERENCES Enterprises(ID_предприятия)
);

INSERT INTO Bank_Details (Банковские_реквизиты) VALUES 
('1234567890'),
('9876543210');

INSERT INTO Contacts (ФИО, Телефон) VALUES 
('Иванов И.И.', '1234567'),
('Петров П.П.', '7654321');

INSERT INTO Enterprises (Название_предприятия, ID_банка, ID_контакта) VALUES 
('ООО "Альфа"', 1, 1),
('ЗАО "Бета"', 2, 2);

INSERT INTO Indicators (Название_показателя, Важность) VALUES 
('Прибыль', 'Высокая'),
('Себестоимость', 'Средняя');

INSERT INTO Indicator_Values (ID_показателя, ID_предприятия, Дата, Значение) VALUES 
(1, 1, '2024-01-01', 100000.00),
(2, 1, '2024-01-01', 50000.00),
(1, 2, '2024-01-01', 200000.00);
