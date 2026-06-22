package main

import (
	"flag"
	"fmt"
	"log"
	"os"

	"go09_01c/davclient"
)

func main() {
	base := flag.String("url", "http://localhost:3000", "WebDAV server URL")
	user := flag.String("user", "", "optional Basic Auth user")
	pass := flag.String("pass", "", "optional Basic Auth password")
	flag.Parse()

	client := davclient.New(*base, *user, *pass)
	args := flag.Args()
	if len(args) == 0 {
		usage()
		os.Exit(2)
	}

	var err error
	switch args[0] {
	case "mkcol":
		if len(args) != 2 {
			usage()
			os.Exit(2)
		}
		err = client.Mkcol(args[1])
	case "put":
		if len(args) != 3 {
			usage()
			os.Exit(2)
		}
		err = client.Put(args[1], args[2])
	case "get":
		if len(args) != 3 {
			usage()
			os.Exit(2)
		}
		err = client.Get(args[1], args[2])
	case "copy":
		if len(args) != 3 {
			usage()
			os.Exit(2)
		}
		err = client.Copy(args[1], args[2])
	case "move":
		if len(args) != 3 {
			usage()
			os.Exit(2)
		}
		err = client.Move(args[1], args[2])
	case "delete":
		if len(args) != 2 {
			usage()
			os.Exit(2)
		}
		err = client.Delete(args[1])
	default:
		usage()
		os.Exit(2)
	}
	if err != nil {
		log.Fatal(err)
	}
}

func usage() {
	fmt.Fprintln(os.Stderr, "usage:")
	fmt.Fprintln(os.Stderr, "  GO09_01c -url http://localhost:3000 [-user user -pass pass] mkcol /dir")
	fmt.Fprintln(os.Stderr, "  GO09_01c -url http://localhost:3000 [-user user -pass pass] put local.txt /dir/file.txt")
	fmt.Fprintln(os.Stderr, "  GO09_01c -url http://localhost:3000 [-user user -pass pass] get /dir/file.txt out.txt")
	fmt.Fprintln(os.Stderr, "  GO09_01c -url http://localhost:3000 [-user user -pass pass] copy /from.txt /to.txt")
	fmt.Fprintln(os.Stderr, "  GO09_01c -url http://localhost:3000 [-user user -pass pass] move /from.txt /to.txt")
	fmt.Fprintln(os.Stderr, "  GO09_01c -url http://localhost:3000 [-user user -pass pass] delete /path")
}
