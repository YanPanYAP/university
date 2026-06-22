#!/bin/zsh
set -euo pipefail

if [[ ! -f "./data/ne_10m_admin_0_countries.shp" ]]; then
  echo "Place Natural Earth shapefile files into ./data first."
  echo "Required file: ./data/ne_10m_admin_0_countries.shp"
  exit 1
fi

docker compose up -d

docker run --rm \
  --network lab4_default \
  -v "$PWD/data:/workspace/data" \
  ghcr.io/osgeo/gdal:alpine-small-3.9.0 \
  ogr2ogr \
  -overwrite \
  -f PostgreSQL \
  "PG:host=postgis dbname=spatial_lab user=postgres password=postgres" \
  /workspace/data/ne_10m_admin_0_countries.shp \
  -nln public.countries \
  -nlt PROMOTE_TO_MULTI \
  -lco GEOMETRY_NAME=geom \
  -lco FID=gid

docker compose exec -T postgis psql -U postgres -d spatial_lab -f /workspace/sql/lab4_postgis.sql
