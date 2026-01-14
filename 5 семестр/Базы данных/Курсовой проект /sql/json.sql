ALTER SESSION SET CONTAINER=CRS_PDB;
ALTER SESSION SET CURRENT_SCHEMA = CRS_CUR;
SELECT directory_name, directory_path
FROM all_directories
WHERE directory_name = 'DATA_PUMP_DIR';
ALTER SESSION SET CONTAINER = CDB$ROOT;
CREATE OR REPLACE DIRECTORY DATA_PUMP_DIR AS '/u01/app/oracle/json';
GRANT READ, WRITE ON DIRECTORY DATA_PUMP_DIR TO CRS_CUR;

SELECT argument_name, position, in_out, data_type
FROM all_arguments
WHERE package_name = 'DBMS_LOB'
  AND object_name = 'LOADCLOBFROMFILE'
ORDER BY position;

--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE export_users_to_json (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_file    UTL_FILE.FILE_TYPE;
    v_first   BOOLEAN := TRUE;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Экспорт USERS в ' || p_filename);

    v_file := UTL_FILE.FOPEN(p_directory, p_filename, 'w', 32767);

    -- Открываем JSON‑массив
    UTL_FILE.PUT_LINE(v_file, '[');

    FOR u IN (
        SELECT
            user_id,
            login,
            email,
            created_at
        FROM USERS
        ORDER BY user_id
    ) LOOP
        IF v_first THEN
            v_first := FALSE;
        ELSE
            -- запятая между объектами
            UTL_FILE.PUT_LINE(v_file, ',');
        END IF;

        UTL_FILE.PUT_LINE(
            v_file,
            JSON_OBJECT(
                'USER_ID'   VALUE u.user_id,
                'LOGIN'     VALUE u.login,
                'EMAIL'     VALUE u.email,
                'CREATED_AT' VALUE TO_CHAR(
                    u.created_at,
                    'YYYY-MM-DD"T"HH24:MI:SS'
                )
            RETURNING VARCHAR2)
        );
    END LOOP;

    -- Закрываем JSON‑массив
    UTL_FILE.PUT_LINE(v_file, ']');

    UTL_FILE.FCLOSE(v_file);

    DBMS_OUTPUT.PUT_LINE('Экспорт USERS завершен.');
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Ошибка в export_users_to_json: ' || SQLERRM);
        IF UTL_FILE.IS_OPEN(v_file) THEN
            UTL_FILE.FCLOSE(v_file);
        END IF;
        RAISE;
END;
/
CREATE OR REPLACE PROCEDURE import_users_from_json_safe (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_bfile        BFILE;
    v_clob         CLOB;
    v_amount       NUMBER;
    v_dest_off     NUMBER := 1;
    v_src_off      NUMBER := 1;
    v_lang_ctx     NUMBER := 0;
    v_warning      NUMBER;
    v_rows_inserted NUMBER := 0;
    v_count        NUMBER;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Начало процедуры импорта: ' || p_filename);

    -- Создаем временный CLOB
    DBMS_LOB.CREATETEMPORARY(v_clob, TRUE);

    -- Подключаем файл
    v_bfile := BFILENAME(p_directory, p_filename);
    DBMS_LOB.OPEN(v_bfile, DBMS_LOB.LOB_READONLY);
    v_amount := DBMS_LOB.GETLENGTH(v_bfile);

    IF v_amount > 0 THEN
        DBMS_LOB.LOADCLOBFROMFILE(
            dest_lob     => v_clob,
            src_bfile    => v_bfile,
            amount       => v_amount,
            dest_offset  => v_dest_off,
            src_offset   => v_src_off,
            bfile_csid   => DBMS_LOB.DEFAULT_CSID,
            lang_context => v_lang_ctx,
            warning      => v_warning
        );
    END IF;

    DBMS_LOB.CLOSE(v_bfile);

    -- Цикл по JSON
    FOR rec IN (
        SELECT 
            jt.user_id,
            jt.login,
            jt.email,
            jt.created_at
        FROM JSON_TABLE(
            v_clob,
            '$[*]'
            COLUMNS (
                user_id    NUMBER PATH '$.USER_ID',
                login      VARCHAR2(50) PATH '$.LOGIN',
                email      VARCHAR2(100) PATH '$.EMAIL',
                created_at VARCHAR2(50) PATH '$.CREATED_AT'
            )
        ) jt
    ) LOOP
        BEGIN
            -- Проверяем существование user_id
            SELECT COUNT(*) INTO v_count
            FROM USERS u
            WHERE u.user_id = rec.user_id;

            IF v_count = 0 THEN
                -- Вставка с корректной обработкой даты
                INSERT INTO USERS(user_id, login, email, created_at)
                VALUES(
                    rec.user_id,
                    rec.login,
                    rec.email,
                    -- Если created_at некорректен, используем SYSDATE
                    NVL(
                        TO_TIMESTAMP_TZ(rec.created_at DEFAULT NULL ON CONVERSION ERROR, 'YYYY-MM-DD"T"HH24:MI:SS'),
                        SYSTIMESTAMP
                    )
                );
                v_rows_inserted := v_rows_inserted + 1;
            --ELSE
                --DBMS_OUTPUT.PUT_LINE('Пропущен user_id=' || rec.user_id || ' (уже существует)');
            END IF;
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('Ошибка вставки user_id=' || rec.user_id || ': ' || SQLERRM);
        END;
    END LOOP;

    DBMS_OUTPUT.PUT_LINE('Количество вставленных записей: ' || v_rows_inserted);
    COMMIT;

    -- Очистка
    IF v_clob IS NOT NULL THEN
        DBMS_LOB.FREETEMPORARY(v_clob);
    END IF;

    DBMS_OUTPUT.PUT_LINE('Импорт пользователей завершен.');
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Общая ошибка: ' || SQLERRM);
        IF v_clob IS NOT NULL THEN DBMS_LOB.FREETEMPORARY(v_clob); END IF;
        ROLLBACK;
        RAISE;
END;
/
BEGIN
    export_users_to_json('DATA_PUMP_DIR', 'users.json');
END;
/
TRUNCATE TABLE USERS;
BEGIN
    import_users_from_json_safe('DATA_PUMP_DIR', 'users.json');
END;
/
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE export_studios_to_json (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_file  UTL_FILE.FILE_TYPE;
    v_first BOOLEAN := TRUE;
BEGIN
    v_file := UTL_FILE.FOPEN(p_directory, p_filename, 'w', 32767);
    UTL_FILE.PUT_LINE(v_file, '[');

    FOR r IN (
        SELECT studio_id, name, description, price_per_hour, square
        FROM   STUDIOS
        ORDER  BY studio_id
    ) LOOP
        IF v_first THEN
            v_first := FALSE;
        ELSE
            UTL_FILE.PUT_LINE(v_file, ',');
        END IF;

        UTL_FILE.PUT_LINE(
            v_file,
            JSON_OBJECT(
                'STUDIO_ID'     VALUE r.studio_id,
                'NAME'          VALUE r.name,
                'DESCRIPTION'   VALUE r.description,
                'PRICE_PER_HOUR' VALUE r.price_per_hour,
                'SQUARE'        VALUE r.square
            RETURNING VARCHAR2)
        );
    END LOOP;

    UTL_FILE.PUT_LINE(v_file, ']');
    UTL_FILE.FCLOSE(v_file);
END;
/
CREATE OR REPLACE PROCEDURE import_studios_from_json_safe (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_bfile     BFILE;
    v_clob      CLOB;
    v_amount    NUMBER;
    v_dest_off  NUMBER := 1;
    v_src_off   NUMBER := 1;
    v_lang_ctx  NUMBER := 0;
    v_warning   NUMBER;
    v_ins       NUMBER := 0;
BEGIN
    DBMS_LOB.CREATETEMPORARY(v_clob, TRUE);

    v_bfile := BFILENAME(p_directory, p_filename);
    DBMS_LOB.OPEN(v_bfile, DBMS_LOB.LOB_READONLY);
    v_amount := DBMS_LOB.GETLENGTH(v_bfile);

    IF v_amount > 0 THEN
        DBMS_LOB.LOADCLOBFROMFILE(
            dest_lob     => v_clob,
            src_bfile    => v_bfile,
            amount       => v_amount,
            dest_offset  => v_dest_off,
            src_offset   => v_src_off,
            bfile_csid   => DBMS_LOB.DEFAULT_CSID,
            lang_context => v_lang_ctx,
            warning      => v_warning
        );
    END IF;
    DBMS_LOB.CLOSE(v_bfile);

    FOR rec IN (
        SELECT
            jt.studio_id,
            jt.name,
            jt.description,
            jt.price_per_hour,
            jt.square
        FROM JSON_TABLE(
                v_clob,
                '$[*]'
                COLUMNS (
                    studio_id     NUMBER        PATH '$.STUDIO_ID',
                    name          VARCHAR2(100) PATH '$.NAME',
                    description   CLOB          PATH '$.DESCRIPTION',
                    price_per_hour NUMBER       PATH '$.PRICE_PER_HOUR',
                    square        NUMBER        PATH '$.SQUARE'
                )
             ) jt
    ) LOOP
        BEGIN
            INSERT INTO STUDIOS (studio_id, name, description, price_per_hour, square)
            VALUES (
                rec.studio_id,   -- если NULL, сработает IDENTITY
                rec.name,
                rec.description,
                rec.price_per_hour,
                rec.square
            );
            v_ins := v_ins + 1;
        EXCEPTION
            WHEN DUP_VAL_ON_INDEX THEN
                NULL;
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('Ошибка вставки STUDIO_ID=' || rec.studio_id || ': ' || SQLERRM);
        END;
    END LOOP;

    DBMS_OUTPUT.PUT_LINE('STUDIOS: вставлено ' || v_ins);
    COMMIT;
    IF v_clob IS NOT NULL THEN DBMS_LOB.FREETEMPORARY(v_clob); END IF;
END;
/
BEGIN
    export_studios_to_json('DATA_PUMP_DIR', 'studios.json');
END;
/
TRUNCATE TABLE STUDIOS;
BEGIN
    import_studios_from_json_safe('DATA_PUMP_DIR', 'studios.json');
END;
/
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE export_services_to_json (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_file  UTL_FILE.FILE_TYPE;
    v_first BOOLEAN := TRUE;
BEGIN
    v_file := UTL_FILE.FOPEN(p_directory, p_filename, 'w', 32767);
    UTL_FILE.PUT_LINE(v_file, '[');

    FOR r IN (
        SELECT service_id, name, price, description
        FROM   SERVICES
        ORDER  BY service_id
    ) LOOP
        IF v_first THEN v_first := FALSE; ELSE UTL_FILE.PUT_LINE(v_file, ','); END IF;

        UTL_FILE.PUT_LINE(
            v_file,
            JSON_OBJECT(
                'SERVICE_ID'  VALUE r.service_id,
                'NAME'        VALUE r.name,
                'PRICE'       VALUE r.price,
                'DESCRIPTION' VALUE r.description
            RETURNING VARCHAR2)
        );
    END LOOP;

    UTL_FILE.PUT_LINE(v_file, ']');
    UTL_FILE.FCLOSE(v_file);
END;
/
CREATE OR REPLACE PROCEDURE import_services_from_json_safe (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_bfile     BFILE;
    v_clob      CLOB;
    v_amount    NUMBER;
    v_dest_off  NUMBER := 1;
    v_src_off   NUMBER := 1;
    v_lang_ctx  NUMBER := 0;
    v_warning   NUMBER;
    v_ins       NUMBER := 0;
BEGIN
    DBMS_LOB.CREATETEMPORARY(v_clob, TRUE);
    v_bfile := BFILENAME(p_directory, p_filename);
    DBMS_LOB.OPEN(v_bfile, DBMS_LOB.LOB_READONLY);
    v_amount := DBMS_LOB.GETLENGTH(v_bfile);

    IF v_amount > 0 THEN
        DBMS_LOB.LOADCLOBFROMFILE(
            dest_lob     => v_clob,
            src_bfile    => v_bfile,
            amount       => v_amount,
            dest_offset  => v_dest_off,
            src_offset   => v_src_off,
            bfile_csid   => DBMS_LOB.DEFAULT_CSID,
            lang_context => v_lang_ctx,
            warning      => v_warning
        );
    END IF;
    DBMS_LOB.CLOSE(v_bfile);

    FOR rec IN (
        SELECT
            jt.service_id,
            jt.name,
            jt.price,
            jt.description
        FROM JSON_TABLE(
                v_clob,
                '$[*]'
                COLUMNS (
                    service_id  NUMBER        PATH '$.SERVICE_ID',
                    name        VARCHAR2(100) PATH '$.NAME',
                    price       NUMBER        PATH '$.PRICE',
                    description CLOB          PATH '$.DESCRIPTION'
                )
             ) jt
    ) LOOP
        BEGIN
            INSERT INTO SERVICES (service_id, name, price, description)
            VALUES (rec.service_id, rec.name, rec.price, rec.description);
            v_ins := v_ins + 1;
        EXCEPTION
            WHEN DUP_VAL_ON_INDEX THEN NULL;
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('Ошибка вставки SERVICE_ID=' || rec.service_id || ': ' || SQLERRM);
        END;
    END LOOP;

    DBMS_OUTPUT.PUT_LINE('SERVICES: вставлено ' || v_ins);
    COMMIT;
    IF v_clob IS NOT NULL THEN DBMS_LOB.FREETEMPORARY(v_clob); END IF;
END;
/
BEGIN
    export_services_to_json('DATA_PUMP_DIR', 'services.json');
END;
/
TRUNCATE TABLE SERVICES;
BEGIN
    import_services_from_json_safe('DATA_PUMP_DIR', 'services.json');
END;
/
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE export_admins_to_json (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_file  UTL_FILE.FILE_TYPE;
    v_first BOOLEAN := TRUE;
BEGIN
    v_file := UTL_FILE.FOPEN(p_directory, p_filename, 'w', 32767);
    UTL_FILE.PUT_LINE(v_file, '[');

    FOR r IN (
        SELECT admin_id, user_id, role_name
        FROM   ADMINS
        ORDER  BY admin_id
    ) LOOP
        IF v_first THEN v_first := FALSE; ELSE UTL_FILE.PUT_LINE(v_file, ','); END IF;

        UTL_FILE.PUT_LINE(
            v_file,
            JSON_OBJECT(
                'ADMIN_ID'  VALUE r.admin_id,
                'USER_ID'   VALUE r.user_id,
                'ROLE_NAME' VALUE r.role_name
            RETURNING VARCHAR2)
        );
    END LOOP;

    UTL_FILE.PUT_LINE(v_file, ']');
    UTL_FILE.FCLOSE(v_file);
END;
/
CREATE OR REPLACE PROCEDURE import_admins_from_json_safe (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_bfile     BFILE;
    v_clob      CLOB;
    v_amount    NUMBER;
    v_dest_off  NUMBER := 1;
    v_src_off   NUMBER := 1;
    v_lang_ctx  NUMBER := 0;
    v_warning   NUMBER;
    v_ins       NUMBER := 0;
BEGIN
    DBMS_LOB.CREATETEMPORARY(v_clob, TRUE);
    v_bfile := BFILENAME(p_directory, p_filename);
    DBMS_LOB.OPEN(v_bfile, DBMS_LOB.LOB_READONLY);
    v_amount := DBMS_LOB.GETLENGTH(v_bfile);

    IF v_amount > 0 THEN
        DBMS_LOB.LOADCLOBFROMFILE(
            dest_lob     => v_clob,
            src_bfile    => v_bfile,
            amount       => v_amount,
            dest_offset  => v_dest_off,
            src_offset   => v_src_off,
            bfile_csid   => DBMS_LOB.DEFAULT_CSID,
            lang_context => v_lang_ctx,
            warning      => v_warning
        );
    END IF;
    DBMS_LOB.CLOSE(v_bfile);

    FOR rec IN (
        SELECT
            jt.admin_id,
            jt.user_id,
            jt.role_name
        FROM JSON_TABLE(
                v_clob,
                '$[*]'
                COLUMNS (
                    admin_id  NUMBER        PATH '$.ADMIN_ID',
                    user_id   NUMBER        PATH '$.USER_ID',
                    role_name VARCHAR2(20)  PATH '$.ROLE_NAME'
                )
             ) jt
    ) LOOP
        BEGIN
            INSERT INTO ADMINS (admin_id, user_id, role_name)
            VALUES (rec.admin_id, rec.user_id, rec.role_name);
            v_ins := v_ins + 1;
        EXCEPTION
            WHEN DUP_VAL_ON_INDEX THEN NULL;
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('Ошибка вставки ADMIN_ID=' || rec.admin_id || ': ' || SQLERRM);
        END;
    END LOOP;

    DBMS_OUTPUT.PUT_LINE('ADMINS: вставлено ' || v_ins);
    COMMIT;
    IF v_clob IS NOT NULL THEN DBMS_LOB.FREETEMPORARY(v_clob); END IF;
END;
/
BEGIN
    export_admins_to_json('DATA_PUMP_DIR', 'admins.json');
END;
/
TRUNCATE TABLE ADMINS;
BEGIN
    import_admins_from_json_safe('DATA_PUMP_DIR', 'admins.json');
END;
/
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE export_studio_rent_orders_to_json (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_file  UTL_FILE.FILE_TYPE;
    v_first BOOLEAN := TRUE;
BEGIN
    v_file := UTL_FILE.FOPEN(p_directory, p_filename, 'w', 32767);
    UTL_FILE.PUT_LINE(v_file, '[');

    FOR r IN (
        SELECT order_id, user_id, studio_id, rent_date, start_time, end_time, total_price, status
        FROM STUDIO_RENT_ORDERS
        ORDER BY order_id
    ) LOOP
        IF v_first THEN v_first := FALSE; ELSE UTL_FILE.PUT_LINE(v_file, ','); END IF;

        UTL_FILE.PUT_LINE(
            v_file,
            JSON_OBJECT(
                'ORDER_ID'   VALUE r.order_id,
                'USER_ID'    VALUE r.user_id,
                'STUDIO_ID'  VALUE r.studio_id,
                'RENT_DATE'  VALUE TO_CHAR(r.rent_date,'YYYY-MM-DD'),
                'START_TIME' VALUE TO_CHAR(r.start_time,'YYYY-MM-DD"T"HH24:MI:SS'),
                'END_TIME'   VALUE TO_CHAR(r.end_time,'YYYY-MM-DD"T"HH24:MI:SS'),
                'TOTAL_PRICE' VALUE r.total_price,
                'STATUS'      VALUE r.status
            RETURNING VARCHAR2)
        );
    END LOOP;

    UTL_FILE.PUT_LINE(v_file, ']');
    UTL_FILE.FCLOSE(v_file);
END;
/
CREATE OR REPLACE PROCEDURE import_studio_rent_orders_from_json_safe (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_bfile BFILE;
    v_clob  CLOB;
    v_amount NUMBER;
    v_dest_off NUMBER := 1;
    v_src_off NUMBER := 1;
    v_lang_ctx NUMBER := 0;
    v_warning NUMBER;
    v_ins NUMBER := 0;
    v_count NUMBER;
BEGIN
    DBMS_LOB.CREATETEMPORARY(v_clob, TRUE);
    v_bfile := BFILENAME(p_directory, p_filename);
    DBMS_LOB.OPEN(v_bfile, DBMS_LOB.LOB_READONLY);
    v_amount := DBMS_LOB.GETLENGTH(v_bfile);
    IF v_amount > 0 THEN
        DBMS_LOB.LOADCLOBFROMFILE(
            dest_lob => v_clob,
            src_bfile => v_bfile,
            amount => v_amount,
            dest_offset => v_dest_off,
            src_offset => v_src_off,
            bfile_csid => DBMS_LOB.DEFAULT_CSID,
            lang_context => v_lang_ctx,
            warning => v_warning
        );
    END IF;
    DBMS_LOB.CLOSE(v_bfile);

    FOR rec IN (
        SELECT jt.order_id, jt.user_id, jt.studio_id, jt.rent_date,
               jt.start_time, jt.end_time, jt.total_price, jt.status
        FROM JSON_TABLE(
                v_clob,
                '$[*]'
                COLUMNS (
                    order_id    NUMBER PATH '$.ORDER_ID',
                    user_id     NUMBER PATH '$.USER_ID',
                    studio_id   NUMBER PATH '$.STUDIO_ID',
                    rent_date   VARCHAR2(20) PATH '$.RENT_DATE',
                    start_time  VARCHAR2(30) PATH '$.START_TIME',
                    end_time    VARCHAR2(30) PATH '$.END_TIME',
                    total_price NUMBER PATH '$.TOTAL_PRICE',
                    status      VARCHAR2(20) PATH '$.STATUS'
                )
             ) jt
    ) LOOP
        BEGIN
            SELECT COUNT(*) INTO v_count FROM STUDIO_RENT_ORDERS WHERE order_id = rec.order_id;
            IF v_count = 0 THEN
                INSERT INTO STUDIO_RENT_ORDERS(
                    order_id, user_id, studio_id, rent_date, start_time, end_time, total_price, status
                ) VALUES (
                    rec.order_id, rec.user_id, rec.studio_id,
                    TO_DATE(rec.rent_date,'YYYY-MM-DD'),
                    TO_TIMESTAMP_TZ(rec.start_time DEFAULT NULL ON CONVERSION ERROR,'YYYY-MM-DD"T"HH24:MI:SS'),
                    TO_TIMESTAMP_TZ(rec.end_time DEFAULT NULL ON CONVERSION ERROR,'YYYY-MM-DD"T"HH24:MI:SS'),
                    rec.total_price,
                    rec.status
                );
                v_ins := v_ins + 1;
            END IF;
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('Ошибка вставки ORDER_ID=' || rec.order_id || ': ' || SQLERRM);
        END;
    END LOOP;

    DBMS_OUTPUT.PUT_LINE('STUDIO_RENT_ORDERS: вставлено ' || v_ins);
    COMMIT;
    IF v_clob IS NOT NULL THEN DBMS_LOB.FREETEMPORARY(v_clob); END IF;
END;
/
BEGIN
    export_studio_rent_orders_to_json('DATA_PUMP_DIR', 'studio_rent_orders.json');
END;
/
TRUNCATE TABLE STUDIO_RENT_ORDERS;
BEGIN
    import_studio_rent_orders_from_json_safe('DATA_PUMP_DIR', 'studio_rent_orders.json');
END;
/
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE export_archived_studio_rent_orders_to_json (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_file  UTL_FILE.FILE_TYPE;
    v_first BOOLEAN := TRUE;
BEGIN
    v_file := UTL_FILE.FOPEN(p_directory, p_filename, 'w', 32767);
    UTL_FILE.PUT_LINE(v_file, '[');

    FOR r IN (
        SELECT order_id, user_id, studio_id, rent_date, start_time, end_time, total_price, archived_at
        FROM ARCHIVED_STUDIO_RENT_ORDERS
        ORDER BY order_id
    ) LOOP
        IF v_first THEN v_first := FALSE; ELSE UTL_FILE.PUT_LINE(v_file, ','); END IF;

        UTL_FILE.PUT_LINE(
            v_file,
            JSON_OBJECT(
                'ORDER_ID'   VALUE r.order_id,
                'USER_ID'    VALUE r.user_id,
                'STUDIO_ID'  VALUE r.studio_id,
                'RENT_DATE'  VALUE TO_CHAR(r.rent_date,'YYYY-MM-DD'),
                'START_TIME' VALUE TO_CHAR(r.start_time,'YYYY-MM-DD"T"HH24:MI:SS'),
                'END_TIME'   VALUE TO_CHAR(r.end_time,'YYYY-MM-DD"T"HH24:MI:SS'),
                'TOTAL_PRICE' VALUE r.total_price,
                'ARCHIVED_AT' VALUE TO_CHAR(r.archived_at,'YYYY-MM-DD"T"HH24:MI:SS')
            RETURNING VARCHAR2)
        );
    END LOOP;

    UTL_FILE.PUT_LINE(v_file, ']');
    UTL_FILE.FCLOSE(v_file);
END;
/
CREATE OR REPLACE PROCEDURE import_archived_studio_rent_orders_from_json_safe (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_bfile BFILE;
    v_clob  CLOB;
    v_amount NUMBER;
    v_dest_off NUMBER := 1;
    v_src_off NUMBER := 1;
    v_lang_ctx NUMBER := 0;
    v_warning NUMBER;
    v_ins NUMBER := 0;
    v_count NUMBER;
BEGIN
    DBMS_LOB.CREATETEMPORARY(v_clob, TRUE);
    v_bfile := BFILENAME(p_directory, p_filename);
    DBMS_LOB.OPEN(v_bfile, DBMS_LOB.LOB_READONLY);
    v_amount := DBMS_LOB.GETLENGTH(v_bfile);
    IF v_amount > 0 THEN
        DBMS_LOB.LOADCLOBFROMFILE(
            dest_lob => v_clob,
            src_bfile => v_bfile,
            amount => v_amount,
            dest_offset => v_dest_off,
            src_offset => v_src_off,
            bfile_csid => DBMS_LOB.DEFAULT_CSID,
            lang_context => v_lang_ctx,
            warning => v_warning
        );
    END IF;
    DBMS_LOB.CLOSE(v_bfile);

    FOR rec IN (
        SELECT jt.order_id, jt.user_id, jt.studio_id, jt.rent_date,
               jt.start_time, jt.end_time, jt.total_price, jt.archived_at
        FROM JSON_TABLE(
                v_clob,
                '$[*]'
                COLUMNS (
                    order_id    NUMBER PATH '$.ORDER_ID',
                    user_id     NUMBER PATH '$.USER_ID',
                    studio_id   NUMBER PATH '$.STUDIO_ID',
                    rent_date   VARCHAR2(20) PATH '$.RENT_DATE',
                    start_time  VARCHAR2(30) PATH '$.START_TIME',
                    end_time    VARCHAR2(30) PATH '$.END_TIME',
                    total_price NUMBER PATH '$.TOTAL_PRICE',
                    archived_at VARCHAR2(30) PATH '$.ARCHIVED_AT'
                )
             ) jt
    ) LOOP
        BEGIN
            SELECT COUNT(*) INTO v_count FROM ARCHIVED_STUDIO_RENT_ORDERS WHERE order_id = rec.order_id;
            IF v_count = 0 THEN
                INSERT INTO ARCHIVED_STUDIO_RENT_ORDERS(
                    order_id, user_id, studio_id, rent_date, start_time, end_time, total_price, archived_at
                ) VALUES (
                    rec.order_id, rec.user_id, rec.studio_id,
                    TO_DATE(rec.rent_date,'YYYY-MM-DD'),
                    TO_TIMESTAMP_TZ(rec.start_time DEFAULT NULL ON CONVERSION ERROR,'YYYY-MM-DD"T"HH24:MI:SS'),
                    TO_TIMESTAMP_TZ(rec.end_time DEFAULT NULL ON CONVERSION ERROR,'YYYY-MM-DD"T"HH24:MI:SS'),
                    rec.total_price,
                    TO_TIMESTAMP_TZ(rec.archived_at DEFAULT NULL ON CONVERSION ERROR,'YYYY-MM-DD"T"HH24:MI:SS')
                );
                v_ins := v_ins + 1;
            END IF;
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('Ошибка вставки ORDER_ID=' || rec.order_id || ': ' || SQLERRM);
        END;
    END LOOP;

    DBMS_OUTPUT.PUT_LINE('ARCHIVED_STUDIO_RENT_ORDERS: вставлено ' || v_ins);
    COMMIT;
    IF v_clob IS NOT NULL THEN DBMS_LOB.FREETEMPORARY(v_clob); END IF;
END;
/
BEGIN
    export_archived_studio_rent_orders_to_json('DATA_PUMP_DIR', 'archived_studio_rent_orders.json');
END;
/
TRUNCATE TABLE ARCHIVED_STUDIO_RENT_ORDERS;
BEGIN
    import_archived_studio_rent_orders_from_json_safe('DATA_PUMP_DIR', 'archived_studio_rent_orders.json');
END;
/
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE export_order_services_to_json (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_file  UTL_FILE.FILE_TYPE;
    v_first BOOLEAN := TRUE;
BEGIN
    v_file := UTL_FILE.FOPEN(p_directory, p_filename, 'w', 32767);
    UTL_FILE.PUT_LINE(v_file, '[');

    FOR r IN (
        SELECT order_id, service_id
        FROM ORDER_SERVICES
        ORDER BY order_id, service_id
    ) LOOP
        IF v_first THEN v_first := FALSE; ELSE UTL_FILE.PUT_LINE(v_file, ','); END IF;

        UTL_FILE.PUT_LINE(
            v_file,
            JSON_OBJECT(
                'ORDER_ID'   VALUE r.order_id,
                'SERVICE_ID' VALUE r.service_id
            RETURNING VARCHAR2)
        );
    END LOOP;

    UTL_FILE.PUT_LINE(v_file, ']');
    UTL_FILE.FCLOSE(v_file);
END;
/
CREATE OR REPLACE PROCEDURE import_order_services_from_json_safe (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_bfile BFILE;
    v_clob  CLOB;
    v_amount NUMBER;
    v_dest_off NUMBER := 1;
    v_src_off NUMBER := 1;
    v_lang_ctx NUMBER := 0;
    v_warning NUMBER;
    v_ins NUMBER := 0;
BEGIN
    DBMS_LOB.CREATETEMPORARY(v_clob, TRUE);
    v_bfile := BFILENAME(p_directory, p_filename);
    DBMS_LOB.OPEN(v_bfile, DBMS_LOB.LOB_READONLY);
    v_amount := DBMS_LOB.GETLENGTH(v_bfile);
    IF v_amount > 0 THEN
        DBMS_LOB.LOADCLOBFROMFILE(
            dest_lob => v_clob,
            src_bfile => v_bfile,
            amount => v_amount,
            dest_offset => v_dest_off,
            src_offset => v_src_off,
            bfile_csid => DBMS_LOB.DEFAULT_CSID,
            lang_context => v_lang_ctx,
            warning => v_warning
        );
    END IF;
    DBMS_LOB.CLOSE(v_bfile);

    FOR rec IN (
        SELECT jt.order_id, jt.service_id
        FROM JSON_TABLE(
                v_clob,
                '$[*]'
                COLUMNS (
                    order_id    NUMBER PATH '$.ORDER_ID',
                    service_id  NUMBER PATH '$.SERVICE_ID'
                )
             ) jt
    ) LOOP
        BEGIN
            INSERT INTO ORDER_SERVICES(order_id, service_id)
            VALUES (rec.order_id, rec.service_id);
            v_ins := v_ins + 1;
        EXCEPTION
            WHEN DUP_VAL_ON_INDEX THEN NULL;
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('Ошибка вставки ORDER_ID=' || rec.order_id || ' SERVICE_ID=' || rec.service_id || ': ' || SQLERRM);
        END;
    END LOOP;

    DBMS_OUTPUT.PUT_LINE('ORDER_SERVICES: вставлено ' || v_ins);
    COMMIT;
    IF v_clob IS NOT NULL THEN DBMS_LOB.FREETEMPORARY(v_clob); END IF;
END;
/
BEGIN
    export_order_services_to_json('DATA_PUMP_DIR', 'order_services.json');
END;
/
TRUNCATE TABLE ORDER_SERVICES;
BEGIN
    import_order_services_from_json_safe('DATA_PUMP_DIR', 'order_services.json');
END;
/
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE export_archived_order_services_to_json (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_file  UTL_FILE.FILE_TYPE;
    v_first BOOLEAN := TRUE;
BEGIN
    v_file := UTL_FILE.FOPEN(p_directory, p_filename, 'w', 32767);
    UTL_FILE.PUT_LINE(v_file, '[');

    FOR r IN (
        SELECT order_id, service_id
        FROM ARCHIVED_ORDER_SERVICES
        ORDER BY order_id, service_id
    ) LOOP
        IF v_first THEN v_first := FALSE; ELSE UTL_FILE.PUT_LINE(v_file, ','); END IF;

        UTL_FILE.PUT_LINE(
            v_file,
            JSON_OBJECT(
                'ORDER_ID'   VALUE r.order_id,
                'SERVICE_ID' VALUE r.service_id
            RETURNING VARCHAR2)
        );
    END LOOP;

    UTL_FILE.PUT_LINE(v_file, ']');
    UTL_FILE.FCLOSE(v_file);
END;
/
CREATE OR REPLACE PROCEDURE import_archived_order_services_from_json_safe (
    p_directory IN VARCHAR2,
    p_filename  IN VARCHAR2
) IS
    v_bfile BFILE;
    v_clob  CLOB;
    v_amount NUMBER;
    v_dest_off NUMBER := 1;
    v_src_off NUMBER := 1;
    v_lang_ctx NUMBER := 0;
    v_warning NUMBER;
    v_ins NUMBER := 0;
BEGIN
    DBMS_LOB.CREATETEMPORARY(v_clob, TRUE);
    v_bfile := BFILENAME(p_directory, p_filename);
    DBMS_LOB.OPEN(v_bfile, DBMS_LOB.LOB_READONLY);
    v_amount := DBMS_LOB.GETLENGTH(v_bfile);
    IF v_amount > 0 THEN
        DBMS_LOB.LOADCLOBFROMFILE(
            dest_lob => v_clob,
            src_bfile => v_bfile,
            amount => v_amount,
            dest_offset => v_dest_off,
            src_offset => v_src_off,
            bfile_csid => DBMS_LOB.DEFAULT_CSID,
            lang_context => v_lang_ctx,
            warning => v_warning
        );
    END IF;
    DBMS_LOB.CLOSE(v_bfile);

    FOR rec IN (
        SELECT jt.order_id, jt.service_id
        FROM JSON_TABLE(
                v_clob,
                '$[*]'
                COLUMNS (
                    order_id    NUMBER PATH '$.ORDER_ID',
                    service_id  NUMBER PATH '$.SERVICE_ID'
                )
             ) jt
    ) LOOP
        BEGIN
            INSERT INTO ARCHIVED_ORDER_SERVICES(order_id, service_id)
            VALUES (rec.order_id, rec.service_id);
            v_ins := v_ins + 1;
        EXCEPTION
            WHEN DUP_VAL_ON_INDEX THEN NULL;
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('Ошибка вставки ORDER_ID=' || rec.order_id || ' SERVICE_ID=' || rec.service_id || ': ' || SQLERRM);
        END;
    END LOOP;

    DBMS_OUTPUT.PUT_LINE('ARCHIVED_ORDER_SERVICES: вставлено ' || v_ins);
    COMMIT;
    IF v_clob IS NOT NULL THEN DBMS_LOB.FREETEMPORARY(v_clob); END IF;
END;
/
BEGIN
    export_archived_order_services_to_json('DATA_PUMP_DIR', 'archived_order_services.json');
END;
/
TRUNCATE TABLE ARCHIVED_ORDER_SERVICES;
BEGIN
    import_archived_order_services_from_json_safe('DATA_PUMP_DIR', 'archived_order_services.json');
END;
/
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE export_all_to_json(p_directory IN VARCHAR2) IS
BEGIN
    -- USERS
    export_users_to_json(p_directory, 'users.json');
    DBMS_OUTPUT.PUT_LINE('Экспорт USERS завершен.');

    -- ADMINS
    export_admins_to_json(p_directory, 'admins.json');
    DBMS_OUTPUT.PUT_LINE('Экспорт ADMINS завершен.');

    -- STUDIOS
    export_studios_to_json(p_directory, 'studios.json');
    DBMS_OUTPUT.PUT_LINE('Экспорт STUDIOS завершен.');

    -- SERVICES
    export_services_to_json(p_directory, 'services.json');
    DBMS_OUTPUT.PUT_LINE('Экспорт SERVICES завершен.');

    -- STUDIO_RENT_ORDERS
    export_studio_rent_orders_to_json(p_directory, 'studio_rent_orders.json');
    DBMS_OUTPUT.PUT_LINE('Экспорт STUDIO_RENT_ORDERS завершен.');

    -- ARCHIVED_STUDIO_RENT_ORDERS
    export_archived_studio_rent_orders_to_json(p_directory, 'archived_studio_rent_orders.json');
    DBMS_OUTPUT.PUT_LINE('Экспорт ARCHIVED_STUDIO_RENT_ORDERS завершен.');

    -- ORDER_SERVICES
    export_order_services_to_json(p_directory, 'order_services.json');
    DBMS_OUTPUT.PUT_LINE('Экспорт ORDER_SERVICES завершен.');

    -- ARCHIVED_ORDER_SERVICES
    export_archived_order_services_to_json(p_directory, 'archived_order_services.json');
    DBMS_OUTPUT.PUT_LINE('Экспорт ARCHIVED_ORDER_SERVICES завершен.');
END;
/
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE import_all_from_json(p_directory IN VARCHAR2) IS
BEGIN
    -- USERS
    EXECUTE IMMEDIATE 'TRUNCATE TABLE USERS';
    import_users_from_json_safe(p_directory, 'users.json');
    DBMS_OUTPUT.PUT_LINE('Импорт USERS завершен.');

    -- ADMINS
    EXECUTE IMMEDIATE 'TRUNCATE TABLE ADMINS';
    import_admins_from_json_safe(p_directory, 'admins.json');
    DBMS_OUTPUT.PUT_LINE('Импорт ADMINS завершен.');

    -- STUDIOS
    EXECUTE IMMEDIATE 'TRUNCATE TABLE STUDIOS';
    import_studios_from_json_safe(p_directory, 'studios.json');
    DBMS_OUTPUT.PUT_LINE('Импорт STUDIOS завершен.');

    -- SERVICES
    EXECUTE IMMEDIATE 'TRUNCATE TABLE SERVICES';
    import_services_from_json_safe(p_directory, 'services.json');
    DBMS_OUTPUT.PUT_LINE('Импорт SERVICES завершен.');

    -- STUDIO_RENT_ORDERS
    EXECUTE IMMEDIATE 'TRUNCATE TABLE STUDIO_RENT_ORDERS';
    import_studio_rent_orders_from_json_safe(p_directory, 'studio_rent_orders.json');
    DBMS_OUTPUT.PUT_LINE('Импорт STUDIO_RENT_ORDERS завершен.');

    -- ARCHIVED_STUDIO_RENT_ORDERS
    EXECUTE IMMEDIATE 'TRUNCATE TABLE ARCHIVED_STUDIO_RENT_ORDERS';
    import_archived_studio_rent_orders_from_json_safe(p_directory, 'archived_studio_rent_orders.json');
    DBMS_OUTPUT.PUT_LINE('Импорт ARCHIVED_STUDIO_RENT_ORDERS завершен.');

    -- ORDER_SERVICES
    EXECUTE IMMEDIATE 'TRUNCATE TABLE ORDER_SERVICES';
    import_order_services_from_json_safe(p_directory, 'order_services.json');
    DBMS_OUTPUT.PUT_LINE('Импорт ORDER_SERVICES завершен.');

    -- ARCHIVED_ORDER_SERVICES
    EXECUTE IMMEDIATE 'TRUNCATE TABLE ARCHIVED_ORDER_SERVICES';
    import_archived_order_services_from_json_safe(p_directory, 'archived_order_services.json');
    DBMS_OUTPUT.PUT_LINE('Импорт ARCHIVED_ORDER_SERVICES завершен.');
END;
/
--------------------------------------------------------------------------------
-- Экспорт всей базы сразу
BEGIN
    export_all_to_json('DATA_PUMP_DIR');
END;
/

-- Импорт всей базы сразу
BEGIN
    import_all_from_json('DATA_PUMP_DIR');
END;
/
--------------------------------------------------------------------------------
SET SERVEROUTPUT ON SIZE UNLIMITED;
DECLARE
    v_bfile     BFILE;
    v_clob      CLOB;
    v_amount    NUMBER;
    v_dest_off  NUMBER := 1;
    v_src_off   NUMBER := 1;
    v_lang_ctx  NUMBER := 0;
    v_warning   NUMBER;
    v_cnt       NUMBER;
BEGIN
    DBMS_LOB.CREATETEMPORARY(v_clob, TRUE);

    v_bfile := BFILENAME('DATA_PUMP_DIR', 'users.json');
    DBMS_LOB.OPEN(v_bfile, DBMS_LOB.LOB_READONLY);
    v_amount := DBMS_LOB.GETLENGTH(v_bfile);

    IF v_amount > 0 THEN
        DBMS_LOB.LOADCLOBFROMFILE(
            dest_lob     => v_clob,
            src_bfile    => v_bfile,
            amount       => v_amount,
            dest_offset  => v_dest_off,
            src_offset   => v_src_off,
            bfile_csid   => DBMS_LOB.DEFAULT_CSID,
            lang_context => v_lang_ctx,
            warning      => v_warning
        );
    END IF;

    DBMS_LOB.CLOSE(v_bfile);

    SELECT COUNT(*)
    INTO   v_cnt
    FROM   JSON_TABLE(
               v_clob,
               '$[*]'
               COLUMNS (user_id NUMBER PATH '$.USER_ID')
           );

    DBMS_OUTPUT.PUT_LINE('rows in JSON: ' || v_cnt);

    DBMS_LOB.FREETEMPORARY(v_clob);
END;
/