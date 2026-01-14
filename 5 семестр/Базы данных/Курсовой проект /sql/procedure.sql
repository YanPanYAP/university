ALTER SESSION SET CONTAINER=CRS_PDB;
ALTER SESSION SET CURRENT_SCHEMA = CRS_CUR;

-- Пакет CRS_CUR
CREATE OR REPLACE PACKAGE CRS_CUR AS
  --------------------------------------
  -- Константы типов ошибок
  --------------------------------------
  c_err_input   CONSTANT VARCHAR2(30) := 'INPUT_ERROR';
  c_err_business CONSTANT VARCHAR2(30) := 'BUSINESS_ERROR';
  c_err_system   CONSTANT VARCHAR2(30) := 'SYSTEM_ERROR';

  --------------------------------------
  -- Общий обработчик ошибок
  --------------------------------------
  PROCEDURE HANDLE_ERROR(p_context IN VARCHAR2, p_error_type IN VARCHAR2);

  --------------------------------------
  -- Утилиты валидации
  --------------------------------------
  PROCEDURE VALIDATE_NOT_NULL(p_val IN VARCHAR2, p_param_name IN VARCHAR2);
  PROCEDURE VALIDATE_NOT_NULL_NUM(p_val IN NUMBER, p_param_name IN VARCHAR2);
  PROCEDURE VALIDATE_STRING_LENGTH(p_val IN VARCHAR2, p_param_name IN VARCHAR2, p_max_len IN PLS_INTEGER);
  PROCEDURE VALIDATE_POSITIVE_NUMBER(p_val IN NUMBER, p_param_name IN VARCHAR2);

  PROCEDURE CHECK_USER_EXISTS(p_user_id IN USERS.USER_ID%TYPE, p_param_name IN VARCHAR2);
  PROCEDURE CHECK_STUDIO_EXISTS(p_studio_id IN STUDIOS.STUDIO_ID%TYPE, p_param_name IN VARCHAR2);
  PROCEDURE CHECK_SERVICE_EXISTS(p_service_id IN SERVICES.SERVICE_ID%TYPE, p_param_name IN VARCHAR2);
  PROCEDURE CHECK_ORDER_EXISTS(p_order_id IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE, p_param_name IN VARCHAR2);
  PROCEDURE CHECK_ADMIN_EXISTS(p_admin_id IN ADMINS.ADMIN_ID%TYPE, p_param_name IN VARCHAR2);

    --------------------------------------
  -- ADMINS CRUD
  --------------------------------------
  PROCEDURE ADD_ADMIN(p_user_id IN USERS.USER_ID%TYPE DEFAULT NULL, p_role_name IN ADMINS.ROLE_NAME%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);
  PROCEDURE UPDATE_ADMIN(p_admin_id IN ADMINS.ADMIN_ID%TYPE DEFAULT NULL, p_role_name IN ADMINS.ROLE_NAME%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);
  PROCEDURE DELETE_ADMIN(p_admin_id IN ADMINS.ADMIN_ID%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);

  --------------------------------------
  -- USERS CRUD
  --------------------------------------
  PROCEDURE ADD_USER(p_login IN USERS.LOGIN%TYPE DEFAULT NULL, p_email IN USERS.EMAIL%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);
  PROCEDURE UPDATE_USER(p_user_id IN USERS.USER_ID%TYPE DEFAULT NULL, p_login IN USERS.LOGIN%TYPE DEFAULT NULL, p_email IN USERS.EMAIL%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);
  PROCEDURE DELETE_USER(p_user_id IN USERS.USER_ID%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);

  --------------------------------------
  -- STUDIOS CRUD
  --------------------------------------
  PROCEDURE ADD_STUDIO(p_name IN STUDIOS.NAME%TYPE DEFAULT NULL, p_description IN STUDIOS.DESCRIPTION%TYPE DEFAULT NULL,
                       p_price IN STUDIOS.PRICE_PER_HOUR%TYPE DEFAULT NULL, p_square IN STUDIOS.SQUARE%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);
  PROCEDURE UPDATE_STUDIO(p_studio_id IN STUDIOS.STUDIO_ID%TYPE DEFAULT NULL, p_name IN STUDIOS.NAME%TYPE DEFAULT NULL,
                          p_description IN STUDIOS.DESCRIPTION%TYPE DEFAULT NULL, p_price IN STUDIOS.PRICE_PER_HOUR%TYPE DEFAULT NULL,
                          p_square IN STUDIOS.SQUARE%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);
  PROCEDURE DELETE_STUDIO(p_studio_id IN STUDIOS.STUDIO_ID%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);

  --------------------------------------
  -- SERVICES CRUD
  --------------------------------------
  PROCEDURE ADD_SERVICE(p_name IN SERVICES.NAME%TYPE DEFAULT NULL, p_price IN SERVICES.PRICE%TYPE DEFAULT NULL, p_description IN SERVICES.DESCRIPTION%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);
  PROCEDURE UPDATE_SERVICE(p_service_id IN SERVICES.SERVICE_ID%TYPE DEFAULT NULL, p_name IN SERVICES.NAME%TYPE DEFAULT NULL,
                           p_price IN SERVICES.PRICE%TYPE DEFAULT NULL, p_description IN SERVICES.DESCRIPTION%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);
  PROCEDURE DELETE_SERVICE(p_service_id IN SERVICES.SERVICE_ID%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);

  --------------------------------------
  -- STUDIO_RENT_ORDERS CRUD
  --------------------------------------
  PROCEDURE ADD_BOOKING(
    p_user_id     IN STUDIO_RENT_ORDERS.USER_ID%TYPE DEFAULT NULL,
    p_studio_id   IN STUDIO_RENT_ORDERS.STUDIO_ID%TYPE DEFAULT NULL,
    p_rent_date   IN STUDIO_RENT_ORDERS.RENT_DATE%TYPE DEFAULT NULL,
    p_start_time  IN STUDIO_RENT_ORDERS.START_TIME%TYPE DEFAULT NULL,
    p_end_time    IN STUDIO_RENT_ORDERS.END_TIME%TYPE DEFAULT NULL,
    p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE DEFAULT NULL
  );

  PROCEDURE UPDATE_BOOKING(
    p_order_id    IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE DEFAULT NULL,
    p_start_time  IN STUDIO_RENT_ORDERS.START_TIME%TYPE DEFAULT NULL,
    p_end_time    IN STUDIO_RENT_ORDERS.END_TIME%TYPE DEFAULT NULL,
    p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE DEFAULT NULL,
    p_status      IN STUDIO_RENT_ORDERS.STATUS%TYPE DEFAULT NULL,
    p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL
  );

  PROCEDURE DELETE_BOOKING(p_order_id IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE DEFAULT NULL, p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);

  --------------------------------------
  -- ORDER_SERVICES CRUD
  --------------------------------------
  PROCEDURE ADD_ORDER_SERVICE(
    p_order_id    IN ORDER_SERVICES.ORDER_ID%TYPE DEFAULT NULL,
    p_service_id  IN ORDER_SERVICES.SERVICE_ID%TYPE DEFAULT NULL,
    p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL
  );
  PROCEDURE UPDATE_ORDER_SERVICE(
    p_order_id    IN ORDER_SERVICES.ORDER_ID%TYPE DEFAULT NULL,
    p_service_id  IN ORDER_SERVICES.SERVICE_ID%TYPE DEFAULT NULL,
    p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL
  );
  PROCEDURE DELETE_ORDER_SERVICE(
    p_order_id    IN ORDER_SERVICES.ORDER_ID%TYPE DEFAULT NULL,
    p_service_id  IN ORDER_SERVICES.SERVICE_ID%TYPE DEFAULT NULL,
    p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL
  );

  --------------------------------------
  -- Архивирование бронирований
  --------------------------------------
  PROCEDURE ARCHIVE_BOOKINGS(p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);
  PROCEDURE ARCHIVE_BOOKING_BY_ID(
    p_order_id     IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE DEFAULT NULL,
    p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL
  );
  PROCEDURE ARCHIVE_BOOKINGS_BY_STATUS(p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL);

