#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/01-generate-ca.sh XYZ
# Example:
#   ./scripts/01-generate-ca.sh YP

INITIALS="${1:-XYZ}"
LAB_DIR="$(cd "$(dirname "$0")/.." && pwd)"
CERTS_DIR="$LAB_DIR/certs"
CN="CA-LAB22-${INITIALS}"

mkdir -p "$CERTS_DIR"

openssl req -x509 -newkey rsa:2048 -nodes \
  -keyout "$CERTS_DIR/ca.key.pem" \
  -out "$CERTS_DIR/ca.crt.pem" \
  -days 365 \
  -sha256 \
  -subj "/CN=${CN}" \
  -addext "basicConstraints=critical,CA:TRUE" \
  -addext "keyUsage=critical,digitalSignature,keyCertSign,cRLSign"

echo "CA certificate generated: $CERTS_DIR/ca.crt.pem"
echo "CA private key generated: $CERTS_DIR/ca.key.pem"
