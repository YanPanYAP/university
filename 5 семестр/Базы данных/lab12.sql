ALTER SESSION SET CONTAINER = CDB$ROOT;
ALTER SESSION SET CONTAINER = FREEPDB1;
CREATE USER student IDENTIFIED BY student;
ALTER USER student QUOTA UNLIMITED ON users;
ALTER SESSION SET CURRENT_SCHEMA = STUDENT;
SET SERVEROUTPUT ON SIZE UNLIMITED;

//1
create table person(
    person_id number primary key,
    name varchar(100) not null,
    birth_date date,
    address varchar(100),
    email varchar2(100) unique
);


//2
insert into person values(1, 'Иванов Иван Антонович', date '1990-01-15', 'Минск', 'ivan@gmail.com');
insert into person values(2, 'Протас Кристина Ивановна', date '2005-10-06', 'Витебск', 'kris@gmail.com');
insert into person values(3, 'Еленская Ксения Владимировна', date '2006-05-30', 'Миоры', 'ksu@gmail.com');
insert into person values(4, 'Шкиндер Ольга Вадимовна', date '1999-09-05', 'Новополоцк', 'olga@gmail.com');
insert into person values(5, 'Глинский Александр Сергеевич', date '1990-02-14', 'Москва', 'sasha@gmail.com');
insert into person values(6, 'Трофимов Павел Юрьевич', date '1985-03-22', 'Брест', 'pavel@gmail.com');
insert into person values(7, 'Мяделец Елена Владимировна', date '1998-01-20', 'Могилев', 'elena@gmail.com');
insert into person values(8, 'Шевякова Мария Алексеевна', date '1976-12-07', 'Барановичи', 'maria@gmail.com');
insert into person values(9, 'Борисенок Николай Алексеевич', date '1968-04-28', 'Гомель', 'kolya@gmail.com');
insert into person values(10, 'Ганебный Максим Александрович', date '2004-08-16', 'Полоцк', 'maks@gmail.com');

update person set address = 'Минск2' where person_id > 8;

delete from person where person_id > 8;

select * from person;

commit;
rollback;

//3-4
create or replace trigger trig_before
before insert or delete or update on person
begin
    if inserting then
        dbms_output.put_line('trig_before: Выполнена операция INSERT в таблицу PERSON');
    elsif updating then
        dbms_output.put_line('trig_before: Выполнена операция UPDATE в таблицу PERSON');
    elsif deleting then
        dbms_output.put_line('trig_before: Выполнена операция DELETE в таблицу PERSON');
    end if;
end;
/

//5
create or replace trigger trig_before_row
before insert or delete or update on person
for each row
begin
    if inserting then
        dbms_output.put_line('trig_before_row: Выполнена операция INSERT в таблицу PERSON');
    elsif updating then
        dbms_output.put_line('trig_before_row: Выполнена операция UPDATE в таблицу PERSON');
    elsif deleting then
        dbms_output.put_line('trig_before_row: Выполнена операция DELETE в таблицу PERSON');
    end if;
end;
/

//7
create or replace trigger trig_after
after insert or delete or update on person
begin
    if inserting then
        dbms_output.put_line('trig_after: Выполнена операция INSERT в таблицу PERSON');
    elsif updating then
        dbms_output.put_line('trig_after: Выполнена операция UPDATE в таблицу PERSON');
    elsif deleting then
        dbms_output.put_line('trig_after: Выполнена операция DELETE в таблицу PERSON');
    end if;
end;
/

//8
create or replace trigger trig_after_row
after insert or delete or update on person
for each row
begin
    if inserting then
        dbms_output.put_line('trig_after_row: Выполнена операция INSERT в таблицу PERSON');
    elsif updating then
        dbms_output.put_line('trig_after_row: Выполнена операция UPDATE в таблицу PERSON');
    elsif deleting then
        dbms_output.put_line('trig_after_row: Выполнена операция DELETE в таблицу PERSON');
    end if;
end;
/

//9
create table audit_t(
    OperationDate date default sysdate,
    OperationType varchar2(50),
    TriggerName varchar2(50),
    Data varchar2(1000)
);


select * from audit_t;

