package main

import (
	"bytes"
	"encoding/json"
	"net/http"
	"net/http/httptest"
	"path/filepath"
	"testing"
)

func newLab4TestServer(t *testing.T, seed []Celebrity) http.Handler {
	t.Helper()

	dataPath := filepath.Join(t.TempDir(), "Celebrities.json")
	store, err := NewFileStore(dataPath, seed)
	if err != nil {
		t.Fatalf("NewFileStore() error = %v", err)
	}

	return NewRouter(store)
}

func TestGetAllCelebrities(t *testing.T) {
	handler := newLab4TestServer(t, []Celebrity{
		{Id: 1, FullName: "Keanu Reeves", Nationality: "Canadian", ReqPhotoPath: "/img/keanu.jpg"},
	})

	req := httptest.NewRequest(http.MethodGet, "/Celebrities/All", nil)
	rec := httptest.NewRecorder()
	handler.ServeHTTP(rec, req)

	if rec.Code != http.StatusOK {
		t.Fatalf("status = %d, want %d", rec.Code, http.StatusOK)
	}

	var got []Celebrity
	if err := json.Unmarshal(rec.Body.Bytes(), &got); err != nil {
		t.Fatalf("json.Unmarshal() error = %v", err)
	}

	if len(got) != 1 || got[0].Id != 1 {
		t.Fatalf("unexpected payload = %+v", got)
	}
}

func TestGetCelebrityByID(t *testing.T) {
	handler := newLab4TestServer(t, []Celebrity{
		{Id: 7, FullName: "Emma Watson", Nationality: "British", ReqPhotoPath: "/img/emma.jpg"},
	})

	req := httptest.NewRequest(http.MethodGet, "/Celebrities/7", nil)
	rec := httptest.NewRecorder()
	handler.ServeHTTP(rec, req)

	if rec.Code != http.StatusOK {
		t.Fatalf("status = %d, want %d", rec.Code, http.StatusOK)
	}
}

func TestPostCelebrityConflict(t *testing.T) {
	handler := newLab4TestServer(t, []Celebrity{
		{Id: 3, FullName: "Tom Hanks", Nationality: "American", ReqPhotoPath: "/img/tom.jpg"},
	})

	body, _ := json.Marshal(Celebrity{Id: 3, FullName: "Tom Hanks", Nationality: "American", ReqPhotoPath: "/img/tom.jpg"})
	req := httptest.NewRequest(http.MethodPost, "/Celebrities", bytes.NewReader(body))
	req.Header.Set("Content-Type", "application/json")
	rec := httptest.NewRecorder()
	handler.ServeHTTP(rec, req)

	if rec.Code != http.StatusConflict {
		t.Fatalf("status = %d, want %d", rec.Code, http.StatusConflict)
	}
}

func TestPutCelebrityNotFound(t *testing.T) {
	handler := newLab4TestServer(t, nil)

	body, _ := json.Marshal(Celebrity{Id: 11, FullName: "Scarlett Johansson", Nationality: "American", ReqPhotoPath: "/img/scarlett.jpg"})
	req := httptest.NewRequest(http.MethodPut, "/Celebrities/11", bytes.NewReader(body))
	req.Header.Set("Content-Type", "application/json")
	rec := httptest.NewRecorder()
	handler.ServeHTTP(rec, req)

	if rec.Code != http.StatusNotFound {
		t.Fatalf("status = %d, want %d", rec.Code, http.StatusNotFound)
	}
}

func TestDeleteCelebrityNotFound(t *testing.T) {
	handler := newLab4TestServer(t, nil)

	req := httptest.NewRequest(http.MethodDelete, "/Celebrities/19", nil)
	rec := httptest.NewRecorder()
	handler.ServeHTTP(rec, req)

	if rec.Code != http.StatusNotFound {
		t.Fatalf("status = %d, want %d", rec.Code, http.StatusNotFound)
	}
}
