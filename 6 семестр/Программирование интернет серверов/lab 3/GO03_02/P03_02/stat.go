package P03_02

import (
	"fmt"
	"sync"
)

type Stat struct {
	mu    sync.Mutex
	getN  int
	postN int
}

func (s *Stat) PlusGet() {
	s.mu.Lock()
	s.getN++
	s.mu.Unlock()
}

func (s *Stat) PlusPost() {
	s.mu.Lock()
	s.postN++
	s.mu.Unlock()
}

func (s *Stat) GenStr() string {
	s.mu.Lock()
	defer s.mu.Unlock()
	return fmt.Sprintf("Get-request count = %d, Post-request count = %d", s.getN, s.postN)
}
