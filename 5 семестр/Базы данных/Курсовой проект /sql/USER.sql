ALTER SESSION SET CONTAINER=CRS_PDB;
ALTER SESSION SET CURRENT_SCHEMA = CRS_CUR;

SET SERVEROUTPUT ON SIZE UNLIMITED;

DECLARE
  my_total NUMBER;
BEGIN
  EXECUTE IMMEDIATE 'BEGIN CRS_CUR.CRS_ANALYTICS.GET_TOTAL_BOOKINGS(:1); END;'
    USING OUT my_total;
  DBMS_OUTPUT.PUT_LINE('Всего бронирований: ' || my_total);
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('Нет доступа к CRS_ANALYTICS: ' || SQLERRM);
END;
/

------------------------------------------------------------
-- STUDIO_RENT_ORDERS CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR_USER_WRAPPER.ADD_BOOKING(
    p_user_id      => 3,
    p_studio_id    => 13,
    p_rent_date    => SYSDATE,
    p_start_time   => SYSTIMESTAMP,
    p_end_time     => SYSTIMESTAMP + INTERVAL '2' HOUR,
    p_total_price  => 500
  );
  DBMS_OUTPUT.PUT_LINE('ADD_BOOKING OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('ADD_BOOKING ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_USER_WRAPPER.UPDATE_BOOKING(
    p_order_id     => 11,
    p_start_time   => SYSTIMESTAMP,
    p_end_time     => SYSTIMESTAMP + INTERVAL '3' HOUR,
    p_total_price  => 700,
    p_status       => 'APPROVED',
    p_current_user => 3
  );
  DBMS_OUTPUT.PUT_LINE('UPDATE_BOOKING OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('UPDATE_BOOKING ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_USER_WRAPPER.DELETE_BOOKING(
    p_order_id     => 11,
    p_current_user => 3
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_BOOKING OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('DELETE_BOOKING ERROR: ' || SQLERRM);
END;
/

------------------------------------------------------------
-- ORDER_SERVICES CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR_USER_WRAPPER.ADD_ORDER_SERVICE(
    p_order_id     => 12,
    p_service_id   => 13,
    p_current_user => 3
  );
  DBMS_OUTPUT.PUT_LINE('ADD_ORDER_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('ADD_ORDER_SERVICE ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_USER_WRAPPER.DELETE_ORDER_SERVICE(
    p_order_id     => 12,
    p_service_id   => 13,
    p_current_user => 3
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_ORDER_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('DELETE_ORDER_SERVICE ERROR: ' || SQLERRM);
END;
/
