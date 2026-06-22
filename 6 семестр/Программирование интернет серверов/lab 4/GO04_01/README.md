# GO04_01

REST web-server on port `3000` for working with `Celebrities.json`.

## Run

```bash
go mod tidy
go run . -data Celebrities.json
```

## Build executable

```bash
go build -o GO04_01
```

## Run executable from another directory

```bash
mkdir -p /tmp/go04-run
cp GO04_01 Celebrities.json /tmp/go04-run/
cd /tmp/go04-run
./GO04_01 -data Celebrities.json
```

## Linux executable

```bash
GOOS=linux GOARCH=amd64 go build -o GO04_01_linux
```

## Example requests

```bash
curl http://localhost:3000/Celebrities/All
curl http://localhost:3000/Celebrities/1
```