END CRS_CUR;
/

-- Пакет CRS_CUR
CREATE OR REPLACE PACKAGE BODY CRS_CUR AS

  --------------------------------------
  -- Общий обработчик ошибок
  --------------------------------------
  PROCEDURE HANDLE_ERROR(p_context IN VARCHAR2, p_error_type IN VARCHAR2) IS
    v_msg VARCHAR2(4000);
  BEGIN
    ROLLBACK;
    v_msg := 'Context=[' || p_context || '] Type=[' || p_error_type || '] Orig=[' || SQLERRM || ']';
    RAISE_APPLICATION_ERROR(-20099, v_msg);
  END HANDLE_ERROR;

  --------------------------------------
  -- Утилиты валидации
  --------------------------------------
  PROCEDURE VALIDATE_NOT_NULL(p_val IN VARCHAR2, p_param_name IN VARCHAR2) IS
  BEGIN
    IF p_val IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр ' || p_param_name || ' не может быть NULL');
    END IF;
  END VALIDATE_NOT_NULL;

  PROCEDURE VALIDATE_NOT_NULL_NUM(p_val IN NUMBER, p_param_name IN VARCHAR2) IS
  BEGIN
    IF p_val IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр ' || p_param_name || ' не может быть NULL');
    END IF;
  END VALIDATE_NOT_NULL_NUM;

  PROCEDURE VALIDATE_STRING_LENGTH(p_val IN VARCHAR2, p_param_name IN VARCHAR2, p_max_len IN PLS_INTEGER) IS
  BEGIN
    IF p_val IS NOT NULL AND LENGTH(p_val) > p_max_len THEN
      RAISE_APPLICATION_ERROR(-20092, 'Параметр ' || p_param_name || ' слишком длинный (макс ' || p_max_len || ')');
    END IF;
  END VALIDATE_STRING_LENGTH;

  PROCEDURE VALIDATE_POSITIVE_NUMBER(p_val IN NUMBER, p_param_name IN VARCHAR2) IS
  BEGIN
    IF p_val IS NOT NULL AND p_val <= 0 THEN
      RAISE_APPLICATION_ERROR(-20091, 'Параметр ' || p_param_name || ' должен быть положительным');
    END IF;
  END VALIDATE_POSITIVE_NUMBER;

  --------------------------------------
  -- Проверки существования записей
  --------------------------------------
  PROCEDURE CHECK_USER_EXISTS(p_user_id IN USERS.USER_ID%TYPE, p_param_name IN VARCHAR2) IS
    v_dummy NUMBER;
  BEGIN
    IF p_user_id IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, p_param_name || ' не может быть NULL');
    END IF;
    SELECT 1 INTO v_dummy FROM USERS WHERE user_id = p_user_id;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Пользователь (' || p_param_name || ') с id=' || p_user_id || ' не найден');
  END CHECK_USER_EXISTS;

  PROCEDURE CHECK_STUDIO_EXISTS(p_studio_id IN STUDIOS.STUDIO_ID%TYPE, p_param_name IN VARCHAR2) IS
    v_dummy NUMBER;
  BEGIN
    IF p_studio_id IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, p_param_name || ' не может быть NULL');
    END IF;
    SELECT 1 INTO v_dummy FROM STUDIOS WHERE studio_id = p_studio_id;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Студия (' || p_param_name || ') с id=' || p_studio_id || ' не найдена');
  END CHECK_STUDIO_EXISTS;

  PROCEDURE CHECK_SERVICE_EXISTS(p_service_id IN SERVICES.SERVICE_ID%TYPE, p_param_name IN VARCHAR2) IS
    v_dummy NUMBER;
  BEGIN
    IF p_service_id IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, p_param_name || ' не может быть NULL');
    END IF;
    SELECT 1 INTO v_dummy FROM SERVICES WHERE service_id = p_service_id;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Услуга (' || p_param_name || ') с id=' || p_service_id || ' не найдена');
  END CHECK_SERVICE_EXISTS;

  PROCEDURE CHECK_ORDER_EXISTS(p_order_id IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE, p_param_name IN VARCHAR2) IS
    v_dummy NUMBER;
  BEGIN
    IF p_order_id IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, p_param_name || ' не может быть NULL');
    END IF;
    SELECT 1 INTO v_dummy FROM STUDIO_RENT_ORDERS WHERE order_id = p_order_id;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Заказ (' || p_param_name || ') с id=' || p_order_id || ' не найден');
  END CHECK_ORDER_EXISTS;

  PROCEDURE CHECK_ADMIN_EXISTS(p_admin_id IN ADMINS.ADMIN_ID%TYPE, p_param_name IN VARCHAR2) IS
    v_dummy NUMBER;
  BEGIN
    IF p_admin_id IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, p_param_name || ' не может быть NULL');
    END IF;
    SELECT 1 INTO v_dummy FROM ADMINS WHERE admin_id = p_admin_id;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Admin (' || p_param_name || ') с id=' || p_admin_id || ' не найден');
  END CHECK_ADMIN_EXISTS;

  --------------------------------------
  -- Вспомогательные процедуры
  --------------------------------------
  PROCEDURE CHECK_ADMIN_ROLE(p_user_id IN USERS.USER_ID%TYPE, p_required_role IN VARCHAR2) IS
    v_role ADMINS.ROLE_NAME%TYPE;
  BEGIN
    SELECT role_name INTO v_role FROM ADMINS WHERE user_id = p_user_id;
    IF v_role != p_required_role AND v_role != 'GLAV_ADMIN' THEN
      RAISE_APPLICATION_ERROR(-20010, 'Недостаточно прав для выполнения операции!');
    END IF;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20011, 'Пользователь не является администратором!');
  END CHECK_ADMIN_ROLE;

  PROCEDURE CHECK_BOOKING_PERMISSION(p_user_id IN USERS.USER_ID%TYPE, p_order_id IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE) IS
    v_role ADMINS.ROLE_NAME%TYPE;
    v_owner STUDIO_RENT_ORDERS.USER_ID%TYPE;
  BEGIN
    BEGIN
      SELECT role_name INTO v_role FROM ADMINS WHERE user_id = p_user_id;
    EXCEPTION
      WHEN NO_DATA_FOUND THEN v_role := NULL;
    END;

    IF v_role IS NOT NULL THEN RETURN; END IF;

    SELECT user_id INTO v_owner FROM STUDIO_RENT_ORDERS WHERE order_id = p_order_id;
    IF v_owner != p_user_id THEN
      RAISE_APPLICATION_ERROR(-20020, 'Недостаточно прав: можно изменять только свои заказы');
    END IF;
  END CHECK_BOOKING_PERMISSION;

  --------------------------------------
  -- ADMINS CRUD
  --------------------------------------
  PROCEDURE ADD_ADMIN(p_user_id IN USERS.USER_ID%TYPE, p_role_name IN ADMINS.ROLE_NAME%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
    v_id ADMINS.ADMIN_ID%TYPE;
  BEGIN
    -- валидация параметров
    VALIDATE_NOT_NULL_NUM(p_user_id, 'p_user_id');
    VALIDATE_NOT_NULL(p_role_name, 'p_role_name');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_USER_EXISTS(p_user_id, 'p_user_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    CHECK_ADMIN_ROLE(p_current_user, 'GLAV_ADMIN');

    SELECT admins_seq.NEXTVAL INTO v_id FROM dual;
    INSERT INTO ADMINS(admin_id, user_id, role_name) VALUES(v_id, p_user_id, p_role_name);
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в ADD_ADMIN');
    WHEN OTHERS THEN
      HANDLE_ERROR('ADD_ADMIN', c_err_system);
  END ADD_ADMIN;

  PROCEDURE UPDATE_ADMIN(p_admin_id IN ADMINS.ADMIN_ID%TYPE, p_role_name IN ADMINS.ROLE_NAME%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_admin_id, 'p_admin_id');
    VALIDATE_NOT_NULL(p_role_name, 'p_role_name');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_ADMIN_EXISTS(p_admin_id, 'p_admin_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    CHECK_ADMIN_ROLE(p_current_user, 'GLAV_ADMIN');

    UPDATE ADMINS SET role_name = p_role_name WHERE admin_id = p_admin_id;
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в UPDATE_ADMIN');
    WHEN OTHERS THEN
      HANDLE_ERROR('UPDATE_ADMIN', c_err_system);
  END UPDATE_ADMIN;

  PROCEDURE DELETE_ADMIN(p_admin_id IN ADMINS.ADMIN_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_admin_id, 'p_admin_id');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_ADMIN_EXISTS(p_admin_id, 'p_admin_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    CHECK_ADMIN_ROLE(p_current_user, 'GLAV_ADMIN');

    DELETE FROM ADMINS WHERE admin_id = p_admin_id;
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в DELETE_ADMIN');
    WHEN OTHERS THEN
      HANDLE_ERROR('DELETE_ADMIN', c_err_system);
  END DELETE_ADMIN;

  --------------------------------------
  -- USERS CRUD
  --------------------------------------
  PROCEDURE ADD_USER(p_login IN USERS.LOGIN%TYPE, p_email IN USERS.EMAIL%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
    v_id USERS.USER_ID%TYPE;
  BEGIN
    VALIDATE_NOT_NULL(p_login, 'p_login');
    VALIDATE_STRING_LENGTH(p_login, 'p_login', 200);
    VALIDATE_NOT_NULL(p_email, 'p_email');
    VALIDATE_STRING_LENGTH(p_email, 'p_email', 320);
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_USER_EXISTS(p_current_user, 'p_current_user');
    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    SELECT users_seq.NEXTVAL INTO v_id FROM dual;
    INSERT INTO USERS(user_id, login, email) VALUES(v_id, p_login, p_email);
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в ADD_USER');
    WHEN OTHERS THEN
      HANDLE_ERROR('ADD_USER', c_err_system);
  END ADD_USER;

  PROCEDURE UPDATE_USER(p_user_id IN USERS.USER_ID%TYPE, p_login IN USERS.LOGIN%TYPE, p_email IN USERS.EMAIL%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_user_id, 'p_user_id');
    VALIDATE_NOT_NULL(p_login, 'p_login');
    VALIDATE_STRING_LENGTH(p_login, 'p_login', 200);
    VALIDATE_NOT_NULL(p_email, 'p_email');
    VALIDATE_STRING_LENGTH(p_email, 'p_email', 320);
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_USER_EXISTS(p_user_id, 'p_user_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');
    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    UPDATE USERS SET login = p_login, email = p_email WHERE user_id = p_user_id;
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в UPDATE_USER');
    WHEN OTHERS THEN
      HANDLE_ERROR('UPDATE_USER', c_err_system);
  END UPDATE_USER;

  PROCEDURE DELETE_USER(p_user_id IN USERS.USER_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_user_id, 'p_user_id');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_USER_EXISTS(p_user_id, 'p_user_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');
    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    DELETE FROM USERS WHERE user_id = p_user_id;
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в DELETE_USER');
    WHEN OTHERS THEN
      HANDLE_ERROR('DELETE_USER', c_err_system);
  END DELETE_USER;

  --------------------------------------
  -- STUDIOS CRUD
  --------------------------------------
  PROCEDURE ADD_STUDIO(p_name IN STUDIOS.NAME%TYPE, p_description IN STUDIOS.DESCRIPTION%TYPE,
                       p_price IN STUDIOS.PRICE_PER_HOUR%TYPE, p_square IN STUDIOS.SQUARE%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
    v_id STUDIOS.STUDIO_ID%TYPE;
  BEGIN
    VALIDATE_NOT_NULL(p_name, 'p_name');
    VALIDATE_STRING_LENGTH(p_name, 'p_name', 200);
    VALIDATE_STRING_LENGTH(p_description, 'p_description', 2000);
    VALIDATE_NOT_NULL_NUM(p_price, 'p_price');
    VALIDATE_POSITIVE_NUMBER(p_price, 'p_price');
    VALIDATE_NOT_NULL_NUM(p_square, 'p_square');
    VALIDATE_POSITIVE_NUMBER(p_square, 'p_square');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_USER_EXISTS(p_current_user, 'p_current_user');
    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    SELECT studios_seq.NEXTVAL INTO v_id FROM dual;
    INSERT INTO STUDIOS(studio_id, name, description, price_per_hour, square) VALUES(v_id, p_name, p_description, p_price, p_square);
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в ADD_STUDIO');
    WHEN OTHERS THEN
      HANDLE_ERROR('ADD_STUDIO', c_err_system);
  END ADD_STUDIO;

  PROCEDURE UPDATE_STUDIO(p_studio_id IN STUDIOS.STUDIO_ID%TYPE, p_name IN STUDIOS.NAME%TYPE,
                          p_description IN STUDIOS.DESCRIPTION%TYPE, p_price IN STUDIOS.PRICE_PER_HOUR%TYPE,
                          p_square IN STUDIOS.SQUARE%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_studio_id, 'p_studio_id');
    VALIDATE_NOT_NULL(p_name, 'p_name');
    VALIDATE_STRING_LENGTH(p_name, 'p_name', 200);
    VALIDATE_STRING_LENGTH(p_description, 'p_description', 2000);
    VALIDATE_NOT_NULL_NUM(p_price, 'p_price');
    VALIDATE_POSITIVE_NUMBER(p_price, 'p_price');
    VALIDATE_NOT_NULL_NUM(p_square, 'p_square');
    VALIDATE_POSITIVE_NUMBER(p_square, 'p_square');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_STUDIO_EXISTS(p_studio_id, 'p_studio_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');
    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    UPDATE STUDIOS SET name = p_name, description = p_description, price_per_hour = p_price, square = p_square WHERE studio_id = p_studio_id;
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в UPDATE_STUDIO');
    WHEN OTHERS THEN
      HANDLE_ERROR('UPDATE_STUDIO', c_err_system);
  END UPDATE_STUDIO;

  PROCEDURE DELETE_STUDIO(p_studio_id IN STUDIOS.STUDIO_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_studio_id, 'p_studio_id');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_STUDIO_EXISTS(p_studio_id, 'p_studio_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');
    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    DELETE FROM STUDIOS WHERE studio_id = p_studio_id;
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в DELETE_STUDIO');
    WHEN OTHERS THEN
      HANDLE_ERROR('DELETE_STUDIO', c_err_system);
  END DELETE_STUDIO;

  --------------------------------------
  -- SERVICES CRUD
  --------------------------------------
  PROCEDURE ADD_SERVICE(p_name IN SERVICES.NAME%TYPE, p_price IN SERVICES.PRICE%TYPE, p_description IN SERVICES.DESCRIPTION%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
    v_id SERVICES.SERVICE_ID%TYPE;
  BEGIN
    VALIDATE_NOT_NULL(p_name, 'p_name');
    VALIDATE_STRING_LENGTH(p_name, 'p_name', 200);
    VALIDATE_STRING_LENGTH(p_description, 'p_description', 2000);
    VALIDATE_NOT_NULL_NUM(p_price, 'p_price');
    VALIDATE_POSITIVE_NUMBER(p_price, 'p_price');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_USER_EXISTS(p_current_user, 'p_current_user');
    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    SELECT services_seq.NEXTVAL INTO v_id FROM dual;
    INSERT INTO SERVICES(service_id, name, price, description) VALUES(v_id, p_name, p_price, p_description);
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в ADD_SERVICE');
    WHEN OTHERS THEN
      HANDLE_ERROR('ADD_SERVICE', c_err_system);
  END ADD_SERVICE;

  PROCEDURE UPDATE_SERVICE(p_service_id IN SERVICES.SERVICE_ID%TYPE, p_name IN SERVICES.NAME%TYPE,
                           p_price IN SERVICES.PRICE%TYPE, p_description IN SERVICES.DESCRIPTION%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_service_id, 'p_service_id');
    VALIDATE_NOT_NULL(p_name, 'p_name');
    VALIDATE_STRING_LENGTH(p_name, 'p_name', 200);
    VALIDATE_STRING_LENGTH(p_description, 'p_description', 2000);
    VALIDATE_NOT_NULL_NUM(p_price, 'p_price');
    VALIDATE_POSITIVE_NUMBER(p_price, 'p_price');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_SERVICE_EXISTS(p_service_id, 'p_service_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');
    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    UPDATE SERVICES SET name = p_name, price = p_price, description = p_description WHERE service_id = p_service_id;
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в UPDATE_SERVICE');
    WHEN OTHERS THEN
      HANDLE_ERROR('UPDATE_SERVICE', c_err_system);
  END UPDATE_SERVICE;

  PROCEDURE DELETE_SERVICE(p_service_id IN SERVICES.SERVICE_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_service_id, 'p_service_id');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_SERVICE_EXISTS(p_service_id, 'p_service_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');
    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    DELETE FROM SERVICES WHERE service_id = p_service_id;
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в DELETE_SERVICE');
    WHEN OTHERS THEN
      HANDLE_ERROR('DELETE_SERVICE', c_err_system);
  END DELETE_SERVICE;

  --------------------------------------
  -- STUDIO_RENT_ORDERS CRUD
  --------------------------------------
  PROCEDURE ADD_BOOKING(
    p_user_id     IN STUDIO_RENT_ORDERS.USER_ID%TYPE,
    p_studio_id   IN STUDIO_RENT_ORDERS.STUDIO_ID%TYPE,
    p_rent_date   IN STUDIO_RENT_ORDERS.RENT_DATE%TYPE,
    p_start_time  IN STUDIO_RENT_ORDERS.START_TIME%TYPE,
    p_end_time    IN STUDIO_RENT_ORDERS.END_TIME%TYPE,
    p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE
  ) IS
    v_order_id STUDIO_RENT_ORDERS.ORDER_ID%TYPE;
    v_count NUMBER;
  BEGIN
    -- валидация входных параметров
    VALIDATE_NOT_NULL_NUM(p_user_id, 'p_user_id');
    VALIDATE_NOT_NULL_NUM(p_studio_id, 'p_studio_id');
    VALIDATE_NOT_NULL(p_rent_date, 'p_rent_date');
    VALIDATE_NOT_NULL(p_start_time, 'p_start_time');
    VALIDATE_NOT_NULL(p_end_time, 'p_end_time');
    VALIDATE_NOT_NULL_NUM(p_total_price, 'p_total_price');
    VALIDATE_POSITIVE_NUMBER(p_total_price, 'p_total_price');

    CHECK_USER_EXISTS(p_user_id, 'p_user_id');
    CHECK_STUDIO_EXISTS(p_studio_id, 'p_studio_id');

    -- логическая проверка: start < end
    IF p_start_time >= p_end_time THEN
      RAISE_APPLICATION_ERROR(-20094, 'p_start_time должен быть меньше p_end_time');
    END IF;

    SELECT COUNT(*) INTO v_count
    FROM STUDIO_RENT_ORDERS
    WHERE studio_id = p_studio_id AND rent_date = p_rent_date
      AND status = 'APPROVED'
      AND (p_start_time < end_time AND p_end_time > start_time);

    IF v_count > 0 THEN
      RAISE_APPLICATION_ERROR(-20001, 'Пересечение бронирований!');
    END IF;

    SELECT orders_seq.NEXTVAL INTO v_order_id FROM dual;
    INSERT INTO STUDIO_RENT_ORDERS(order_id, user_id, studio_id, rent_date, start_time, end_time, total_price)
      VALUES(v_order_id, p_user_id, p_studio_id, p_rent_date, p_start_time, p_end_time, p_total_price);
    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в ADD_BOOKING');
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Ссылка на несуществующую запись в ADD_BOOKING');
    WHEN OTHERS THEN
      HANDLE_ERROR('ADD_BOOKING', c_err_system);
  END ADD_BOOKING;

  PROCEDURE DELETE_BOOKING(
    p_order_id     IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  ) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_order_id, 'p_order_id');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_ORDER_EXISTS(p_order_id, 'p_order_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    -- Проверка прав на изменение заказа
    CHECK_BOOKING_PERMISSION(p_current_user, p_order_id);

    -- Сначала архивируем услуги заказа
    INSERT INTO ARCHIVED_ORDER_SERVICES(order_id, service_id)
    SELECT order_id, service_id
    FROM ORDER_SERVICES
    WHERE order_id = p_order_id;

    -- Затем архивируем сам заказ
    INSERT INTO ARCHIVED_STUDIO_RENT_ORDERS(order_id, user_id, studio_id, rent_date, start_time, end_time, total_price)
    SELECT order_id, user_id, studio_id, rent_date, start_time, end_time, total_price
    FROM STUDIO_RENT_ORDERS
    WHERE order_id = p_order_id;

    -- Удаляем сначала услуги, потом заказ
    DELETE FROM ORDER_SERVICES WHERE order_id = p_order_id;
    DELETE FROM STUDIO_RENT_ORDERS WHERE order_id = p_order_id;

    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в DELETE_BOOKING');
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Ссылка на несуществующую запись в DELETE_BOOKING');
    WHEN OTHERS THEN
      HANDLE_ERROR('DELETE_BOOKING', c_err_system);
  END DELETE_BOOKING;

  PROCEDURE UPDATE_BOOKING(
    p_order_id    IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE,
    p_start_time  IN STUDIO_RENT_ORDERS.START_TIME%TYPE,
    p_end_time    IN STUDIO_RENT_ORDERS.END_TIME%TYPE,
    p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE,
    p_status      IN STUDIO_RENT_ORDERS.STATUS%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  ) IS
    v_count NUMBER;
    v_studio_id STUDIO_RENT_ORDERS.STUDIO_ID%TYPE;
    v_rent_date STUDIO_RENT_ORDERS.RENT_DATE%TYPE;
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_order_id, 'p_order_id');
    VALIDATE_NOT_NULL(p_start_time, 'p_start_time');
    VALIDATE_NOT_NULL(p_end_time, 'p_end_time');
    VALIDATE_NOT_NULL_NUM(p_total_price, 'p_total_price');
    VALIDATE_POSITIVE_NUMBER(p_total_price, 'p_total_price');
    VALIDATE_NOT_NULL(p_status, 'p_status');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_ORDER_EXISTS(p_order_id, 'p_order_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    CHECK_BOOKING_PERMISSION(p_current_user, p_order_id);

    SELECT studio_id, rent_date INTO v_studio_id, v_rent_date
    FROM STUDIO_RENT_ORDERS WHERE order_id = p_order_id;

    IF p_start_time >= p_end_time THEN
      RAISE_APPLICATION_ERROR(-20094, 'p_start_time должен быть меньше p_end_time');
    END IF;

    SELECT COUNT(*) INTO v_count
    FROM STUDIO_RENT_ORDERS
    WHERE studio_id = v_studio_id AND rent_date = v_rent_date
      AND status = 'APPROVED' AND order_id <> p_order_id
      AND (p_start_time < end_time AND p_end_time > start_time);

    IF v_count > 0 THEN
      RAISE_APPLICATION_ERROR(-20002, 'Пересечение бронирований при обновлении!');
    END IF;

    UPDATE STUDIO_RENT_ORDERS
      SET start_time = p_start_time, end_time = p_end_time, total_price = p_total_price, status = p_status
      WHERE order_id = p_order_id;

    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в UPDATE_BOOKING');
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Ссылка на несуществующую запись в UPDATE_BOOKING');
    WHEN OTHERS THEN
      HANDLE_ERROR('UPDATE_BOOKING', c_err_system);
  END UPDATE_BOOKING;

  --------------------------------------
  -- ORDER_SERVICES CRUD
  --------------------------------------
  PROCEDURE UPDATE_ORDER_SERVICE(
    p_order_id    IN ORDER_SERVICES.ORDER_ID%TYPE,
    p_service_id  IN ORDER_SERVICES.SERVICE_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  ) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_order_id, 'p_order_id');
    VALIDATE_NOT_NULL_NUM(p_service_id, 'p_service_id');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_ORDER_EXISTS(p_order_id, 'p_order_id');
    CHECK_SERVICE_EXISTS(p_service_id, 'p_service_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    NULL;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в UPDATE_ORDER_SERVICE');
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Ссылка на несуществующую запись в UPDATE_ORDER_SERVICE');
    WHEN OTHERS THEN
      HANDLE_ERROR('UPDATE_ORDER_SERVICE', c_err_system);
  END UPDATE_ORDER_SERVICE;

  PROCEDURE ADD_ORDER_SERVICE(
    p_order_id     IN ORDER_SERVICES.ORDER_ID%TYPE,
    p_service_id   IN ORDER_SERVICES.SERVICE_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  ) IS
    v_service_price SERVICES.PRICE%TYPE;
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_order_id, 'p_order_id');
    VALIDATE_NOT_NULL_NUM(p_service_id, 'p_service_id');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_ORDER_EXISTS(p_order_id, 'p_order_id');
    CHECK_SERVICE_EXISTS(p_service_id, 'p_service_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    CHECK_BOOKING_PERMISSION(p_current_user, p_order_id);

    SELECT price INTO v_service_price FROM SERVICES WHERE service_id = p_service_id;

    INSERT INTO ORDER_SERVICES(order_id, service_id) VALUES(p_order_id, p_service_id);

    UPDATE STUDIO_RENT_ORDERS
       SET total_price = total_price + v_service_price
     WHERE order_id = p_order_id;

    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в ADD_ORDER_SERVICE');
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Ссылка на несуществующую запись в ADD_ORDER_SERVICE');
    WHEN OTHERS THEN
      HANDLE_ERROR('ADD_ORDER_SERVICE', c_err_system);
  END ADD_ORDER_SERVICE;

  PROCEDURE DELETE_ORDER_SERVICE(
    p_order_id     IN ORDER_SERVICES.ORDER_ID%TYPE,
    p_service_id   IN ORDER_SERVICES.SERVICE_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  ) IS
    v_service_price SERVICES.PRICE%TYPE;
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_order_id, 'p_order_id');
    VALIDATE_NOT_NULL_NUM(p_service_id, 'p_service_id');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_ORDER_EXISTS(p_order_id, 'p_order_id');
    CHECK_SERVICE_EXISTS(p_service_id, 'p_service_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    CHECK_BOOKING_PERMISSION(p_current_user, p_order_id);

    SELECT price INTO v_service_price FROM SERVICES WHERE service_id = p_service_id;

    DELETE FROM ORDER_SERVICES WHERE order_id = p_order_id AND service_id = p_service_id;

    UPDATE STUDIO_RENT_ORDERS
       SET total_price = total_price - v_service_price
     WHERE order_id = p_order_id;

    COMMIT;

  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в DELETE_ORDER_SERVICE');
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Ссылка на несуществующую запись в DELETE_ORDER_SERVICE');
    WHEN OTHERS THEN
      HANDLE_ERROR('DELETE_ORDER_SERVICE', c_err_system);
  END DELETE_ORDER_SERVICE;

  --------------------------------------
  -- Архивирование бронирований
  --------------------------------------
  PROCEDURE ARCHIVE_BOOKINGS(p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    INSERT INTO ARCHIVED_STUDIO_RENT_ORDERS(order_id, user_id, studio_id, rent_date, start_time, end_time, total_price)
      SELECT order_id, user_id, studio_id, rent_date, start_time, end_time, total_price
      FROM STUDIO_RENT_ORDERS
      WHERE status = 'CANCELLED' OR end_time < SYSTIMESTAMP;

    DELETE FROM STUDIO_RENT_ORDERS WHERE status = 'CANCELLED' OR end_time < SYSTIMESTAMP;
    COMMIT;
  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в ARCHIVE_BOOKINGS');
    WHEN OTHERS THEN
      HANDLE_ERROR('ARCHIVE_BOOKINGS', c_err_system);
  END ARCHIVE_BOOKINGS;

  PROCEDURE ARCHIVE_BOOKING_BY_ID(
    p_order_id     IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  ) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_order_id, 'p_order_id');
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');

    CHECK_ORDER_EXISTS(p_order_id, 'p_order_id');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    -- Копируем услуги заказа в архив
    INSERT INTO ARCHIVED_ORDER_SERVICES(order_id, service_id)
    SELECT order_id, service_id
    FROM ORDER_SERVICES
    WHERE order_id = p_order_id;

    -- Копируем заказ в архив
    INSERT INTO ARCHIVED_STUDIO_RENT_ORDERS(order_id, user_id, studio_id, rent_date, start_time, end_time, total_price)
    SELECT order_id, user_id, studio_id, rent_date, start_time, end_time, total_price
    FROM STUDIO_RENT_ORDERS
    WHERE order_id = p_order_id;

    -- Удаляем сначала услуги, потом заказ
    DELETE FROM ORDER_SERVICES WHERE order_id = p_order_id;
    DELETE FROM STUDIO_RENT_ORDERS WHERE order_id = p_order_id;

    COMMIT;
  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в ARCHIVE_BOOKING_BY_ID');
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20093, 'Ссылка на несуществующую запись в ARCHIVE_BOOKING_BY_ID');
    WHEN OTHERS THEN
      HANDLE_ERROR('ARCHIVE_BOOKING_BY_ID', c_err_system);
  END ARCHIVE_BOOKING_BY_ID;

  PROCEDURE ARCHIVE_BOOKINGS_BY_STATUS(p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    VALIDATE_NOT_NULL_NUM(p_current_user, 'p_current_user');
    CHECK_USER_EXISTS(p_current_user, 'p_current_user');

    CHECK_ADMIN_ROLE(p_current_user, 'ADMIN');

    -- Копируем услуги в архив
    INSERT INTO ARCHIVED_ORDER_SERVICES(order_id, service_id)
    SELECT os.order_id, os.service_id
    FROM ORDER_SERVICES os
    JOIN STUDIO_RENT_ORDERS o ON o.order_id = os.order_id
    WHERE o.status IN ('APPROVED', 'CANCELLED');

    -- Копируем сами заказы в архив
    INSERT INTO ARCHIVED_STUDIO_RENT_ORDERS(order_id, user_id, studio_id, rent_date, start_time, end_time, total_price)
    SELECT order_id, user_id, studio_id, rent_date, start_time, end_time, total_price
    FROM STUDIO_RENT_ORDERS
    WHERE status IN ('APPROVED', 'CANCELLED');

    -- Удаляем сначала услуги, потом заказы
    DELETE FROM ORDER_SERVICES WHERE order_id IN (
        SELECT order_id FROM STUDIO_RENT_ORDERS WHERE status IN ('APPROVED', 'CANCELLED')
    );
    DELETE FROM STUDIO_RENT_ORDERS WHERE status IN ('APPROVED', 'CANCELLED');

    COMMIT;
  EXCEPTION
    WHEN VALUE_ERROR OR INVALID_NUMBER THEN
      RAISE_APPLICATION_ERROR(-20091, 'Неверный формат параметров в ARCHIVE_BOOKINGS_BY_STATUS');
    WHEN OTHERS THEN
      HANDLE_ERROR('ARCHIVE_BOOKINGS_BY_STATUS', c_err_system);
  END ARCHIVE_BOOKINGS_BY_STATUS;

END CRS_CUR;
/

--------------------------------------------------------------------------------
-- Аналитика
CREATE OR REPLACE PACKAGE CRS_ANALYTICS AS
  c_err_input   CONSTANT VARCHAR2(30) := 'INPUT_ERROR';
  c_err_business CONSTANT VARCHAR2(30) := 'BUSINESS_ERROR';
  c_err_system   CONSTANT VARCHAR2(30) := 'SYSTEM_ERROR';

  -- Количество бронирований по периодам
  PROCEDURE GET_BOOKINGS_BY_PERIOD(
    p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL,
    p_start_date   IN DATE DEFAULT NULL,
    p_end_date     IN DATE DEFAULT NULL
  );

  -- Популярные студии
  PROCEDURE GET_POPULAR_STUDIOS(
    p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL,
    p_limit        IN NUMBER DEFAULT NULL
  );

  -- Популярные услуги
  PROCEDURE GET_POPULAR_SERVICES(
    p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL,
    p_limit        IN NUMBER DEFAULT NULL
  );

  -- Общее количество бронирований
  PROCEDURE GET_TOTAL_BOOKINGS(
    p_current_user IN USERS.USER_ID%TYPE DEFAULT NULL,
    p_count        OUT NUMBER
);

  PROCEDURE HANDLE_ERROR(p_context IN VARCHAR2, p_error_type IN VARCHAR2);
END CRS_ANALYTICS;
/

CREATE OR REPLACE PACKAGE BODY CRS_ANALYTICS AS

  --------------------------------------
  -- Общий обработчик ошибок
  --------------------------------------
  PROCEDURE HANDLE_ERROR(p_context IN VARCHAR2, p_error_type IN VARCHAR2) IS
    v_msg VARCHAR2(4000);
  BEGIN
    ROLLBACK;
    v_msg := 'Context=[' || p_context || '] Type=[' || p_error_type || '] Orig=[' || SQLERRM || ']';
    RAISE_APPLICATION_ERROR(-20099, v_msg);
  END HANDLE_ERROR;


  --------------------------------------
  -- Проверка прав администратора
  --------------------------------------
  PROCEDURE CHECK_ADMIN_ROLE(p_user_id IN USERS.USER_ID%TYPE) IS
    v_role ADMINS.ROLE_NAME%TYPE;
  BEGIN
    -- NULL проверка всегда первой
    IF p_user_id IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр p_user_id не может быть NULL');
    END IF;

    BEGIN
      SELECT role_name INTO v_role FROM ADMINS WHERE user_id = p_user_id;
    EXCEPTION
      WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR(-20011, 'Пользователь не является администратором');
    END;

    IF v_role NOT IN ('ADMIN', 'GLAV_ADMIN') THEN
      RAISE_APPLICATION_ERROR(-20010, 'Недостаточно прав для выполнения операции');
    END IF;

  EXCEPTION
    WHEN OTHERS THEN
      HANDLE_ERROR('CHECK_ADMIN_ROLE', 'SYSTEM_ERROR');
  END CHECK_ADMIN_ROLE;


  --------------------------------------
  -- Количество бронирований по дням
  --------------------------------------
  PROCEDURE GET_BOOKINGS_BY_PERIOD(
    p_current_user IN USERS.USER_ID%TYPE,
    p_start_date   IN DATE,
    p_end_date     IN DATE
  ) IS
  BEGIN
    IF p_current_user IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр p_current_user не может быть NULL');
    END IF;

    IF p_start_date IS NULL OR p_end_date IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметры дат не могут быть NULL');
    END IF;

    IF p_start_date > p_end_date THEN
      RAISE_APPLICATION_ERROR(-20090, 'Дата начала не может быть позже даты окончания');
    END IF;

    CHECK_ADMIN_ROLE(p_current_user);

    FOR r IN (
      SELECT TRUNC(rent_date) AS day, COUNT(*) AS bookings_count
      FROM (
        SELECT rent_date FROM STUDIO_RENT_ORDERS
        WHERE rent_date BETWEEN p_start_date AND p_end_date
        UNION ALL
        SELECT rent_date FROM ARCHIVED_STUDIO_RENT_ORDERS
        WHERE rent_date BETWEEN p_start_date AND p_end_date
      )
      GROUP BY TRUNC(rent_date)
      ORDER BY day
    ) LOOP
      DBMS_OUTPUT.PUT_LINE('Дата: ' || r.day || ' | Бронирований: ' || r.bookings_count);
    END LOOP;

  EXCEPTION
    WHEN OTHERS THEN
      HANDLE_ERROR('GET_BOOKINGS_BY_PERIOD', 'SYSTEM_ERROR');
  END GET_BOOKINGS_BY_PERIOD;


  --------------------------------------
  -- Популярные студии
  --------------------------------------
  PROCEDURE GET_POPULAR_STUDIOS(
    p_current_user IN USERS.USER_ID%TYPE,
    p_limit        IN NUMBER
  ) IS
  BEGIN
    IF p_current_user IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр p_current_user не может быть NULL');
    END IF;

    IF p_limit IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр p_limit не может быть NULL');
    END IF;

    IF p_limit <= 0 THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр p_limit должен быть положительным числом');
    END IF;

    CHECK_ADMIN_ROLE(p_current_user);

    FOR r IN (
      SELECT s.name AS studio_name, COUNT(*) AS bookings_count
      FROM (
        SELECT studio_id FROM STUDIO_RENT_ORDERS
        UNION ALL
        SELECT studio_id FROM ARCHIVED_STUDIO_RENT_ORDERS
      ) o
      JOIN STUDIOS s ON o.studio_id = s.studio_id
      GROUP BY s.name
      ORDER BY bookings_count DESC
      FETCH FIRST p_limit ROWS ONLY
    ) LOOP
      DBMS_OUTPUT.PUT_LINE('Студия: ' || r.studio_name || ' | Бронирований: ' || r.bookings_count);
    END LOOP;

  EXCEPTION
    WHEN OTHERS THEN
      HANDLE_ERROR('GET_POPULAR_STUDIOS', 'SYSTEM_ERROR');
  END GET_POPULAR_STUDIOS;


  --------------------------------------
  -- Популярные услуги
  --------------------------------------
  PROCEDURE GET_POPULAR_SERVICES(
    p_current_user IN USERS.USER_ID%TYPE,
    p_limit        IN NUMBER
  ) IS
  BEGIN
    IF p_current_user IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр p_current_user не может быть NULL');
    END IF;

    IF p_limit IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр p_limit не может быть NULL');
    END IF;

    IF p_limit <= 0 THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр p_limit должен быть положительным числом');
    END IF;

    CHECK_ADMIN_ROLE(p_current_user);

    FOR r IN (
      SELECT srv.name AS service_name, COUNT(*) AS usage_count
      FROM (
        SELECT service_id FROM ORDER_SERVICES
        UNION ALL
        SELECT service_id FROM ARCHIVED_ORDER_SERVICES
      ) all_services
      JOIN SERVICES srv ON all_services.service_id = srv.service_id
      GROUP BY srv.name
      ORDER BY usage_count DESC
      FETCH FIRST p_limit ROWS ONLY
    ) LOOP
      DBMS_OUTPUT.PUT_LINE('Услуга: ' || r.service_name || ' | Использований: ' || r.usage_count);
    END LOOP;

  EXCEPTION
    WHEN OTHERS THEN
      HANDLE_ERROR('GET_POPULAR_SERVICES', 'SYSTEM_ERROR');
  END GET_POPULAR_SERVICES;


  --------------------------------------
  -- Общее количество бронирований
  --------------------------------------
  PROCEDURE GET_TOTAL_BOOKINGS(
    p_current_user IN USERS.USER_ID%TYPE,
    p_count        OUT NUMBER
) IS
    v_count NUMBER;
BEGIN
    IF p_current_user IS NULL THEN
      RAISE_APPLICATION_ERROR(-20090, 'Параметр p_current_user не может быть NULL');
    END IF;

    CHECK_ADMIN_ROLE(p_current_user);

    SELECT COUNT(*) INTO v_count
    FROM (
      SELECT order_id FROM STUDIO_RENT_ORDERS
      UNION ALL
      SELECT order_id FROM ARCHIVED_STUDIO_RENT_ORDERS
    );

    DBMS_OUTPUT.PUT_LINE('Общее количество бронирований: ' || v_count);

    -- вывод для APEX
    p_count := v_count;

EXCEPTION
    WHEN OTHERS THEN
      HANDLE_ERROR('GET_TOTAL_BOOKINGS', 'SYSTEM_ERROR');
END GET_TOTAL_BOOKINGS;


END CRS_ANALYTICS;
/


--------------------------------------------------------------------------------

CREATE OR REPLACE PACKAGE CRS_CUR_USER_WRAPPER AUTHID CURRENT_USER AS
  PROCEDURE ADD_BOOKING(
    p_user_id     IN STUDIO_RENT_ORDERS.USER_ID%TYPE,
    p_studio_id   IN STUDIO_RENT_ORDERS.STUDIO_ID%TYPE,
    p_rent_date   IN STUDIO_RENT_ORDERS.RENT_DATE%TYPE,
    p_start_time  IN STUDIO_RENT_ORDERS.START_TIME%TYPE,
    p_end_time    IN STUDIO_RENT_ORDERS.END_TIME%TYPE,
    p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE
  );

  PROCEDURE UPDATE_BOOKING(
    p_order_id    IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE,
    p_start_time  IN STUDIO_RENT_ORDERS.START_TIME%TYPE,
    p_end_time    IN STUDIO_RENT_ORDERS.END_TIME%TYPE,
    p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE,
    p_status      IN STUDIO_RENT_ORDERS.STATUS%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  );

  PROCEDURE DELETE_BOOKING(
    p_order_id     IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  );

  PROCEDURE ADD_ORDER_SERVICE(
    p_order_id     IN ORDER_SERVICES.ORDER_ID%TYPE,
    p_service_id   IN ORDER_SERVICES.SERVICE_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  );

  PROCEDURE DELETE_ORDER_SERVICE(
    p_order_id     IN ORDER_SERVICES.ORDER_ID%TYPE,
    p_service_id   IN ORDER_SERVICES.SERVICE_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  );

END CRS_CUR_USER_WRAPPER;
/

CREATE OR REPLACE PACKAGE BODY CRS_CUR_USER_WRAPPER AS

  PROCEDURE ADD_BOOKING(
    p_user_id     IN STUDIO_RENT_ORDERS.USER_ID%TYPE,
    p_studio_id   IN STUDIO_RENT_ORDERS.STUDIO_ID%TYPE,
    p_rent_date   IN STUDIO_RENT_ORDERS.RENT_DATE%TYPE,
    p_start_time  IN STUDIO_RENT_ORDERS.START_TIME%TYPE,
    p_end_time    IN STUDIO_RENT_ORDERS.END_TIME%TYPE,
    p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE
  ) IS
  BEGIN
    CRS_CUR.ADD_BOOKING(p_user_id, p_studio_id, p_rent_date, p_start_time, p_end_time, p_total_price);
  END ADD_BOOKING;

  PROCEDURE UPDATE_BOOKING(
    p_order_id    IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE,
    p_start_time  IN STUDIO_RENT_ORDERS.START_TIME%TYPE,
    p_end_time    IN STUDIO_RENT_ORDERS.END_TIME%TYPE,
    p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE,
    p_status      IN STUDIO_RENT_ORDERS.STATUS%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  ) IS
  BEGIN
    CRS_CUR.UPDATE_BOOKING(p_order_id, p_start_time, p_end_time, p_total_price, p_status, p_current_user);
  END UPDATE_BOOKING;

  PROCEDURE DELETE_BOOKING(
    p_order_id     IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  ) IS
  BEGIN
    CRS_CUR.DELETE_BOOKING(p_order_id, p_current_user);
  END DELETE_BOOKING;

  PROCEDURE ADD_ORDER_SERVICE(
    p_order_id     IN ORDER_SERVICES.ORDER_ID%TYPE,
    p_service_id   IN ORDER_SERVICES.SERVICE_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  ) IS
  BEGIN
    CRS_CUR.ADD_ORDER_SERVICE(p_order_id, p_service_id, p_current_user);
  END ADD_ORDER_SERVICE;

  PROCEDURE DELETE_ORDER_SERVICE(
    p_order_id     IN ORDER_SERVICES.ORDER_ID%TYPE,
    p_service_id   IN ORDER_SERVICES.SERVICE_ID%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  ) IS
  BEGIN
    CRS_CUR.DELETE_ORDER_SERVICE(p_order_id, p_service_id, p_current_user);
  END DELETE_ORDER_SERVICE;

END CRS_CUR_USER_WRAPPER;
/

CREATE OR REPLACE PACKAGE CRS_CUR_ADMIN_WRAPPER AUTHID CURRENT_USER AS

  -- USERS CRUD
  PROCEDURE ADD_USER(p_login IN USERS.LOGIN%TYPE, p_email IN USERS.EMAIL%TYPE, p_current_user IN USERS.USER_ID%TYPE);
  PROCEDURE UPDATE_USER(p_user_id IN USERS.USER_ID%TYPE, p_login IN USERS.LOGIN%TYPE, p_email IN USERS.EMAIL%TYPE, p_current_user IN USERS.USER_ID%TYPE);
  PROCEDURE DELETE_USER(p_user_id IN USERS.USER_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE);

  -- STUDIOS CRUD
  PROCEDURE ADD_STUDIO(p_name IN STUDIOS.NAME%TYPE, p_description IN STUDIOS.DESCRIPTION%TYPE,
                       p_price IN STUDIOS.PRICE_PER_HOUR%TYPE, p_square IN STUDIOS.SQUARE%TYPE, p_current_user IN USERS.USER_ID%TYPE);
  PROCEDURE UPDATE_STUDIO(p_studio_id IN STUDIOS.STUDIO_ID%TYPE, p_name IN STUDIOS.NAME%TYPE,
                          p_description IN STUDIOS.DESCRIPTION%TYPE, p_price IN STUDIOS.PRICE_PER_HOUR%TYPE,
                          p_square IN STUDIOS.SQUARE%TYPE, p_current_user IN USERS.USER_ID%TYPE);
  PROCEDURE DELETE_STUDIO(p_studio_id IN STUDIOS.STUDIO_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE);

  -- SERVICES CRUD
  PROCEDURE ADD_SERVICE(p_name IN SERVICES.NAME%TYPE, p_price IN SERVICES.PRICE%TYPE, p_description IN SERVICES.DESCRIPTION%TYPE, p_current_user IN USERS.USER_ID%TYPE);
  PROCEDURE UPDATE_SERVICE(p_service_id IN SERVICES.SERVICE_ID%TYPE, p_name IN SERVICES.NAME%TYPE,
                           p_price IN SERVICES.PRICE%TYPE, p_description IN SERVICES.DESCRIPTION%TYPE, p_current_user IN USERS.USER_ID%TYPE);
  PROCEDURE DELETE_SERVICE(p_service_id IN SERVICES.SERVICE_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE);

  -- STUDIO_RENT_ORDERS CRUD
  PROCEDURE ADD_BOOKING(
    p_user_id     IN STUDIO_RENT_ORDERS.USER_ID%TYPE,
    p_studio_id   IN STUDIO_RENT_ORDERS.STUDIO_ID%TYPE,
    p_rent_date   IN STUDIO_RENT_ORDERS.RENT_DATE%TYPE,
    p_start_time  IN STUDIO_RENT_ORDERS.START_TIME%TYPE,
    p_end_time    IN STUDIO_RENT_ORDERS.END_TIME%TYPE,
    p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE
  );
  PROCEDURE UPDATE_BOOKING(
    p_order_id    IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE,
    p_start_time  IN STUDIO_RENT_ORDERS.START_TIME%TYPE,
    p_end_time    IN STUDIO_RENT_ORDERS.END_TIME%TYPE,
    p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE,
    p_status      IN STUDIO_RENT_ORDERS.STATUS%TYPE,
    p_current_user IN USERS.USER_ID%TYPE
  );
  PROCEDURE DELETE_BOOKING(p_order_id IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE);

  -- ORDER_SERVICES CRUD
  PROCEDURE ADD_ORDER_SERVICE(p_order_id IN ORDER_SERVICES.ORDER_ID%TYPE, p_service_id IN ORDER_SERVICES.SERVICE_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE);
  PROCEDURE UPDATE_ORDER_SERVICE(p_order_id IN ORDER_SERVICES.ORDER_ID%TYPE, p_service_id IN ORDER_SERVICES.SERVICE_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE);
  PROCEDURE DELETE_ORDER_SERVICE(p_order_id IN ORDER_SERVICES.ORDER_ID%TYPE, p_service_id IN ORDER_SERVICES.SERVICE_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE);

  -- Архивирование
  PROCEDURE ARCHIVE_BOOKINGS(p_current_user IN USERS.USER_ID%TYPE);
  PROCEDURE ARCHIVE_BOOKING_BY_ID(p_order_id IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE);
  PROCEDURE ARCHIVE_BOOKINGS_BY_STATUS(p_current_user IN USERS.USER_ID%TYPE);

END CRS_CUR_ADMIN_WRAPPER;
/

CREATE OR REPLACE PACKAGE BODY CRS_CUR_ADMIN_WRAPPER AS

  -- USERS CRUD
  PROCEDURE ADD_USER(p_login IN USERS.LOGIN%TYPE, p_email IN USERS.EMAIL%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.ADD_USER(p_login, p_email, p_current_user);
  END ADD_USER;

  PROCEDURE UPDATE_USER(p_user_id IN USERS.USER_ID%TYPE, p_login IN USERS.LOGIN%TYPE, p_email IN USERS.EMAIL%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.UPDATE_USER(p_user_id, p_login, p_email, p_current_user);
  END UPDATE_USER;

  PROCEDURE DELETE_USER(p_user_id IN USERS.USER_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.DELETE_USER(p_user_id, p_current_user);
  END DELETE_USER;

  -- STUDIOS CRUD
  PROCEDURE ADD_STUDIO(p_name IN STUDIOS.NAME%TYPE, p_description IN STUDIOS.DESCRIPTION%TYPE,
                       p_price IN STUDIOS.PRICE_PER_HOUR%TYPE, p_square IN STUDIOS.SQUARE%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.ADD_STUDIO(p_name, p_description, p_price, p_square, p_current_user);
  END ADD_STUDIO;

  PROCEDURE UPDATE_STUDIO(p_studio_id IN STUDIOS.STUDIO_ID%TYPE, p_name IN STUDIOS.NAME%TYPE,
                          p_description IN STUDIOS.DESCRIPTION%TYPE, p_price IN STUDIOS.PRICE_PER_HOUR%TYPE,
                          p_square IN STUDIOS.SQUARE%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.UPDATE_STUDIO(p_studio_id, p_name, p_description, p_price, p_square, p_current_user);
  END UPDATE_STUDIO;

  PROCEDURE DELETE_STUDIO(p_studio_id IN STUDIOS.STUDIO_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.DELETE_STUDIO(p_studio_id, p_current_user);
  END DELETE_STUDIO;

  -- SERVICES CRUD
  PROCEDURE ADD_SERVICE(p_name IN SERVICES.NAME%TYPE, p_price IN SERVICES.PRICE%TYPE, p_description IN SERVICES.DESCRIPTION%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.ADD_SERVICE(p_name, p_price, p_description, p_current_user);
  END ADD_SERVICE;

  PROCEDURE UPDATE_SERVICE(p_service_id IN SERVICES.SERVICE_ID%TYPE, p_name IN SERVICES.NAME%TYPE,
                           p_price IN SERVICES.PRICE%TYPE, p_description IN SERVICES.DESCRIPTION%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.UPDATE_SERVICE(p_service_id, p_name, p_price, p_description, p_current_user);
  END UPDATE_SERVICE;

  PROCEDURE DELETE_SERVICE(p_service_id IN SERVICES.SERVICE_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.DELETE_SERVICE(p_service_id, p_current_user);
  END DELETE_SERVICE;

  -- STUDIO_RENT_ORDERS CRUD
  PROCEDURE ADD_BOOKING(p_user_id IN STUDIO_RENT_ORDERS.USER_ID%TYPE,
                        p_studio_id IN STUDIO_RENT_ORDERS.STUDIO_ID%TYPE,
                        p_rent_date IN STUDIO_RENT_ORDERS.RENT_DATE%TYPE,
                        p_start_time IN STUDIO_RENT_ORDERS.START_TIME%TYPE,
                        p_end_time IN STUDIO_RENT_ORDERS.END_TIME%TYPE,
                        p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE) IS
  BEGIN
    CRS_CUR.ADD_BOOKING(p_user_id, p_studio_id, p_rent_date, p_start_time, p_end_time, p_total_price);
  END ADD_BOOKING;

  PROCEDURE UPDATE_BOOKING(p_order_id IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE,
                           p_start_time IN STUDIO_RENT_ORDERS.START_TIME%TYPE,
                           p_end_time IN STUDIO_RENT_ORDERS.END_TIME%TYPE,
                           p_total_price IN STUDIO_RENT_ORDERS.TOTAL_PRICE%TYPE,
                           p_status IN STUDIO_RENT_ORDERS.STATUS%TYPE,
                           p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.UPDATE_BOOKING(p_order_id, p_start_time, p_end_time, p_total_price, p_status, p_current_user);
  END UPDATE_BOOKING;

  PROCEDURE DELETE_BOOKING(p_order_id IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.DELETE_BOOKING(p_order_id, p_current_user);
  END DELETE_BOOKING;

  -- ORDER_SERVICES CRUD
  PROCEDURE ADD_ORDER_SERVICE(p_order_id IN ORDER_SERVICES.ORDER_ID%TYPE, p_service_id IN ORDER_SERVICES.SERVICE_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.ADD_ORDER_SERVICE(p_order_id, p_service_id, p_current_user);
  END ADD_ORDER_SERVICE;

  PROCEDURE UPDATE_ORDER_SERVICE(p_order_id IN ORDER_SERVICES.ORDER_ID%TYPE, p_service_id IN ORDER_SERVICES.SERVICE_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.UPDATE_ORDER_SERVICE(p_order_id, p_service_id, p_current_user);
  END UPDATE_ORDER_SERVICE;

  PROCEDURE DELETE_ORDER_SERVICE(p_order_id IN ORDER_SERVICES.ORDER_ID%TYPE, p_service_id IN ORDER_SERVICES.SERVICE_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.DELETE_ORDER_SERVICE(p_order_id, p_service_id, p_current_user);
  END DELETE_ORDER_SERVICE;

  -- Архивирование
  PROCEDURE ARCHIVE_BOOKINGS(p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.ARCHIVE_BOOKINGS(p_current_user);
  END ARCHIVE_BOOKINGS;

  PROCEDURE ARCHIVE_BOOKING_BY_ID(p_order_id IN STUDIO_RENT_ORDERS.ORDER_ID%TYPE, p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.ARCHIVE_BOOKING_BY_ID(p_order_id, p_current_user);
  END ARCHIVE_BOOKING_BY_ID;

  PROCEDURE ARCHIVE_BOOKINGS_BY_STATUS(p_current_user IN USERS.USER_ID%TYPE) IS
  BEGIN
    CRS_CUR.ARCHIVE_BOOKINGS_BY_STATUS(p_current_user);
  END ARCHIVE_BOOKINGS_BY_STATUS;

END CRS_CUR_ADMIN_WRAPPER;
/


