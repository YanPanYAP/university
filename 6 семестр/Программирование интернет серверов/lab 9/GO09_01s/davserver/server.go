package davserver

import (
	"errors"
	"fmt"
	"io"
	"net/http"
	"net/url"
	"os"
	"path"
	"path/filepath"
	"strings"
)

type Options struct {
	Root string
	User string
	Pass string
}

type Server struct {
	root string
	user string
	pass string
}

func New(options Options) http.Handler {
	root := options.Root
	if root == "" {
		root = "webdav-root"
	}

	return &Server{
		root: root,
		user: options.User,
		pass: options.Pass,
	}
}

func (s *Server) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	if !s.authorized(r) {
		w.Header().Set("WWW-Authenticate", `Basic realm="GO09 WebDAV"`)
		http.Error(w, "authorization required", http.StatusUnauthorized)
		return
	}

	switch r.Method {
	case "MKCOL":
		s.handleMkcol(w, r)
	case http.MethodPut:
		s.handlePut(w, r)
	case http.MethodGet:
		s.handleGet(w, r)
	case "COPY":
		s.handleCopy(w, r)
	case "MOVE":
		s.handleMove(w, r)
	case http.MethodDelete:
		s.handleDelete(w, r)
	default:
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
	}
}

func (s *Server) authorized(r *http.Request) bool {
	if s.user == "" && s.pass == "" {
		return true
	}
	user, pass, ok := r.BasicAuth()
	return ok && user == s.user && pass == s.pass
}

func (s *Server) handleMkcol(w http.ResponseWriter, r *http.Request) {
	target, err := s.resolveURLPath(r.URL.Path)
	if err != nil {
		http.Error(w, err.Error(), http.StatusForbidden)
		return
	}
	if exists(target) {
		http.Error(w, "collection already exists", http.StatusMethodNotAllowed)
		return
	}
	if !exists(filepath.Dir(target)) {
		http.Error(w, "parent collection does not exist", http.StatusConflict)
		return
	}
	if err := os.Mkdir(target, 0o755); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	w.WriteHeader(http.StatusCreated)
}

func (s *Server) handlePut(w http.ResponseWriter, r *http.Request) {
	target, err := s.resolveURLPath(r.URL.Path)
	if err != nil {
		http.Error(w, err.Error(), http.StatusForbidden)
		return
	}
	if !exists(filepath.Dir(target)) {
		http.Error(w, "parent collection does not exist", http.StatusConflict)
		return
	}

	created := !exists(target)
	file, err := os.OpenFile(target, os.O_CREATE|os.O_WRONLY|os.O_TRUNC, 0o644)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	defer file.Close()

	if _, err := io.Copy(file, r.Body); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	if created {
		w.WriteHeader(http.StatusCreated)
		return
	}
	w.WriteHeader(http.StatusNoContent)
}

func (s *Server) handleGet(w http.ResponseWriter, r *http.Request) {
	target, err := s.resolveURLPath(r.URL.Path)
	if err != nil {
		http.Error(w, err.Error(), http.StatusForbidden)
		return
	}
	info, err := os.Stat(target)
	if errors.Is(err, os.ErrNotExist) {
		http.NotFound(w, r)
		return
	}
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	if info.IsDir() {
		http.Error(w, "directory listing is not supported", http.StatusForbidden)
		return
	}
	http.ServeFile(w, r, target)
}

func (s *Server) handleCopy(w http.ResponseWriter, r *http.Request) {
	src, dst, existed, ok := s.copyMovePaths(w, r)
	if !ok {
		return
	}
	if err := copyPath(src, dst); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	writeCreatedOrNoContent(w, existed)
}

func (s *Server) handleMove(w http.ResponseWriter, r *http.Request) {
	src, dst, existed, ok := s.copyMovePaths(w, r)
	if !ok {
		return
	}
	if err := os.Rename(src, dst); err != nil {
		if err := copyPath(src, dst); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}
		if err := os.RemoveAll(src); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}
	}
	writeCreatedOrNoContent(w, existed)
}

