ALTER SESSION SET CONTAINER=CRS_PDB;
ALTER SESSION SET CURRENT_SCHEMA = CRS_CUR;
----------------------------------------------------------------------
-- Сбор статистики
----------------------------------------------------------------------
BEGIN
    DBMS_STATS.GATHER_TABLE_STATS(
        ownname => 'CRS_CUR',
        tabname => 'USERS',
        estimate_percent => 100,
        cascade => TRUE
    );
    DBMS_STATS.GATHER_TABLE_STATS('CRS_CUR', 'STUDIO_RENT_ORDERS', cascade=>TRUE);
    DBMS_STATS.GATHER_TABLE_STATS('CRS_CUR', 'STUDIOS', cascade=>TRUE);
    DBMS_STATS.GATHER_TABLE_STATS('CRS_CUR', 'ADMINS', cascade=>TRUE);
END;
/

EXPLAIN PLAN FOR
SELECT COUNT(*)
FROM STUDIO_RENT_ORDERS
WHERE studio_id = 1
  AND rent_date = DATE '2025-05-10'
  AND status = 'APPROVED'
  AND (TIMESTAMP '2025-05-10 10:00:00' < end_time 
       AND TIMESTAMP '2025-05-10 12:00:00' > start_time);

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

DROP INDEX SRO_IX_OVERLAP;
CREATE INDEX SRO_IX_OVERLAP
ON STUDIO_RENT_ORDERS (studio_id, rent_date, status, end_time, start_time);




----------------------------------------------------------------------
-- 2. Анализ ОТЧЕТА (JOIN + GROUP BY)
----------------------------------------------------------------------
EXPLAIN PLAN SET STATEMENT_ID = 'REPORT_QUERY' FOR
SELECT s.name, COUNT(o.order_id)
FROM STUDIO_RENT_ORDERS o
JOIN STUDIOS s ON o.studio_id = s.studio_id
WHERE o.rent_date >= SYSDATE - 30
GROUP BY s.name;

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY('PLAN_TABLE', 'REPORT_QUERY'));

----------------------------------------------------------------------
-- 3. Анализ ПРОВЕРКИ ПЕРЕСЕЧЕНИЙ (Самое важное место!)
-- Используем bind переменные (:val), так план будет честнее
----------------------------------------------------------------------
EXPLAIN PLAN SET STATEMENT_ID = 'CHK_OVERLAP' FOR
SELECT COUNT(*)
FROM STUDIO_RENT_ORDERS
WHERE studio_id = :p_studio_id 
  AND rent_date = :p_rent_date 
  AND status = 'APPROVED'
  AND (:p_start_time < end_time AND :p_end_time > start_time);

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY('PLAN_TABLE', 'CHK_OVERLAP'));

----------------------------------------------------------------------
-- 4. Анализ ПРОВЕРКИ РОЛИ АДМИНА (Поиск по FK)
----------------------------------------------------------------------
EXPLAIN PLAN SET STATEMENT_ID = 'CHK_ROLE' FOR
SELECT role_name 
FROM ADMINS 
WHERE user_id = :p_user_id;

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY('PLAN_TABLE', 'CHK_ROLE'));

----------------------------------------------------------------------
-- 5. Анализ CRUD ОПЕРАЦИЙ (Update/Insert)
----------------------------------------------------------------------
EXPLAIN PLAN SET STATEMENT_ID = 'UPD_PRICE' FOR
UPDATE STUDIO_RENT_ORDERS
SET total_price = total_price + 100
WHERE order_id = :p_order_id;

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY('PLAN_TABLE', 'UPD_PRICE'));

----------------------------------------------------------------------
-- 6. Анализ АРХИВАЦИИ (Insert ... Select)
----------------------------------------------------------------------
EXPLAIN PLAN SET STATEMENT_ID = 'ARCHIVE_MOV' FOR
INSERT INTO ARCHIVED_STUDIO_RENT_ORDERS(order_id, user_id, studio_id, rent_date, start_time, end_time, total_price)
SELECT order_id, user_id, studio_id, rent_date, start_time, end_time, total_price
FROM STUDIO_RENT_ORDERS
WHERE order_id = :p_order_id;

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY('PLAN_TABLE', 'ARCHIVE_MOV'));

----------------------------------------------------------------------
-- 7. Анализ МАССОВОГО АРХИВИРОВАНИЯ (ARCHIVE_BOOKINGS)
-- Тестирует индекс SRO_IX_ARCHIVE (END_TIME, STATUS)
----------------------------------------------------------------------
EXPLAIN PLAN SET STATEMENT_ID = 'MASS_ARCHIVE_SEL' FOR
SELECT order_id, user_id, studio_id, rent_date, start_time, end_time, total_price
FROM STUDIO_RENT_ORDERS
WHERE status = 'CANCELLED' OR end_time < SYSTIMESTAMP; -- Фактически < (текущее время)

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY('PLAN_TABLE', 'MASS_ARCHIVE_SEL'));

----------------------------------------------------------------------
-- 8. Анализ СЛОЖНОГО МАССОВОГО УДАЛЕНИЯ (Услуги по заказам)
-- Используем эффективный DELETE EXISTS для ORDER_SERVICES.
----------------------------------------------------------------------
EXPLAIN PLAN SET STATEMENT_ID = 'DELETE_OS_BY_STATUS' FOR
DELETE FROM ORDER_SERVICES os
WHERE EXISTS (
    SELECT 1 
    FROM STUDIO_RENT_ORDERS o 
    WHERE o.order_id = os.order_id 
      AND o.status IN ('APPROVED', 'CANCELLED')
);

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY('PLAN_TABLE', 'DELETE_OS_BY_STATUS'));

----------------------------------------------------------------------
-- 9. Анализ ВЫБОРКИ ЦЕНЫ (ORDER_SERVICES ADD/DELETE)
-- Проверка скорости поиска по PK в таблице SERVICES.
----------------------------------------------------------------------
EXPLAIN PLAN SET STATEMENT_ID = 'SEL_SERVICE_PRICE' FOR
SELECT price 
FROM SERVICES 
WHERE service_id = :p_service_id;

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY('PLAN_TABLE', 'SEL_SERVICE_PRICE'));

----------------------------------------------------------------------
-- 10. Анализ ПОЛУЧЕНИЯ МЕТАДАННЫХ (UPDATE_BOOKING)
-- Проверка скорости получения данных по PK перед проверкой пересечений.
----------------------------------------------------------------------
EXPLAIN PLAN SET STATEMENT_ID = 'SEL_BOOKING_METADATA' FOR
SELECT studio_id, rent_date 
FROM STUDIO_RENT_ORDERS 
WHERE order_id = :p_order_id;

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY('PLAN_TABLE', 'SEL_BOOKING_METADATA'));