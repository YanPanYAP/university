CREATE DATABASE IF NOT EXISTS YAP CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE YAP;

CREATE TABLE AUDITORIUM_TYPE (
    AUDITORIUM_TYPE CHAR(10) PRIMARY KEY,
    AUDITORIUM_TYPENAME VARCHAR(30)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO AUDITORIUM_TYPE VALUES 
('ЛК', 'Лекционная'),
('ЛК-П', 'Лекционная с проектором'),
('ЛБ', 'Лабораторная'),
('ЛБ-X', 'Лабораторная специальная'),
('ПР', 'Практическая');

CREATE TABLE AUDITORIUM (
    AUDITORIUM CHAR(20) PRIMARY KEY,
    AUDITORIUM_TYPE CHAR(10),
    AUDITORIUM_CAPACITY INT DEFAULT 1 CHECK (AUDITORIUM_CAPACITY BETWEEN 1 AND 300),
    AUDITORIUM_NAME VARCHAR(50),
    FOREIGN KEY (AUDITORIUM_TYPE) REFERENCES AUDITORIUM_TYPE(AUDITORIUM_TYPE)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO AUDITORIUM VALUES
('206-1','ЛК-П',15,'206-1'),
('301-1','ЛК-П',15,'301-1'),
('236-1','ЛК',60,'236-1'),
('313-1','ЛБ',60,'313-1'),
('324-1','ЛБ',50,'324-1'),
('413-1','ЛК-П',15,'413-1'),
('423-1','ЛК',90,'423-1'),
('408-2','ЛК',90,'408-2');

CREATE TABLE FACULTY (
    FACULTY CHAR(10) PRIMARY KEY,
    FACULTY_NAME VARCHAR(50) DEFAULT '???'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO FACULTY VALUES
('ФИТ','Факультет информационных технологий'),
('ФРЭ','Факультет радиотехники и электроники'),
('ФКП','Факультет компьютерного проектирования'),
('ФММ','Факультет машиноведения и механики'),
('ФЭС','Факультет экономики и управления'),
('ФГД','Факультет гуманитарных дисциплин');

CREATE TABLE PULPIT (
    PULPIT CHAR(20) PRIMARY KEY,
    PULPIT_NAME VARCHAR(100),
    FACULTY CHAR(10),
    FOREIGN KEY (FACULTY) REFERENCES FACULTY(FACULTY)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO PULPIT VALUES
('ИСиТ','Информационные системы и технологии','ФИТ'),
('ПОИТ','Программное обеспечение ИТ','ФИТ'),
('ВМ','Вычислительные машины','ФИТ'),
('ЭЭ','Электроэнергетика','ФРЭ'),
('МТ','Машиностроение и технологии','ФММ');

CREATE TABLE TEACHER (
    TEACHER CHAR(10) PRIMARY KEY,
    TEACHER_NAME VARCHAR(100),
    PULPIT CHAR(20),
    FOREIGN KEY (PULPIT) REFERENCES PULPIT(PULPIT)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO TEACHER VALUES
('ИВН','Иванов Иван Николаевич','ИСиТ'),
('ПТР','Петров Пётр Сергеевич','ПОИТ'),
('СИД','Сидоров Сергей Алексеевич','ВМ');

CREATE TABLE SUBJECT (
    SUBJECT CHAR(10) PRIMARY KEY,
    SUBJECT_NAME VARCHAR(100) UNIQUE,
    PULPIT CHAR(20),
    FOREIGN KEY (PULPIT) REFERENCES PULPIT(PULPIT)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO SUBJECT VALUES
('БД','Базы данных','ПОИТ'),
('ОС','Операционные системы','ПОИТ'),
('ПСКП','Программирование сетевых клиент-серверных приложений','ПОИТ'),
('ТП','Теория программирования','ИСиТ');

CREATE USER IF NOT EXISTS 'student'@'localhost' IDENTIFIED BY 'fitfit';
GRANT ALL PRIVILEGES ON YAP.* TO 'student'@'localhost';
FLUSH PRIVILEGES;