//10
create or replace trigger trig_before_row
before insert or delete or update on person
for each row
begin
    if inserting then
        insert into audit_t values(sysdate, 'INSERT', 'trig_before_row', 'Старая строка: ' || :old.person_id || 
                                    ' ' || :old.name || ' ' || :old.birth_date || ' ' || :old.address || ' ' || :old.email || 'Новая строка: ' || :new.person_id || 
                                    ' ' || :new.name || ' ' || :new.birth_date || ' ' || :new.address || ' ' || :new.email);
        dbms_output.put_line('trig_before_row: Выполнена операция INSERT в таблицу PERSON');
    elsif updating then
        insert into audit_t values(sysdate, 'UPDATE', 'trig_before_row', 'Старая строка: ' || :old.person_id || 
                                    ' ' || :old.name || ' ' || :old.birth_date || ' ' || :old.address || ' ' || :old.email || 'Новая строка: ' || :new.person_id || 
                                    ' ' || :new.name || ' ' || :new.birth_date || ' ' || :new.address || ' ' || :new.email);
        dbms_output.put_line('trig_before_row: Выполнена операция UPDATE в таблицу PERSON');
    elsif deleting then
        insert into audit_t values(sysdate, 'DELETE', 'trig_before_row', 'Старая строка: ' || :old.person_id || 
                                    ' ' || :old.name || ' ' || :old.birth_date || ' ' || :old.address || ' ' || :old.email || 'Новая строка: ' || :new.person_id || 
                                    ' ' || :new.name || ' ' || :new.birth_date || ' ' || :new.address || ' ' || :new.email);
        dbms_output.put_line('trig_before_row: Выполнена операция DELETE в таблицу PERSON');
    end if;
end;
/

create or replace trigger trig_after_row
after insert or delete or update on person
for each row
begin
    if inserting then
        insert into audit_t values(sysdate, 'INSERT', 'trig_after_row', 'Старая строка: ' || :old.person_id || 
                                    ' ' || :old.name || ' ' || :old.birth_date || ' ' || :old.address || ' ' || :old.email || 'Новая строка: ' || :new.person_id || 
                                    ' ' || :new.name || ' ' || :new.birth_date || ' ' || :new.address || ' ' || :new.email);
        dbms_output.put_line('trig_after_row: Выполнена операция INSERT в таблицу PERSON');
    elsif updating then
        insert into audit_t values(sysdate, 'UPDATE', 'trig_after_row', 'Старая строка: ' || :old.person_id || 
                                    ' ' || :old.name || ' ' || :old.birth_date || ' ' || :old.address || ' ' || :old.email || 'Новая строка: ' || :new.person_id || 
                                    ' ' || :new.name || ' ' || :new.birth_date || ' ' || :new.address || ' ' || :new.email);
        dbms_output.put_line('trig_after_row: Выполнена операция UPDATE в таблицу PERSON');
    elsif deleting then
        insert into audit_t values(sysdate, 'DELETE', 'trig_after_row', 'Старая строка: ' || :old.person_id || 
                                    ' ' || :old.name || ' ' || :old.birth_date || ' ' || :old.address || ' ' || :old.email || 'Новая строка: ' || :new.person_id || 
                                    ' ' || :new.name || ' ' || :new.birth_date || ' ' || :new.address || ' ' || :new.email);
        dbms_output.put_line('trig_after_row: Выполнена операция DELETE в таблицу PERSON');
    end if;
end;
/


//11
insert into person values(11, 'Иванов Иван Иванович', date '1990-01-15', 'Минск', 'ivan@gmail.com');

//12


create or replace trigger trig_drop_table
before drop on schema
begin
    if ora_dict_obj_type = 'TABLE'
       and ora_dict_obj_name = 'PERSON'
    then
        raise_application_error(-20000, 'Do not drop table PERSON.');
    end if;
end;
/
drop table person;

//13
drop table audit_t;

select * from user_triggers;
select trigger_name, trigger_type, status from user_triggers;

//14
create view pers_view
as select person_id, name, birth_date, address, email from person;

select * from pers_view;
select * from person;

CREATE OR REPLACE TRIGGER trig_view
INSTEAD OF INSERT ON pers_view
FOR EACH ROW
BEGIN
    INSERT INTO person (person_id, name, birth_date, address, email)
    VALUES (:NEW.person_id, :NEW.name, :NEW.birth_date, :NEW.address, :NEW.email);
END;
/

insert into pers_view values(12, 'Сеньшов Даниил Сергеевич', date '1997-02-14', 'Минск', 'daya@gmail.com');

