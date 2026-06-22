# GO06_01

REST web-server on port `3000` for working with the `Celebrities` table through GORM and SQLite.

## Run

```bash
go mod tidy
go run . -db Celebrities.db
```

## Build executable

```bash
go build -o GO06_01
```

## Run executable from another directory

```bash
mkdir -p /tmp/go06-run
cp GO06_01 /tmp/go06-run/
cd /tmp/go06-run
./GO06_01 -db Celebrities.db
```

The SQLite database file will be created automatically.
