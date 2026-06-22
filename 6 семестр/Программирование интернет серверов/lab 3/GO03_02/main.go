package main

import (
	"log"
	"net/http"

	"GO03_02/P03_02"
)

func main() {
	stat := &P03_02.Stat{}

	mux := http.NewServeMux()

	mux.HandleFunc("/S", func(w http.ResponseWriter, r *http.Request) {
		switch r.Method {
		case http.MethodGet:
			stat.PlusGet()
			w.WriteHeader(http.StatusOK)
		case http.MethodPost:
			stat.PlusPost()
			w.WriteHeader(http.StatusOK)
		default:
			http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
		}
	})

	mux.HandleFunc("/G", func(w http.ResponseWriter, r *http.Request) {
		if r.Method != http.MethodGet {
			http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
			return
		}
		w.Header().Set("Content-Type", "text/plain; charset=utf-8")
		_, _ = w.Write([]byte(stat.GenStr()))
	})

	addr := ":3000"
	log.Printf("listening on %s", addr)
	if err := http.ListenAndServe(addr, mux); err != nil {
		log.Fatal(err)
	}
}
