package main

import (
	"log"
	"net/http"

	"github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool {
		return true
	},
}

func buildServerMessage(message string) string {
	return "from server " + message
}

func wsHandler(w http.ResponseWriter, r *http.Request) {
	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Printf("upgrade error: %v", err)
		return
	}
	defer conn.Close()

	log.Printf("client connected: %s", r.RemoteAddr)

	for {
		messageType, message, err := conn.ReadMessage()
		if err != nil {
			log.Printf("client disconnected: %v", err)
			return
		}

		text := string(message)
		log.Printf("received: %s", text)

		response := buildServerMessage(text)
		if err := conn.WriteMessage(messageType, []byte(response)); err != nil {
			log.Printf("write error: %v", err)
			return
		}
	}
}

func main() {
	http.HandleFunc("/ws", wsHandler)

	log.Printf("GO08_01s listening on :3000")
	if err := http.ListenAndServe(":3000", nil); err != nil {
		log.Fatalf("server stopped: %v", err)
	}
}
