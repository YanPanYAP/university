package main

import (
	"fmt"
	"io"
	"net/http"

	"GO02_01/go02_01lib"
)

const C01 = 3.14

func handler(w http.ResponseWriter, r *http.Request) {
	switch r.Method {
	case http.MethodGet:
		_, _ = fmt.Fprintf(w,
			"C01 = %e,\nC02 = %e,\nC03 = %e\n",
			C01,
			C02,
			go02_01lib.C03,
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
	if err := http.ListenAndServe(":3000", nil); err != nil {
		panic(err)
	}
}