func (s *Server) copyMovePaths(w http.ResponseWriter, r *http.Request) (string, string, bool, bool) {
	src, err := s.resolveURLPath(r.URL.Path)
	if err != nil {
		http.Error(w, err.Error(), http.StatusForbidden)
		return "", "", false, false
	}
	if !exists(src) {
		http.NotFound(w, r)
		return "", "", false, false
	}

	dst, err := s.resolveDestination(r)
	if err != nil {
		status := http.StatusBadRequest
		if errors.Is(err, errForbidden) {
			status = http.StatusForbidden
		}
		http.Error(w, err.Error(), status)
		return "", "", false, false
	}
	if sameOrChild(dst, src) {
		http.Error(w, "destination is inside source", http.StatusForbidden)
		return "", "", false, false
	}
	if !exists(filepath.Dir(dst)) {
		http.Error(w, "destination parent does not exist", http.StatusConflict)
		return "", "", false, false
	}

	existed := exists(dst)
	if existed && strings.EqualFold(r.Header.Get("Overwrite"), "F") {
		http.Error(w, "destination already exists", http.StatusPreconditionFailed)
		return "", "", false, false
	}
	if existed {
		if err := os.RemoveAll(dst); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return "", "", false, false
		}
	}
	return src, dst, existed, true
}

func (s *Server) handleDelete(w http.ResponseWriter, r *http.Request) {
	target, err := s.resolveURLPath(r.URL.Path)
	if err != nil {
		http.Error(w, err.Error(), http.StatusForbidden)
		return
	}
	if !exists(target) {
		http.NotFound(w, r)
		return
	}
	if err := os.RemoveAll(target); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	w.WriteHeader(http.StatusNoContent)
}

var errForbidden = errors.New("forbidden destination")

func (s *Server) resolveDestination(r *http.Request) (string, error) {
	raw := r.Header.Get("Destination")
	if raw == "" {
		return "", errors.New("Destination header is required")
	}
	parsed, err := url.Parse(raw)
	if err != nil {
		return "", fmt.Errorf("invalid Destination header: %w", err)
	}
	if parsed.Host != "" && !sameHost(parsed.Host, r.Host) {
		return "", errForbidden
	}
	target, err := s.resolveURLPath(parsed.Path)
	if err != nil {
		return "", errForbidden
	}
	return target, nil
}

func (s *Server) resolveURLPath(urlPath string) (string, error) {
	decoded, err := url.PathUnescape(urlPath)
	if err != nil {
		return "", err
	}
	if strings.Contains(decoded, "\x00") {
		return "", errors.New("path contains null byte")
	}
	for _, part := range strings.Split(decoded, "/") {
		if part == ".." {
			return "", errors.New("path traversal is forbidden")
		}
	}

	cleaned := path.Clean("/" + decoded)
	root, err := filepath.Abs(s.root)
	if err != nil {
		return "", err
	}
	if err := os.MkdirAll(root, 0o755); err != nil {
		return "", err
	}

	target := filepath.Join(root, filepath.FromSlash(strings.TrimPrefix(cleaned, "/")))
	absTarget, err := filepath.Abs(target)
	if err != nil {
		return "", err
	}
	rel, err := filepath.Rel(root, absTarget)
	if err != nil {
		return "", err
	}
	if rel == ".." || strings.HasPrefix(rel, ".."+string(filepath.Separator)) || filepath.IsAbs(rel) {
		return "", errors.New("path escapes root")
	}
	return absTarget, nil
}

func copyPath(src, dst string) error {
	info, err := os.Stat(src)
	if err != nil {
		return err
	}
	if info.IsDir() {
		return copyDir(src, dst)
	}
	return copyFile(src, dst, info.Mode())
}

func copyDir(src, dst string) error {
	return filepath.WalkDir(src, func(current string, entry os.DirEntry, err error) error {
		if err != nil {
			return err
		}
		rel, err := filepath.Rel(src, current)
		if err != nil {
			return err
		}
		target := filepath.Join(dst, rel)
		info, err := entry.Info()
		if err != nil {
			return err
		}
		if entry.IsDir() {
			return os.MkdirAll(target, info.Mode())
		}
		return copyFile(current, target, info.Mode())
	})
}

func copyFile(src, dst string, mode os.FileMode) error {
	in, err := os.Open(src)
	if err != nil {
		return err
	}
	defer in.Close()

	out, err := os.OpenFile(dst, os.O_CREATE|os.O_WRONLY|os.O_TRUNC, mode)
	if err != nil {
		return err
	}
	defer out.Close()

	_, err = io.Copy(out, in)
	return err
}

func writeCreatedOrNoContent(w http.ResponseWriter, existed bool) {
	if existed {
		w.WriteHeader(http.StatusNoContent)
		return
	}
	w.WriteHeader(http.StatusCreated)
}

func exists(name string) bool {
	_, err := os.Stat(name)
	return err == nil
}

func sameHost(left, right string) bool {
	return strings.EqualFold(left, right)
}

func sameOrChild(candidate, parent string) bool {
	rel, err := filepath.Rel(parent, candidate)
	if err != nil {
		return false
	}
	return rel == "." || (!strings.HasPrefix(rel, ".."+string(filepath.Separator)) && rel != ".." && !filepath.IsAbs(rel))
}
