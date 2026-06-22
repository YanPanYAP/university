package main

import (
	"errors"
	"flag"
	"log"
	"net/http"
)

var (
	ErrNotFound = errors.New("celebrity not found")
	ErrConflict = errors.New("celebrity already exists")
)

func main() {
	dataPath := flag.String("data", "Celebrities.json", "path to Celebrities.json")
	addr := flag.String("addr", ":3000", "http listen address")
	flag.Parse()

	store, err := NewFileStore(*dataPath, nil)
	if err != nil {
		log.Fatalf("cannot initialize file store: %v", err)
	}

	router := NewRouter(store)
	log.Printf("GO04_01 listening on %s using %s", *addr, *dataPath)
	if err := http.ListenAndServe(*addr, router); err != nil {
		log.Fatalf("server stopped: %v", err)
	}
}
