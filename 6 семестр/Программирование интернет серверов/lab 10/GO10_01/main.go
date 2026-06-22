package main

import (
	"database/sql"
	"flag"
	"log"
	"net/http"

	_ "github.com/mattn/go-sqlite3"
)

func main() {
	dbPath := flag.String("db", "Celebrities.db", "path to sqlite database")
	addr := flag.String("addr", ":3000", "http listen address")
	flag.Parse()

	db, err := sql.Open("sqlite3", *dbPath)
	if err != nil {
		log.Fatalf("cannot open database: %v", err)
	}
	defer db.Close()

	store, err := NewSQLStore(db)
	if err != nil {
		log.Fatalf("cannot initialize SQL store: %v", err)
	}

	router := NewRouter(store)
	log.Printf("GO10_01 listening on %s using %s", *addr, *dbPath)
	if err := http.ListenAndServe(*addr, router); err != nil {
		log.Fatalf("server stopped: %v", err)
	}
}
