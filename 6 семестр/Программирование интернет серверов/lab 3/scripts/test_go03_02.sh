#!/usr/bin/env bash
set -euo pipefail

base="http://localhost:3000"

call() {
  local method="$1"
  local path="$2"
  local url="${base}${path}"
  local code
  code=$(curl -s -o /dev/null -w "%{http_code}" -X "$method" "$url")
  printf "%s %s -> %s\n" "$method" "$path" "$code"
}

echo "Step 1: send requests to /S"
call GET /S
call GET /S
call POST /S
call POST /S
call POST /S

echo "Step 2: check /G"
body=$(curl -s -X GET "${base}/G")
printf "GET /G -> %s\n" "$body"
