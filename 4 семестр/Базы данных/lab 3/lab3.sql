/* CREATE DATABASE lab3;
USE lab3;*/

CREATE TABLE ТОВАРЫ (
    id INT PRIMARY KEY AUTO_INCREMENT,
    Наименование NVARCHAR(20) UNIQUE,
    Цена REAL
);

CREATE TABLE ЗАКАЗЧИКИ (
    id INT PRIMARY KEY AUTO_INCREMENT,
    Наименование_фирмы NVARCHAR(50) UNIQUE,
    Адрес NVARCHAR(100),
    Расчетный_счет NVARCHAR(20)
);

CREATE TABLE КОНТАКТЫ_ЗАКАЗЧИКОВ (
    id INT PRIMARY KEY AUTO_INCREMENT,
    заказчик_id INT,
    Контактное_лицо NVARCHAR(50),
    Телефон NVARCHAR(20),
    FOREIGN KEY (заказчик_id) REFERENCES ЗАКАЗЧИКИ(id)
);

CREATE TABLE ПОСТАВКИ (
    id INT PRIMARY KEY AUTO_INCREMENT,
    товар_id INT,
    Дата_поставки DATE,
    Количество INT,
    FOREIGN KEY (товар_id) REFERENCES ТОВАРЫ(id)
);

CREATE TABLE ЗАКАЗЫ (
    id INT PRIMARY KEY AUTO_INCREMENT,
    Номер_заказа NVARCHAR(10) UNIQUE,
    заказчик_id INT,
    Дата_оформления DATE,
    FOREIGN KEY (заказчик_id) REFERENCES ЗАКАЗЧИКИ(id)
);

CREATE TABLE ДЕТАЛИ_ЗАКАЗА (
    id INT PRIMARY KEY AUTO_INCREMENT,
    заказ_id INT,
    товар_id INT,
    Цена_продажи REAL,
    Количество INT,
    FOREIGN KEY (заказ_id) REFERENCES ЗАКАЗЫ(id),
    FOREIGN KEY (товар_id) REFERENCES ТОВАРЫ(id)
);

INSERT INTO ТОВАРЫ (Наименование, Цена) VALUES 
('Ноутбук', 7000),
('Смартфон', 4000),
('Планшет', 3000),
('Монитор', 2500),
('Клавиатура', 500),
('Колонки', 300),
('Компьютерная_мышь', 200);

INSERT INTO ЗАКАЗЧИКИ (Наименование_фирмы, Адрес, Расчетный_счет) VALUES
('21 век', 'Минск, пр.Независимости, 10', 'BY30UNBS30121'),
('Электросила', 'Минск, ул.Притыцкого, 50', 'BY30UNBS30122');

INSERT INTO КОНТАКТЫ_ЗАКАЗЧИКОВ (заказчик_id, Контактное_лицо, Телефон) VALUES
(1, 'Иванов Петр', '+375291234567'),
(2, 'Сидоров Алексей', '+375332345678');

INSERT INTO ЗАКАЗЫ (Номер_заказа, заказчик_id, Дата_оформления) VALUES
('Z001', 1, '2025-03-10'),
('Z002', 2, '2025-03-12');

INSERT INTO ДЕТАЛИ_ЗАКАЗА (заказ_id, товар_id, Цена_продажи, Количество) VALUES
(1, 1, 7400, 2),
(2, 2, 4900, 5);

INSERT INTO ПОСТАВКИ (товар_id, Дата_поставки, Количество) VALUES
(1, '2025-03-15', 2),
(2, '2025-03-20', 5);

SELECT * FROM ЗАКАЗЫ;
SELECT Наименование, Цена FROM ТОВАРЫ;
SELECT COUNT(*) FROM ЗАКАЗЫ;
UPDATE ТОВАРЫ SET Цена = Цена * 1.1 WHERE Наименование = 'Ноутбук';
SELECT * FROM ТОВАРЫ WHERE Наименование = 'Ноутбук';

ALTER TABLE ТОВАРЫ ADD COLUMN Производитель NVARCHAR(50);
ALTER TABLE ТОВАРЫ ADD CONSTRAINT chk_Цена CHECK (Цена > 0);
ALTER TABLE ТОВАРЫ DROP COLUMN Производитель;

/* 7. SQLCMD для справки 
!sqlcmd -?;*/