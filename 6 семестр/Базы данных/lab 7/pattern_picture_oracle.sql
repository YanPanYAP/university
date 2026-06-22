-- Oracle SQL
-- Поиск паттерна как на картинке:
-- рост -> падение -> второй рост больше первого и третьего -> падение -> небольшой рост -> падение

WITH prices AS (
    SELECT 1 AS item_id, 1 AS point_no, 10 AS value FROM dual UNION ALL
    SELECT 1, 2, 28 FROM dual UNION ALL
    SELECT 1, 3, 20 FROM dual UNION ALL
    SELECT 1, 4, 36 FROM dual UNION ALL
    SELECT 1, 5, 52 FROM dual UNION ALL
    SELECT 1, 6, 34 FROM dual UNION ALL
    SELECT 1, 7, 28 FROM dual UNION ALL
    SELECT 1, 8, 33 FROM dual UNION ALL
    SELECT 1, 9, 22 FROM dual UNION ALL
    SELECT 1, 10, 12 FROM dual UNION ALL
    SELECT 2, 1, 40 FROM dual UNION ALL
    SELECT 2, 2, 38 FROM dual UNION ALL
    SELECT 2, 3, 39 FROM dual UNION ALL
    SELECT 2, 4, 37 FROM dual
)
SELECT
    item_id,
    start_point,
    first_top_point,
    first_bottom_point,
    main_top_point,
    second_bottom_point,
    small_top_point,
    end_point,
    start_value,
    first_top_value,
    first_bottom_value,
    main_top_value,
    second_bottom_value,
    small_top_value,
    end_value
FROM prices
MATCH_RECOGNIZE (
    PARTITION BY item_id
    ORDER BY point_no
    MEASURES
        START_POINT.point_no AS start_point,
        FIRST_TOP.point_no AS first_top_point,
        FIRST_BOTTOM.point_no AS first_bottom_point,
        MAIN_TOP.point_no AS main_top_point,
        SECOND_BOTTOM.point_no AS second_bottom_point,
        SMALL_TOP.point_no AS small_top_point,
        END_POINT.point_no AS end_point,

        START_POINT.value AS start_value,
        FIRST_TOP.value AS first_top_value,
        FIRST_BOTTOM.value AS first_bottom_value,
        MAIN_TOP.value AS main_top_value,
        SECOND_BOTTOM.value AS second_bottom_value,
        SMALL_TOP.value AS small_top_value,
        END_POINT.value AS end_value
    ONE ROW PER MATCH
    AFTER MATCH SKIP TO NEXT ROW
    PATTERN (
        START_POINT
        FIRST_TOP
        FIRST_BOTTOM
        MAIN_RISE*
        MAIN_TOP
        MAIN_FALL*
        SECOND_BOTTOM
        SMALL_TOP
        FINAL_FALL*
        END_POINT
    )
    DEFINE
        FIRST_TOP AS FIRST_TOP.value > PREV(FIRST_TOP.value),
        FIRST_BOTTOM AS FIRST_BOTTOM.value < PREV(FIRST_BOTTOM.value),

        MAIN_RISE AS MAIN_RISE.value > PREV(MAIN_RISE.value),
        MAIN_TOP AS MAIN_TOP.value > PREV(MAIN_TOP.value),

        MAIN_FALL AS MAIN_FALL.value < PREV(MAIN_FALL.value),
        SECOND_BOTTOM AS SECOND_BOTTOM.value < PREV(SECOND_BOTTOM.value),

        SMALL_TOP AS SMALL_TOP.value > PREV(SMALL_TOP.value)
                   AND SMALL_TOP.value < MAIN_TOP.value
                   AND MAIN_TOP.value - FIRST_BOTTOM.value > FIRST_TOP.value - START_POINT.value
                   AND MAIN_TOP.value - FIRST_BOTTOM.value > SMALL_TOP.value - SECOND_BOTTOM.value,

        FINAL_FALL AS FINAL_FALL.value < PREV(FINAL_FALL.value),
        END_POINT AS END_POINT.value < PREV(END_POINT.value)
)
ORDER BY item_id, start_point;
