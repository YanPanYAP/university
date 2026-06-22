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

call GET /A
call GET /A/B
call POST /A
call POST /A/B
call PUT /A
call PUT /A/B

call GET /other
call POST /other
call PUT /other
