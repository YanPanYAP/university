package main

import (
	"fmt"
	"io"
	"net/http"

	"GO02_02/go02_02lib"
)

var A01 = 3

func handler(w http.ResponseWriter, r *http.Request) {
	switch r.Method {
	case http.MethodGet:
		_, _ = fmt.Fprintf(w,
			"A01 = %d\nA02 = %t\nA03 = %s\n",
			A01,
			A02,
			go02_02lib.A03,
		)
	case http.MethodPost, http.MethodPut, http.MethodDelete:
		body, _ := io.ReadAll(r.Body)
		_, _ = fmt.Fprintf(w, "%s request processed. Body: %s\n", r.Method, string(body))
	default:
		w.Header().Set("Allow", "GET, POST, PUT, DELETE")
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
	}
}

func main() {
	http.HandleFunc("/", handler)
	if err := http.ListenAndServe(":4000", nil); err != nil {
		panic(err)
	}
}
