package main

type Celebrity struct {
	Id           int    `json:"id"`
	FullName     string `json:"fullName"`
	Nationality  string `json:"nationality"`
	ReqPhotoPath string `json:"reqPhotoPath"`
}

type CelebrityStore interface {
	All() ([]Celebrity, error)
	ByID(id int) (Celebrity, error)
	Create(item Celebrity) error
	Update(id int, item Celebrity) error
	Delete(id int) error
}
