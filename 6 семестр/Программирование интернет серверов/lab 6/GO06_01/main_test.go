package main

import (
	"bytes"
	"encoding/json"
	"net/http"
	"net/http/httptest"
	"path/filepath"
	"testing"

	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
	gormlogger "gorm.io/gorm/logger"
)

func newLab6TestServer(t *testing.T, seed []Celebrity) http.Handler {
	t.Helper()

	dbPath := filepath.Join(t.TempDir(), "celebrities-gorm.db")
	db, err := gorm.Open(sqlite.Open(dbPath), &gorm.Config{
		Logger: gormlogger.Default.LogMode(gormlogger.Silent),
	})
	if err != nil {
		t.Fatalf("gorm.Open() error = %v", err)
	}

	store, err := NewGormStore(db)
	if err != nil {
		t.Fatalf("NewGormStore() error = %v", err)
	}

	for _, item := range seed {
		if err := store.Create(item); err != nil {
			t.Fatalf("seed Create() error = %v", err)
		}
	}

	return NewRouter(store)
}

func TestGetAllCelebrities(t *testing.T) {
	handler := newLab6TestServer(t, []Celebrity{
		{Id: 1, FullName: "Keanu Reeves", Nationality: "Canadian", ReqPhotoPath: "/img/keanu.jpg"},
	})

	req := httptest.NewRequest(http.MethodGet, "/Celebrities/All", nil)
	rec := httptest.NewRecorder()
	handler.ServeHTTP(rec, req)

	if rec.Code != http.StatusOK {
		t.Fatalf("status = %d, want %d", rec.Code, http.StatusOK)
	}
}

func TestGetCelebrityByID(t *testing.T) {
	handler := newLab6TestServer(t, []Celebrity{
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
	handler := newLab6TestServer(t, []Celebrity{
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
	handler := newLab6TestServer(t, nil)

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
	handler := newLab6TestServer(t, nil)

	req := httptest.NewRequest(http.MethodDelete, "/Celebrities/19", nil)
	rec := httptest.NewRecorder()
	handler.ServeHTTP(rec, req)

	if rec.Code != http.StatusNotFound {
		t.Fatalf("status = %d, want %d", rec.Code, http.StatusNotFound)
	}
}
