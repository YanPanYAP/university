ALTER SESSION SET CONTAINER=CRS_PDB;
ALTER SESSION SET CURRENT_SCHEMA = CRS_CUR;

CREATE OR REPLACE VIEW vw_top5StudiosOrders AS
SELECT s.studio_id,
       s.name AS studio_name,
       COUNT(o.order_id) AS total_orders,
       SUM(o.total_price) AS total_revenue
FROM STUDIOS s
JOIN STUDIO_RENT_ORDERS o ON s.studio_id = o.studio_id
GROUP BY s.studio_id, s.name
ORDER BY total_orders DESC
FETCH FIRST 5 ROWS ONLY;

CREATE OR REPLACE VIEW vw_servicesRevenue AS
SELECT sv.service_id,
       sv.name AS service_name,
       COUNT(os.order_id) AS total_orders,
       SUM(sv.price) AS total_revenue
FROM SERVICES sv
JOIN ORDER_SERVICES os ON sv.service_id = os.service_id
GROUP BY sv.service_id, sv.name
ORDER BY total_revenue DESC;

CREATE OR REPLACE VIEW vw_userOrders AS
SELECT u.user_id,
       u.login,
       COUNT(o.order_id) AS total_orders,
       SUM(o.total_price) AS total_spent
FROM USERS u
LEFT JOIN STUDIO_RENT_ORDERS o ON u.user_id = o.user_id
GROUP BY u.user_id, u.login
ORDER BY total_spent DESC;

CREATE OR REPLACE PROCEDURE show_top5_studios IS
BEGIN
  FOR rec IN (SELECT * FROM vw_top5StudiosOrders) LOOP
    DBMS_OUTPUT.PUT_LINE(
      'Studio ID: ' || rec.studio_id ||
      ', Name: ' || rec.studio_name ||
      ', Total Orders: ' || rec.total_orders ||
      ', Total Revenue: ' || rec.total_revenue
    );
  END LOOP;
END;
/

CREATE OR REPLACE PROCEDURE show_services_revenue IS
BEGIN
  FOR rec IN (SELECT * FROM vw_servicesRevenue) LOOP
    DBMS_OUTPUT.PUT_LINE(
      'Service ID: ' || rec.service_id ||
      ', Name: ' || rec.service_name ||
      ', Total Orders: ' || rec.total_orders ||
      ', Total Revenue: ' || rec.total_revenue
    );
  END LOOP;
END;
/

CREATE OR REPLACE PROCEDURE show_user_orders IS
BEGIN
  FOR rec IN (SELECT * FROM vw_userOrders) LOOP
    DBMS_OUTPUT.PUT_LINE(
      'User ID: ' || rec.user_id ||
      ', Login: ' || rec.login ||
      ', Total Orders: ' || rec.total_orders ||
      ', Total Spent: ' || rec.total_spent
    );
  END LOOP;
END;
/

SET SERVEROUTPUT ON;

BEGIN
  show_top5_studios;
  show_services_revenue;
  show_user_orders;
END;
/

--------------------------------------------------------------------------------

CREATE OR REPLACE TRIGGER trig_deleteStudioOrder
AFTER DELETE ON STUDIO_RENT_ORDERS
FOR EACH ROW
BEGIN
  INSERT INTO ARCHIVED_STUDIO_RENT_ORDERS (
    user_id, studio_id, rent_date, start_time, end_time, total_price, archived_at
  )
  VALUES (
    :OLD.user_id, :OLD.studio_id, :OLD.rent_date, :OLD.start_time,
    :OLD.end_time, :OLD.total_price, SYSDATE
  );
END;
/

--------------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION func_avg_studio_revenue(p_studio_id IN NUMBER)
RETURN NUMBER
IS
  v_avg NUMBER;
BEGIN
  SELECT AVG(total_price)
  INTO v_avg
  FROM STUDIO_RENT_ORDERS
  WHERE studio_id = p_studio_id;

  RETURN NVL(v_avg, 0);
END func_avg_studio_revenue;
/

CREATE OR REPLACE FUNCTION func_avg_service_revenue(p_service_id IN NUMBER)
RETURN NUMBER
IS
  v_avg NUMBER;
BEGIN
  SELECT AVG(sv.price)
  INTO v_avg
  FROM ORDER_SERVICES os
  JOIN SERVICES sv ON os.service_id = sv.service_id
  WHERE sv.service_id = p_service_id;

  RETURN NVL(v_avg, 0);
END func_avg_service_revenue;
/

CREATE OR REPLACE FUNCTION func_user_services_count(p_user_id IN NUMBER)
RETURN NUMBER
IS
  v_count NUMBER;
BEGIN
  SELECT COUNT(*)
  INTO v_count
  FROM STUDIO_RENT_ORDERS o
  JOIN ORDER_SERVICES os ON o.order_id = os.order_id
  WHERE o.user_id = p_user_id;

  RETURN NVL(v_count, 0);
END func_user_services_count;
/

CREATE OR REPLACE FUNCTION func_max_daily_revenue(p_studio_id IN NUMBER)
RETURN NUMBER
IS
  v_max NUMBER;
BEGIN
  SELECT MAX(daily_total)
  INTO v_max
  FROM (
    SELECT SUM(total_price) AS daily_total
    FROM STUDIO_RENT_ORDERS
    WHERE studio_id = p_studio_id
    GROUP BY TRUNC(rent_date)
  );

  RETURN NVL(v_max, 0);
END func_max_daily_revenue;
/



DECLARE
  v_avg NUMBER;
BEGIN
  v_avg := func_avg_studio_revenue(1);
  DBMS_OUTPUT.PUT_LINE('Средняя выручка студии 1: ' || v_avg);
END;
/

DECLARE
  v_avg NUMBER;
BEGIN
  v_avg := func_avg_service_revenue(3);
  DBMS_OUTPUT.PUT_LINE('Средняя выручка по услуге 3: ' || v_avg);
END;
/

DECLARE
  v_count NUMBER;
BEGIN
  v_count := func_user_services_count(10);
  DBMS_OUTPUT.PUT_LINE('Пользователь 10 заказал услуг: ' || v_count);
END;
/

DECLARE
  v_max NUMBER;
BEGIN
  v_max := func_max_daily_revenue(5);
  DBMS_OUTPUT.PUT_LINE('Максимальная выручка за день студии 5: ' || v_max);
END;
/

