#!/usr/bin/env bash
set -euo pipefail

BASE_URL="${1:-http://localhost:5001/lb}"
COUNT_PER_METHOD="${2:-50}"
KEEP_SESSION="${3:-false}"

if ! [[ "$COUNT_PER_METHOD" =~ ^[0-9]+$ ]]; then
  echo "COUNT_PER_METHOD must be an integer" >&2
  exit 1
fi

methods=("GET" "POST" "PUT" "DELETE")

get_x=0; get_y=0; get_z=0
post_x=0; post_y=0; post_z=0
put_x=0; put_y=0; put_z=0
delete_x=0; delete_y=0; delete_z=0

total_x=0; total_y=0; total_z=0

cookie_file=""
if [[ "$KEEP_SESSION" == "true" ]]; then
  cookie_file="$(mktemp)"
  trap 'rm -f "$cookie_file"' EXIT
fi

increment_count() {
  local method="$1"
  local nick="$2"
  case "$method:$nick" in
    GET:X) get_x=$((get_x + 1)) ;;
    GET:Y) get_y=$((get_y + 1)) ;;
    GET:Z) get_z=$((get_z + 1)) ;;
    POST:X) post_x=$((post_x + 1)) ;;
    POST:Y) post_y=$((post_y + 1)) ;;
    POST:Z) post_z=$((post_z + 1)) ;;
    PUT:X) put_x=$((put_x + 1)) ;;
    PUT:Y) put_y=$((put_y + 1)) ;;
    PUT:Z) put_z=$((put_z + 1)) ;;
    DELETE:X) delete_x=$((delete_x + 1)) ;;
    DELETE:Y) delete_y=$((delete_y + 1)) ;;
    DELETE:Z) delete_z=$((delete_z + 1)) ;;
    *)
      echo "Unexpected Nick for $method: $nick" >&2
      exit 1
      ;;
  esac

  case "$nick" in
    X) total_x=$((total_x + 1)) ;;
    Y) total_y=$((total_y + 1)) ;;
    Z) total_z=$((total_z + 1)) ;;
    *)
      echo "Unexpected Nick: $nick" >&2
      exit 1
      ;;
  esac
}

for method in "${methods[@]}"; do
  for ((i=1; i<=COUNT_PER_METHOD; i++)); do
    if [[ "$KEEP_SESSION" == "true" ]]; then
      response="$(curl -sS -X "$method" -c "$cookie_file" -b "$cookie_file" "$BASE_URL")"
    else
      response="$(curl -sS -X "$method" "$BASE_URL")"
    fi

    nick="$(printf '%s' "$response" | sed -n 's/.*"Nick":"\([^"]*\)".*/\1/p')"

    if [[ -z "$nick" ]]; then
      echo "Unexpected response for $method #$i: $response" >&2
      exit 1
    fi

    increment_count "$method" "$nick"
  done
done

echo "=== Result by HTTP method ==="
echo
echo "[GET]"
echo "X : $get_x"
echo "Y : $get_y"
echo "Z : $get_z"

echo
echo "[POST]"
echo "X : $post_x"
echo "Y : $post_y"
echo "Z : $post_z"

echo
echo "[PUT]"
echo "X : $put_x"
echo "Y : $put_y"
echo "Z : $put_z"

echo
echo "[DELETE]"
echo "X : $delete_x"
echo "Y : $delete_y"
echo "Z : $delete_z"

echo
echo "=== Total for all methods ==="
echo "X : $total_x"
echo "Y : $total_y"
echo "Z : $total_z"
