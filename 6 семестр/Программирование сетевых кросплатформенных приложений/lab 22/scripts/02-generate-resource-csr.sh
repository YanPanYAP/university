#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/02-generate-resource-csr.sh ABC
# Example:
#   ./scripts/02-generate-resource-csr.sh YP

INITIALS="${1:-ABC}"
LAB_DIR="$(cd "$(dirname "$0")/.." && pwd)"
CERTS_DIR="$LAB_DIR/certs"
CN="RS-LAB22-${INITIALS}"

mkdir -p "$CERTS_DIR"

openssl req -new -newkey rsa:2048 -nodes \
  -keyout "$CERTS_DIR/resource.key.pem" \
  -out "$CERTS_DIR/resource.csr.pem" \
  -sha256 \
  -subj "/CN=${CN}"

echo "Resource private key generated: $CERTS_DIR/resource.key.pem"
echo "Resource CSR generated: $CERTS_DIR/resource.csr.pem"
