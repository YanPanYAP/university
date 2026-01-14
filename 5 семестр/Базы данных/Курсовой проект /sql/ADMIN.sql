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
-- USERS CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.ADD_USER(
    p_login        => 'test_user',
    p_email        => 'test@test.com',
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('ADD_USER OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('ADD_USER ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.UPDATE_USER(
    p_user_id      => 3,
    p_login        => 'test_user_updated',
    p_email        => 'updated@test.com',
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('UPDATE_USER OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('UPDATE_USER ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.DELETE_USER(
    p_user_id      => 3,
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_USER OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('DELETE_USER ERROR: ' || SQLERRM);
END;
/

------------------------------------------------------------
-- STUDIOS CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.ADD_STUDIO(
    p_name         => 'Test Studio',
    p_description  => 'Test description',
    p_price        => 100,
    p_square       => 50,
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('ADD_STUDIO OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('ADD_STUDIO ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.UPDATE_STUDIO(
    p_studio_id    => 12,
    p_name         => 'Updated Studio',
    p_description  => 'Updated description',
    p_price        => 150,
    p_square       => 70,
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('UPDATE_STUDIO OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('UPDATE_STUDIO ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.DELETE_STUDIO(
    p_studio_id    => 12,
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_STUDIO OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('DELETE_STUDIO ERROR: ' || SQLERRM);
END;
/

------------------------------------------------------------
-- SERVICES CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.ADD_SERVICE(
    p_name         => 'Photo',
    p_price        => 50,
    p_description  => 'Photo service',
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('ADD_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('ADD_SERVICE ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.UPDATE_SERVICE(
    p_service_id   => 12,
    p_name         => 'Photo PRO',
    p_price        => 80,
    p_description  => 'Updated service',
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('UPDATE_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('UPDATE_SERVICE ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.DELETE_SERVICE(
    p_service_id   => 12,
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('DELETE_SERVICE ERROR: ' || SQLERRM);
END;
/

------------------------------------------------------------
-- STUDIO_RENT_ORDERS CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.ADD_BOOKING(
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
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.UPDATE_BOOKING(
    p_order_id     => 11,
    p_start_time   => SYSTIMESTAMP,
    p_end_time     => SYSTIMESTAMP + INTERVAL '3' HOUR,
    p_total_price  => 700,
    p_status       => 'APPROVED',
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('UPDATE_BOOKING OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('UPDATE_BOOKING ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.DELETE_BOOKING(
    p_order_id     => 11,
    p_current_user => 2
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
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.ADD_ORDER_SERVICE(
    p_order_id     => 12,
    p_service_id   => 13,
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('ADD_ORDER_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('ADD_ORDER_SERVICE ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.DELETE_ORDER_SERVICE(
    p_order_id     => 12,
    p_service_id   => 13,
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_ORDER_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('DELETE_ORDER_SERVICE ERROR: ' || SQLERRM);
END;
/

------------------------------------------------------------
-- ARCHIVING
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.ARCHIVE_BOOKING_BY_ID(
    p_order_id     => 12,
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('ARCHIVE_BOOKING_BY_ID OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('ARCHIVE_BOOKING_BY_ID ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.ARCHIVE_BOOKINGS_BY_STATUS(
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('ARCHIVE_BOOKINGS_BY_STATUS OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('ARCHIVE_BOOKINGS_BY_STATUS ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR_ADMIN_WRAPPER.ARCHIVE_BOOKINGS(
    p_current_user => 2
  );
  DBMS_OUTPUT.PUT_LINE('ARCHIVE_BOOKINGS OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('ARCHIVE_BOOKINGS ERROR: ' || SQLERRM);
END;
/