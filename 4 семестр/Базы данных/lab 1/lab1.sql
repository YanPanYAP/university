CREATE TABLE ТОВАРЫ (
Наименование NVARCHAR(20) PRIMARY KEY,
Цена REAL,
Количество INT
);

CREATE TABLE ЗАКАЗЧИКИ (
	Наименование_фирмы NVARCHAR(20) PRIMARY KEY,
	Адрес NVARCHAR(50),
	Расчетный_счет NVARCHAR(15)
);

CREATE TABLE ЗАКАЗЫ (
Номер_заказа NVARCHAR(10) PRIMARY KEY,
Наименование_товара NVARCHAR(20),
Цена_продажи REAL,
Количество INT,
Дата_поставки DATE,
Заказчик NVARCHAR(20)
);

ALTER TABLE ЗАКАЗЫ
ADD FOREIGN KEY (Наименование_товара) REFERENCES ТОВАРЫ(Наименование);

ALTER TABLE ЗАКАЗЫ
ADD FOREIGN KEY (Заказчик) REFERENCES ЗАКАЗЧИКИ(Наименование_фирмы);

INSERT INTO ТОВАРЫ (Наименование, Цена, Количество) VALUES 
('Ноутбук', 7000, 10),
('Смартфон', 4000, 15),
('Планшет', 3000, 8),
('Монитор', 2500, 5),
('Клавиатура', 500, 20),
('Колонки', 300, 5),
('Компьютерная_мышь', 200, 10);

INSERT INTO ЗАКАЗЧИКИ (Наименование_фирмы, Адрес, Расчетный_счет) VALUES
('21 век', 'Минск, пр.Независимости, 10', 'BY30UNBS30121'),
('Электросила', 'Минск, ул.Притыцкого, 50', 'BY30UNBS30122'),
('5 элемент', 'Минск, ул.Немига, 15', 'BY30UNBS30123'),
('Xiaomi-store', 'Минск, ул.Сурганова, 7', 'BY30UNBS30124'),
('Wildberries', 'Минск, ул.Кальварийская, 20', 'BY30UNBS30125'),
('Золотое яблоко', 'Минск, пр.Дзержинского, 8', 'BY30UNBS30126'),
('Vans', 'Минск, ул.Богдановича, 5', 'BY30UNBS30127');

INSERT INTO ЗАКАЗЫ (Номер_заказа, Наименование_товара, Цена_продажи, Количество, Дата_поставки, Заказчик) VALUES
('Z001', 'Ноутбук', 7400, 2, '2025-03-15', '21 век'),
('Z002', 'Смартфон', 4900, 5, '2025-03-20', 'Электросила'),
('Z003', 'Планшет', 2950, 3, '2025-03-25', '5 элемент'),
('Z004', 'Монитор', 2400, 1, '2025-04-01', 'Xiaomi-store'),
('Z005', 'Клавиатура', 440, 4, '2025-04-10', 'Wildberries'),
('Z006', 'Компьютерная_мышь', 240, 10, '2025-04-15', 'Золотое яблоко'),
('Z007', 'Колонки', 690, 2, '2025-04-20', 'Vans');

SELECT * FROM ЗАКАЗЫ WHERE Дата_поставки > '2025-03-25';
SELECT * FROM ТОВАРЫ WHERE Цена BETWEEN 2000 AND 4000;

SELECT DISTINCT ЗАКАЗЧИКИ.Наименование_фирмы 
FROM ЗАКАЗЫ 
JOIN ЗАКАЗЧИКИ ON ЗАКАЗЫ.Заказчик = ЗАКАЗЧИКИ.Наименование_фирмы
WHERE ЗАКАЗЫ.Наименование_товара = 'Ноутбук';

SELECT * FROM ЗАКАЗЫ 
WHERE Заказчик = '21 век' OR Заказчик = '5 элемент'
ORDER BY Дата_поставки;