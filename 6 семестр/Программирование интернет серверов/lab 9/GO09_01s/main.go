package main

import (
	"flag"
	"log"
	"net/http"

	"go09_01s/davserver"
)

func main() {
	addr := flag.String("addr", ":3000", "HTTP listen address")
	root := flag.String("root", "webdav-root", "directory used as WebDAV storage")
	user := flag.String("user", "", "optional Basic Auth user")
	pass := flag.String("pass", "", "optional Basic Auth password")
	flag.Parse()

	handler := davserver.New(davserver.Options{
		Root: *root,
		User: *user,
		Pass: *pass,
	})

	log.Printf("GO09_01s WebDAV server listening on %s, root=%s", *addr, *root)
	if err := http.ListenAndServe(*addr, handler); err != nil {
		log.Fatalf("server stopped: %v", err)
	}
}
