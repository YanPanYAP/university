#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/03-sign-resource-cert.sh ABC
# Example:
#   ./scripts/03-sign-resource-cert.sh YP

INITIALS="${1:-ABC}"
LAB_DIR="$(cd "$(dirname "$0")/.." && pwd)"
CERTS_DIR="$LAB_DIR/certs"
EXT_FILE="$LAB_DIR/openssl/resource.ext"

if [[ ! -f "$CERTS_DIR/ca.key.pem" || ! -f "$CERTS_DIR/ca.crt.pem" ]]; then
  echo "CA files not found in $CERTS_DIR. Run 01-generate-ca.sh first."
  exit 1
fi

if [[ ! -f "$CERTS_DIR/resource.csr.pem" ]]; then
  echo "Resource CSR not found in $CERTS_DIR. Run 02-generate-resource-csr.sh first."
  exit 1
fi

cat > "$EXT_FILE" << EOT
authorityKeyIdentifier=keyid,issuer
basicConstraints=CA:FALSE
keyUsage=digitalSignature,keyEncipherment
extendedKeyUsage=serverAuth
subjectAltName=@alt_names

[alt_names]
DNS.1=LAB22-${INITIALS}
DNS.2=${INITIALS}
EOT

openssl x509 -req \
  -in "$CERTS_DIR/resource.csr.pem" \
  -CA "$CERTS_DIR/ca.crt.pem" \
  -CAkey "$CERTS_DIR/ca.key.pem" \
  -CAcreateserial \
  -out "$CERTS_DIR/resource.crt.pem" \
  -days 365 \
  -sha256 \
  -extfile "$EXT_FILE"

echo "Resource certificate generated: $CERTS_DIR/resource.crt.pem"
echo "CA certificate to transfer/import: $CERTS_DIR/ca.crt.pem"
