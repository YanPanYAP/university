package main

import (
	"errors"
	"flag"
	"log"
	"net/http"

	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
	gormlogger "gorm.io/gorm/logger"
)

var (
	ErrNotFound = errors.New("celebrity not found")
	ErrConflict = errors.New("celebrity already exists")
)

func main() {
	dbPath := flag.String("db", "Celebrities.db", "path to sqlite database")
	addr := flag.String("addr", ":3000", "http listen address")
	flag.Parse()

	db, err := gorm.Open(sqlite.Open(*dbPath), &gorm.Config{
		Logger: gormlogger.Default.LogMode(gormlogger.Silent),
	})
	if err != nil {
		log.Fatalf("cannot open database: %v", err)
	}

	store, err := NewGormStore(db)
	if err != nil {
		log.Fatalf("cannot initialize GORM store: %v", err)
	}

	router := NewRouter(store)
	log.Printf("GO06_01 listening on %s using %s", *addr, *dbPath)
	if err := http.ListenAndServe(*addr, router); err != nil {
		log.Fatalf("server stopped: %v", err)
	}
}
