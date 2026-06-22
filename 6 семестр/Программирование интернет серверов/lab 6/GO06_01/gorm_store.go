package main

import (
	"errors"

	"gorm.io/gorm"
)

type GormStore struct {
	db *gorm.DB
}

func NewGormStore(db *gorm.DB) (*GormStore, error) {
	store := &GormStore{db: db}
	if err := db.AutoMigrate(&Celebrity{}); err != nil {
		return nil, err
	}
	return store, nil
}

func (s *GormStore) All() ([]Celebrity, error) {
	var items []Celebrity
	err := s.db.Order("id").Find(&items).Error
	return items, err
}

func (s *GormStore) ByID(id int) (Celebrity, error) {
	var item Celebrity
	err := s.db.First(&item, "id = ?", id).Error
	if errors.Is(err, gorm.ErrRecordNotFound) {
		return Celebrity{}, ErrNotFound
	}
	return item, err
}

func (s *GormStore) Create(item Celebrity) error {
	if _, err := s.ByID(item.Id); err == nil {
		return ErrConflict
	} else if err != nil && err != ErrNotFound {
		return err
	}
	return s.db.Create(&item).Error
}

func (s *GormStore) Update(id int, item Celebrity) error {
	result := s.db.Model(&Celebrity{}).Where("id = ?", id).Updates(map[string]any{
		"full_name":      item.FullName,
		"nationality":    item.Nationality,
		"req_photo_path": item.ReqPhotoPath,
	})
	if result.Error != nil {
		return result.Error
	}
	if result.RowsAffected == 0 {
		return ErrNotFound
	}
	return nil
}

func (s *GormStore) Delete(id int) error {
	result := s.db.Delete(&Celebrity{}, "id = ?", id)
	if result.Error != nil {
		return result.Error
	}
	if result.RowsAffected == 0 {
		return ErrNotFound
	}
	return nil
}
