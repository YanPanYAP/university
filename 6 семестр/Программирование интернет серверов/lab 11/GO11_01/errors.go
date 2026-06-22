package main

import "errors"

var (
	ErrNotFound = errors.New("celebrity not found")
	ErrConflict = errors.New("celebrity already exists")
)
