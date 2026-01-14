ALTER SESSION SET CONTAINER=CRS_PDB;
ALTER SESSION SET CURRENT_SCHEMA = CRS_CUR;

INSERT INTO ADMINS (USER_ID, LOGIN, EMAIL, CREATED_AT) 
VALUES (1, 'alex_ivanov', 'alex.ivanov@example.com', SYSDATE);

INSERT INTO USERS (USER_ID, LOGIN, EMAIL, CREATED_AT) 
VALUES (2, 'maria_petrova', 'maria.petrova@example.com', SYSDATE);

INSERT INTO USERS (USER_ID, LOGIN, EMAIL, CREATED_AT) 
VALUES (3, 'sergey_smirnov', 'sergey.smirnov@example.com', SYSDATE);
COMMIT;

INSERT INTO ADMINS (user_id, role_name)
VALUES (1, 'GLAV_ADMIN');
INSERT INTO ADMINS (user_id, role_name)
VALUES (2, 'ADMIN');
COMMIT;

------------------------------------------------------------
-- ADMINS CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR.ADD_ADMIN(
    p_user_id      => 3,
    p_role_name    => 'ADMIN',
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('ADD_ADMIN OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.UPDATE_ADMIN(
    p_admin_id     => 2,
    p_role_name    => 'GLAV_ADMIN',
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('UPDATE_ADMIN OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.CRS_CUR.DELETE_ADMIN(
    p_admin_id     => 12,
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_ADMIN OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

------------------------------------------------------------
-- USERS CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR.CRS_CUR.ADD_USER(
    p_login        => 'test_user',
    p_email        => 'test@test.com',
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('ADD_USER OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.UPDATE_USER(
    p_user_id      => 19,
    p_login        => 'test_user_updated',
    p_email        => 'updated@test.com',
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('UPDATE_USER OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.DELETE_USER(
    p_user_id      => 19,
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_USER OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

------------------------------------------------------------
-- STUDIOS CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR.ADD_STUDIO(
    p_name         => 'Test Studio',
    p_description  => 'Test description',
    p_price        => 100,
    p_square       => 50,
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('ADD_STUDIO OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.UPDATE_STUDIO(
      p_studio_id    => 12,
    p_name         => 'Updated Studio',
    p_description  => 'Updated description',
    p_price        => 150,
    p_square       => 70,
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('UPDATE_STUDIO OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.DELETE_STUDIO(
    p_studio_id    => 12,
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_STUDIO OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

------------------------------------------------------------
-- SERVICES CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR.ADD_SERVICE(
    p_name         => 'Photo',
    p_price        => 50,
    p_description  => 'Photo service',
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('ADD_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.UPDATE_SERVICE(
    p_service_id   => 12,
    p_name         => 'Photo PRO',
    p_price        => 80,
    p_description  => 'Updated service',
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('UPDATE_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.DELETE_SERVICE(
    p_service_id   => 12,
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

------------------------------------------------------------
-- STUDIO_RENT_ORDERS CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR.ADD_BOOKING(
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
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.UPDATE_BOOKING(
    p_order_id     => 11,
    p_start_time   => SYSTIMESTAMP,
    p_end_time     => SYSTIMESTAMP + INTERVAL '3' HOUR,
    p_total_price  => 700,
    p_status       => 'APPROVED',
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('UPDATE_BOOKING OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.DELETE_BOOKING(
    p_order_id     => 11,
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_BOOKING OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

------------------------------------------------------------
-- ORDER_SERVICES CRUD
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR.ADD_ORDER_SERVICE(
    p_order_id     => 12,
    p_service_id   => 13,
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('ADD_ORDER_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.DELETE_ORDER_SERVICE(
    p_order_id     => 12,
    p_service_id   => 13,
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('DELETE_ORDER_SERVICE OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

------------------------------------------------------------
-- ARCHIVING
------------------------------------------------------------
BEGIN
  CRS_CUR.CRS_CUR.ARCHIVE_BOOKING_BY_ID(
    p_order_id     => 12,
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('ARCHIVE_BOOKING_BY_ID OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.ARCHIVE_BOOKINGS_BY_STATUS(
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('ARCHIVE_BOOKINGS_BY_STATUS OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_CUR.ARCHIVE_BOOKINGS(
    p_current_user => 1
  );
  DBMS_OUTPUT.PUT_LINE('ARCHIVE_BOOKINGS OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE(SQLERRM);
END;
/

------------------------------------------------------------
-- ANALYTICS
------------------------------------------------------------
SET SERVEROUTPUT ON SIZE UNLIMITED;

BEGIN
  CRS_CUR.CRS_ANALYTICS.GET_BOOKINGS_BY_PERIOD(
    p_current_user => 1,
    p_start_date   => SYSDATE - 30,
    p_end_date     => SYSDATE
  );
  DBMS_OUTPUT.PUT_LINE('GET_BOOKINGS_BY_PERIOD OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('GET_BOOKINGS_BY_PERIOD ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_ANALYTICS.GET_POPULAR_STUDIOS(
    p_current_user => 1,
    p_limit        => 5
  );
  DBMS_OUTPUT.PUT_LINE('GET_POPULAR_STUDIOS OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('GET_POPULAR_STUDIOS ERROR: ' || SQLERRM);
END;
/

BEGIN
  CRS_CUR.CRS_ANALYTICS.GET_POPULAR_SERVICES(
    p_current_user => 1,
    p_limit        => 5
  );
  DBMS_OUTPUT.PUT_LINE('GET_POPULAR_SERVICES OK');
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('GET_POPULAR_SERVICES ERROR: ' || SQLERRM);
END;
/

DECLARE
  v_total NUMBER;
BEGIN
  CRS_CUR.CRS_ANALYTICS.GET_TOTAL_BOOKINGS(
    p_current_user => 1,
    p_count        => v_total
  );
  DBMS_OUTPUT.PUT_LINE('GET_TOTAL_BOOKINGS OK, total = ' || v_total);
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('GET_TOTAL_BOOKINGS ERROR: ' || SQLERRM);
END;
/
