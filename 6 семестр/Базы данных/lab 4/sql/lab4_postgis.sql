CREATE EXTENSION IF NOT EXISTS postgis;

-- 6-8
SELECT
    f_table_schema,
    f_table_name,
    f_geometry_column,
    type AS geometry_type,
    srid
FROM public.geometry_columns
WHERE f_table_schema = 'public'
ORDER BY f_table_schema, f_table_name;

SELECT
    column_name,
    data_type
FROM information_schema.columns
WHERE table_schema = 'public'
  AND table_name = 'countries'
  AND column_name <> 'geom'
ORDER BY ordinal_position;

-- 9
SELECT
    admin,
    LEFT(ST_AsText(geom), 500) AS wkt_fragment
FROM public.countries
LIMIT 3;

-- 10.1
SELECT
    a.admin AS country_a,
    b.admin AS country_b,
    ST_AsText(ST_Intersection(a.geom, b.geom)) AS intersection_wkt
FROM public.countries a
JOIN public.countries b
  ON a.gid <> b.gid
WHERE a.admin = 'Poland'
  AND b.admin = 'Germany';

  SELECT
    a.admin AS country_a,
    b.admin AS country_b,
    ST_Intersects(a.geom, b.geom) AS intersects
FROM public.countries a
JOIN public.countries b
  ON a.gid <> b.gid
WHERE a.admin = 'Poland'
  AND b.admin = 'Germany';


-- 10.2
SELECT
    c.admin,
    (dp).path[1] AS polygon_no,
    (dp).path[2] AS ring_no,
    (dp).path[3] AS point_no,
    ST_X((dp).geom) AS x,
    ST_Y((dp).geom) AS y
FROM (
    SELECT admin, ST_DumpPoints(geom) AS dp
    FROM public.countries
    WHERE admin = 'Belarus'
) c
LIMIT 20;

-- 10.3
SELECT
    admin,
    ROUND(
        CAST(
            ST_Area(geom::geography) / 1000000 AS numeric
        ),
        2
    ) AS area_sq_km
FROM public.countries
WHERE admin IN ('Belarus', 'Poland', 'Lithuania')
ORDER BY area_sq_km DESC;

-- 11
DROP TABLE IF EXISTS public.user_points;
CREATE TABLE public.user_points (
    point_id serial PRIMARY KEY,
    point_name text NOT NULL,
    geom geometry(Point, 4326) NOT NULL
);

INSERT INTO public.user_points (point_name, geom)
VALUES
    ('Minsk point', ST_SetSRID(ST_MakePoint(27.5615, 53.9045), 4326)),
    ('Warsaw point', ST_SetSRID(ST_MakePoint(21.0122, 52.2298), 4326));

DROP TABLE IF EXISTS public.user_lines;
CREATE TABLE public.user_lines (
    line_id serial PRIMARY KEY,
    line_name text NOT NULL,
    geom geometry(LineString, 4326) NOT NULL
);

INSERT INTO public.user_lines (line_name, geom)
VALUES (
    'Belarus route',
    ST_GeomFromText('LINESTRING(23.1783 53.1327, 27.5615 53.9045, 30.3347 59.9343)', 4326)
);

DROP TABLE IF EXISTS public.user_polygons;
CREATE TABLE public.user_polygons (
    polygon_id serial PRIMARY KEY,
    polygon_name text NOT NULL,
    geom geometry(Polygon, 4326) NOT NULL
);

INSERT INTO public.user_polygons (polygon_name, geom)
VALUES (
    'Belarus test polygon',
    ST_GeomFromText(
        'POLYGON((27.3 53.8, 27.8 53.8, 27.8 54.1, 27.3 54.1, 27.3 53.8))',
        4326
    )
);

-- 12
SELECT
    p.point_name,
    c.admin AS country_name
FROM public.user_points p
JOIN public.countries c
  ON ST_Contains(c.geom, p.geom)
ORDER BY p.point_id;

SELECT
    l.line_name,
    c.admin AS country_name
FROM public.user_lines l
JOIN public.countries c
  ON ST_Intersects(c.geom, l.geom)
ORDER BY l.line_id, c.admin;

SELECT
    p.polygon_name,
    c.admin AS country_name
FROM public.user_polygons p
JOIN public.countries c
  ON ST_Intersects(c.geom, p.geom)
ORDER BY p.polygon_id, c.admin;

-- 13
CREATE INDEX IF NOT EXISTS idx_countries_geom
ON public.countries
USING GIST (geom);

CREATE INDEX IF NOT EXISTS idx_user_points_geom
ON public.user_points
USING GIST (geom);

EXPLAIN ANALYZE
SELECT
    c.admin
FROM public.countries c
WHERE ST_Intersects(
    c.geom,
    ST_SetSRID(ST_MakePoint(27.5615, 53.9045), 4326)
);

-- 14
CREATE OR REPLACE FUNCTION public.find_country_by_point(
    p_lon double precision,
    p_lat double precision
)
RETURNS TABLE (
    country_name text,
    point_wkt text
)
LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
    SELECT
        c.admin::text,
        ST_AsText(ST_SetSRID(ST_MakePoint(p_lon, p_lat), 4326))
    FROM public.countries c
    WHERE ST_Contains(c.geom, ST_SetSRID(ST_MakePoint(p_lon, p_lat), 4326));
END;
$$;

SELECT * FROM public.find_country_by_point(27.5615, 53.9045);
