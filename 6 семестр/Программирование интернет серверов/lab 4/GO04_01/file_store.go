package main

import (
	"encoding/json"
	"os"
	"path/filepath"
	"sync"
)

type FileStore struct {
	path string
	mu   sync.Mutex
}

func NewFileStore(path string, seed []Celebrity) (*FileStore, error) {
	store := &FileStore{path: path}
	if _, err := os.Stat(path); os.IsNotExist(err) {
		if err := os.MkdirAll(filepath.Dir(path), 0o755); err != nil {
			return nil, err
		}
		if err := store.writeAll(seed); err != nil {
			return nil, err
		}
	} else if err != nil {
		return nil, err
	}
	return store, nil
}

func (s *FileStore) All() ([]Celebrity, error) {
	s.mu.Lock()
	defer s.mu.Unlock()
	return s.readAll()
}

func (s *FileStore) ByID(id int) (Celebrity, error) {
	s.mu.Lock()
	defer s.mu.Unlock()

	items, err := s.readAll()
	if err != nil {
		return Celebrity{}, err
	}
	for _, item := range items {
		if item.Id == id {
			return item, nil
		}
	}
	return Celebrity{}, ErrNotFound
}

func (s *FileStore) Create(item Celebrity) error {
	s.mu.Lock()
	defer s.mu.Unlock()

	items, err := s.readAll()
	if err != nil {
		return err
	}
	for _, current := range items {
		if current.Id == item.Id {
			return ErrConflict
		}
	}
	items = append(items, item)
	return s.writeAll(items)
}

func (s *FileStore) Update(id int, item Celebrity) error {
	s.mu.Lock()
	defer s.mu.Unlock()

	items, err := s.readAll()
	if err != nil {
		return err
	}
	for index, current := range items {
		if current.Id == id {
			items[index] = item
			return s.writeAll(items)
		}
	}
	return ErrNotFound
}

func (s *FileStore) Delete(id int) error {
	s.mu.Lock()
	defer s.mu.Unlock()

	items, err := s.readAll()
	if err != nil {
		return err
	}
	for index, current := range items {
		if current.Id == id {
			items = append(items[:index], items[index+1:]...)
			return s.writeAll(items)
		}
	}
	return ErrNotFound
}

func (s *FileStore) readAll() ([]Celebrity, error) {
	content, err := os.ReadFile(s.path)
	if err != nil {
		return nil, err
	}
	if len(content) == 0 {
		return []Celebrity{}, nil
	}

	var items []Celebrity
	if err := json.Unmarshal(content, &items); err != nil {
		return nil, err
	}
	if items == nil {
		return []Celebrity{}, nil
	}
	return items, nil
}

func (s *FileStore) writeAll(items []Celebrity) error {
	if items == nil {
		items = []Celebrity{}
	}
	content, err := json.MarshalIndent(items, "", "  ")
	if err != nil {
		return err
	}
	content = append(content, '\n')
	return os.WriteFile(s.path, content, 0o644)
}
