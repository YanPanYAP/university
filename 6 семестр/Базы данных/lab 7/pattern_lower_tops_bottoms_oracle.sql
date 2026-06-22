-- Oracle SQL
-- Поиск нисходящего зигзага:
-- каждая следующая вершина ниже предыдущей
-- и каждый следующий низ ниже предыдущего.

WITH prices AS (
    -- Здесь демо-данные. Если есть своя таблица, замени этот WITH на SELECT из нее.
    SELECT 1 AS item_id, 1 AS point_no, 90 AS value FROM dual UNION ALL
    SELECT 1, 2, 70 FROM dual UNION ALL
    SELECT 1, 3, 84 FROM dual UNION ALL
    SELECT 1, 4, 55 FROM dual UNION ALL
    SELECT 1, 5, 76 FROM dual UNION ALL
    SELECT 1, 6, 35 FROM dual UNION ALL
    SELECT 1, 7, 65 FROM dual UNION ALL
    SELECT 1, 8, 20 FROM dual UNION ALL

    -- Лишние точки для примера, они не подходят под паттерн.
    SELECT 2, 1, 30 FROM dual UNION ALL
    SELECT 2, 2, 50 FROM dual UNION ALL
    SELECT 2, 3, 45 FROM dual UNION ALL
    SELECT 2, 4, 60 FROM dual
)
SELECT
    item_id,
    top1_point,
    bottom1_point,
    top2_point,
    bottom2_point,
    top3_point,
    bottom3_point,
    top1_value,
    bottom1_value,
    top2_value,
    bottom2_value,
    top3_value,
    bottom3_value
FROM prices
MATCH_RECOGNIZE (
    PARTITION BY item_id
    ORDER BY point_no
    MEASURES
        TOP1.point_no AS top1_point,
        BOTTOM1.point_no AS bottom1_point,
        TOP2.point_no AS top2_point,
        BOTTOM2.point_no AS bottom2_point,
        TOP3.point_no AS top3_point,
        BOTTOM3.point_no AS bottom3_point,

        TOP1.value AS top1_value,
        BOTTOM1.value AS bottom1_value,
        TOP2.value AS top2_value,
        BOTTOM2.value AS bottom2_value,
        TOP3.value AS top3_value,
        BOTTOM3.value AS bottom3_value
    ONE ROW PER MATCH
    AFTER MATCH SKIP TO NEXT ROW
    PATTERN (
        TOP1
        BOTTOM1
        TOP2
        BOTTOM2
        TOP3
        BOTTOM3
    )
    DEFINE
        BOTTOM1 AS BOTTOM1.value < TOP1.value,

        TOP2 AS TOP2.value > BOTTOM1.value
                AND TOP2.value < TOP1.value,
        BOTTOM2 AS BOTTOM2.value < TOP2.value
                  AND BOTTOM2.value < BOTTOM1.value,

        TOP3 AS TOP3.value > BOTTOM2.value
                AND TOP3.value < TOP2.value,
        BOTTOM3 AS BOTTOM3.value < TOP3.value
                  AND BOTTOM3.value < BOTTOM2.value
)
ORDER BY item_id, top1_point;
