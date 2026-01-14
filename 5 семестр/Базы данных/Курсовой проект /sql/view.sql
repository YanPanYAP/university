ALTER SESSION SET CONTAINER=CRS_PDB;
ALTER SESSION SET CURRENT_SCHEMA = CRS_CUR;

-- Все бронирования (активные + архив)
CREATE OR REPLACE VIEW V_ALL_BOOKINGS AS
SELECT 
    b.order_id, 
    b.user_id, 
    b.studio_id, 
    s.name AS studio_name,
    b.rent_date, 
    b.start_time, 
    b.end_time, 
    b.total_price, 
    b.status
FROM STUDIO_RENT_ORDERS b
JOIN STUDIOS s ON b.studio_id = s.studio_id
UNION ALL
SELECT 
    b.order_id, 
    b.user_id, 
    b.studio_id, 
    s.name AS studio_name,
    b.rent_date, 
    b.start_time, 
    b.end_time, 
    b.total_price, 
    'APPROVED' AS status
FROM ARCHIVED_STUDIO_RENT_ORDERS b
JOIN STUDIOS s ON b.studio_id = s.studio_id;

-- Количество бронирований по дням
CREATE OR REPLACE VIEW V_BOOKINGS_BY_DAY AS
SELECT TRUNC(rent_date) AS day, COUNT(*) AS bookings_count
FROM V_ALL_BOOKINGS
GROUP BY TRUNC(rent_date)
ORDER BY day;

-- Количество бронирований по студиям
CREATE OR REPLACE VIEW V_STUDIO_BOOKINGS_COUNT AS
SELECT 
    studio_id, 
    studio_name,
    COUNT(*) AS bookings_count
FROM V_ALL_BOOKINGS
WHERE status = 'APPROVED'
GROUP BY studio_id, studio_name
ORDER BY bookings_count DESC;

-- Количество бронирований по услугам с названием услуги
CREATE OR REPLACE VIEW V_SERVICE_BOOKINGS_COUNT AS
SELECT 
    os.service_id, 
    srv.name AS service_name,
    COUNT(*) AS bookings_count
FROM ORDER_SERVICES os
JOIN SERVICES srv ON os.service_id = srv.service_id
JOIN V_ALL_BOOKINGS b ON os.order_id = b.order_id
WHERE b.status = 'APPROVED'
GROUP BY os.service_id, srv.name
ORDER BY bookings_count DESC;

-- Суммарная стоимость бронирований
CREATE OR REPLACE VIEW V_TOTAL_BOOKINGS AS
SELECT COUNT(*) AS total_bookings,
       SUM(total_price) AS total_revenue
FROM V_ALL_BOOKINGS
WHERE status = 'APPROVED';

--------------------------------
CREATE OR REPLACE VIEW CRS_V_ANALYTICS AS
WITH ALL_BOOKINGS AS (
    SELECT 
        b.order_id, 
        b.user_id, 
        b.studio_id, 
        s.name AS studio_name,
        b.rent_date, 
        b.start_time, 
        b.end_time, 
        b.total_price, 
        b.status
    FROM STUDIO_RENT_ORDERS b
    LEFT JOIN STUDIOS s ON b.studio_id = s.studio_id
    UNION ALL
    SELECT 
        b.order_id, 
        b.user_id, 
        b.studio_id, 
        s.name AS studio_name,
        b.rent_date, 
        b.start_time, 
        b.end_time, 
        b.total_price, 
        b.status
    FROM ARCHIVED_STUDIO_RENT_ORDERS b
    LEFT JOIN STUDIOS s ON b.studio_id = s.studio_id
),
BOOKINGS_BY_DAY AS (
    SELECT 
        TRUNC(rent_date) AS day, 
        COUNT(*) AS bookings_count
    FROM ALL_BOOKINGS
    GROUP BY TRUNC(rent_date)
),
STUDIO_BOOKINGS AS (
    SELECT 
        studio_id, 
        studio_name, 
        COUNT(*) AS bookings_count
    FROM ALL_BOOKINGS
    WHERE status = 'APPROVED'
    GROUP BY studio_id, studio_name
),
SERVICE_BOOKINGS AS (
    SELECT 
        os.service_id,
        srv.name AS service_name,
        COUNT(*) AS bookings_count
    FROM ORDER_SERVICES os
    LEFT JOIN SERVICES srv ON os.service_id = srv.service_id
    LEFT JOIN ALL_BOOKINGS b ON os.order_id = b.order_id
    WHERE b.status = 'APPROVED'
    GROUP BY os.service_id, srv.name
),
TOTAL_BOOKINGS AS (
    SELECT 
        COUNT(*) AS total_bookings,
        SUM(total_price) AS total_revenue
    FROM ALL_BOOKINGS
    WHERE status = 'APPROVED'
)
SELECT 
    ab.order_id,
    ab.user_id,
    ab.studio_id,
    ab.studio_name,
    ab.rent_date,
    ab.start_time,
    ab.end_time,
    ab.total_price,
    ab.status,
    bbd.day,
    bbd.bookings_count AS bookings_by_day,
    sb.studio_name AS studio_name_count,
    sb.bookings_count AS studio_bookings_count,
    srb.service_name,
    srb.bookings_count AS service_bookings_count,
    tb.total_bookings,
    tb.total_revenue
FROM ALL_BOOKINGS ab
LEFT JOIN BOOKINGS_BY_DAY bbd ON TRUNC(ab.rent_date) = bbd.day
LEFT JOIN STUDIO_BOOKINGS sb ON ab.studio_id = sb.studio_id
LEFT JOIN SERVICE_BOOKINGS srb ON ab.order_id IN (
    SELECT order_id FROM ORDER_SERVICES WHERE service_id = srb.service_id
)
CROSS JOIN TOTAL_BOOKINGS tb;

BEGIN
    CRS_ANALYTICS.GET_BOOKINGS_BY_PERIOD(
        p_current_user => 1,
        p_start_date   => DATE '2025-01-01',
        p_end_date     => DATE '2025-12-31'
    );
END;
/

BEGIN
    CRS_ANALYTICS.GET_POPULAR_STUDIOS(
        p_current_user => 1,
        p_limit        => 5
    );
END;
/

BEGIN
    CRS_ANALYTICS.GET_POPULAR_SERVICES(
        p_current_user => 1,
        p_limit        => 5
    );
END;
/

BEGIN
    CRS_ANALYTICS.GET_TOTAL_BOOKINGS(p_current_user => 1);
END;
/


