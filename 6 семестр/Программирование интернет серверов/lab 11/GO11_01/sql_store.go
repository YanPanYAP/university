package main

import "database/sql"

type SQLStore struct {
	db *sql.DB
}

func NewSQLStore(db *sql.DB) (*SQLStore, error) {
	store := &SQLStore{db: db}
	if err := store.init(); err != nil {
		return nil, err
	}
	return store, nil
}

func (s *SQLStore) init() error {
	_, err := s.db.Exec(`
		CREATE TABLE IF NOT EXISTS celebrities (
			id INTEGER PRIMARY KEY,
			full_name TEXT NOT NULL,
			nationality TEXT NOT NULL,
			req_photo_path TEXT NOT NULL
		)
	`)
	return err
}

func (s *SQLStore) All() ([]Celebrity, error) {
	rows, err := s.db.Query(`
		SELECT id, full_name, nationality, req_photo_path
		FROM celebrities
		ORDER BY id
	`)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	items := []Celebrity{}
	for rows.Next() {
		var item Celebrity
		if err := rows.Scan(&item.Id, &item.FullName, &item.Nationality, &item.ReqPhotoPath); err != nil {
			return nil, err
		}
		items = append(items, item)
	}
	return items, rows.Err()
}

func (s *SQLStore) ByID(id int) (Celebrity, error) {
	var item Celebrity
	err := s.db.QueryRow(`
		SELECT id, full_name, nationality, req_photo_path
		FROM celebrities
		WHERE id = ?
	`, id).Scan(&item.Id, &item.FullName, &item.Nationality, &item.ReqPhotoPath)
	if err == sql.ErrNoRows {
		return Celebrity{}, ErrNotFound
	}
	if err != nil {
		return Celebrity{}, err
	}
	return item, nil
}

func (s *SQLStore) Create(item Celebrity) error {
	if _, err := s.ByID(item.Id); err == nil {
		return ErrConflict
	} else if err != nil && err != ErrNotFound {
		return err
	}

	_, err := s.db.Exec(`
		INSERT INTO celebrities (id, full_name, nationality, req_photo_path)
		VALUES (?, ?, ?, ?)
	`, item.Id, item.FullName, item.Nationality, item.ReqPhotoPath)
	return err
}

func (s *SQLStore) Update(id int, item Celebrity) error {
	result, err := s.db.Exec(`
		UPDATE celebrities
		SET full_name = ?, nationality = ?, req_photo_path = ?
		WHERE id = ?
	`, item.FullName, item.Nationality, item.ReqPhotoPath, id)
	if err != nil {
		return err
	}

	affected, err := result.RowsAffected()
	if err != nil {
		return err
	}
	if affected == 0 {
		return ErrNotFound
	}
	return nil
}

func (s *SQLStore) Delete(id int) error {
	result, err := s.db.Exec(`DELETE FROM celebrities WHERE id = ?`, id)
	if err != nil {
		return err
	}

	affected, err := result.RowsAffected()
	if err != nil {
		return err
	}
	if affected == 0 {
		return ErrNotFound
	}
	return nil
}
