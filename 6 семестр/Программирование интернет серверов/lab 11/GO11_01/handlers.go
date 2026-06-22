package main

import (
	"encoding/json"
	"errors"
	"log"
	"net/http"
	"strconv"

	"github.com/gorilla/mux"
)

func NewRouter(store CelebrityStore) http.Handler {
	r := mux.NewRouter()
	r.HandleFunc("/Celebrities/All", getAllHandler(store)).Methods(http.MethodGet)
	r.HandleFunc("/Celebrities/{id}", getByIDHandler(store)).Methods(http.MethodGet)
	r.HandleFunc("/Celebrities", createHandler(store)).Methods(http.MethodPost)
	r.HandleFunc("/Celebrities/{id}", updateHandler(store)).Methods(http.MethodPut)
	r.HandleFunc("/Celebrities/{id}", deleteHandler(store)).Methods(http.MethodDelete)
	r.HandleFunc("/openapi.json", openAPIHandler()).Methods(http.MethodGet)
	r.HandleFunc("/docs", openAPIUIHandler()).Methods(http.MethodGet)
	r.PathPrefix("/swagger/").HandlerFunc(openAPIUIHandler()).Methods(http.MethodGet)
	return r
}

func getAllHandler(store CelebrityStore) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		log.Printf("%s %s", r.Method, r.URL.Path)
		items, err := store.All()
		if err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}
		writeJSON(w, http.StatusOK, items)
	}
}

func getByIDHandler(store CelebrityStore) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		log.Printf("%s %s", r.Method, r.URL.Path)
		id, ok := parseID(w, r)
		if !ok {
			return
		}

		item, err := store.ByID(id)
		if err != nil {
			if errors.Is(err, ErrNotFound) {
				http.Error(w, err.Error(), http.StatusNotFound)
				return
			}
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}
		writeJSON(w, http.StatusOK, item)
	}
}

func createHandler(store CelebrityStore) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		log.Printf("%s %s", r.Method, r.URL.Path)
		var item Celebrity
		if err := json.NewDecoder(r.Body).Decode(&item); err != nil {
			http.Error(w, "invalid json body", http.StatusBadRequest)
			return
		}

		if err := store.Create(item); err != nil {
			if errors.Is(err, ErrConflict) {
				http.Error(w, err.Error(), http.StatusConflict)
				return
			}
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}
		writeJSON(w, http.StatusCreated, item)
	}
}

func updateHandler(store CelebrityStore) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		log.Printf("%s %s", r.Method, r.URL.Path)
		id, ok := parseID(w, r)
		if !ok {
			return
		}

		var item Celebrity
		if err := json.NewDecoder(r.Body).Decode(&item); err != nil {
			http.Error(w, "invalid json body", http.StatusBadRequest)
			return
		}
		item.Id = id

		if err := store.Update(id, item); err != nil {
			if errors.Is(err, ErrNotFound) {
				http.Error(w, err.Error(), http.StatusNotFound)
				return
			}
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}
		writeJSON(w, http.StatusOK, item)
	}
}

func deleteHandler(store CelebrityStore) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		log.Printf("%s %s", r.Method, r.URL.Path)
		id, ok := parseID(w, r)
		if !ok {
			return
		}

		if err := store.Delete(id); err != nil {
			if errors.Is(err, ErrNotFound) {
				http.Error(w, err.Error(), http.StatusNotFound)
				return
			}
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}
		w.WriteHeader(http.StatusNoContent)
	}
}

func parseID(w http.ResponseWriter, r *http.Request) (int, bool) {
	idValue := mux.Vars(r)["id"]
	id, err := strconv.Atoi(idValue)
	if err != nil {
		http.Error(w, "invalid id", http.StatusBadRequest)
		return 0, false
	}
	return id, true
}

func writeJSON(w http.ResponseWriter, status int, value any) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(status)
	_ = json.NewEncoder(w).Encode(value)
}
