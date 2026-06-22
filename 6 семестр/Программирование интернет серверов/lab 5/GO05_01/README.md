# GO05_01

REST web-server on port `3000` for working with the `Celebrities` table through `database/sql` and SQLite.

## Run

```bash
go mod tidy
go run . -db Celebrities.db
```

## Build executable

```bash
go build -o GO05_01
```

## Run executable from another directory

```bash
mkdir -p /tmp/go05-run
cp GO05_01 /tmp/go05-run/
cd /tmp/go05-run
./GO05_01 -db Celebrities.db
```

The SQLite database file will be created automatically.
