package main

import (
	"log"
	"time"

	"github.com/gorilla/websocket"
)

func buildMessages() []string {
	return []string{
		"message 1",
		"message 2",
		"message 3",
		"message 4",
		"message 5",
	}
}

func main() {
	conn, _, err := websocket.DefaultDialer.Dial("ws://localhost:3000/ws", nil)
	if err != nil {
		log.Fatalf("dial error: %v", err)
	}
	defer conn.Close()

	for _, message := range buildMessages() {
		log.Printf("send: %s", message)
		if err := conn.WriteMessage(websocket.TextMessage, []byte(message)); err != nil {
			log.Fatalf("write error: %v", err)
		}

		_, response, err := conn.ReadMessage()
		if err != nil {
			log.Fatalf("read error: %v", err)
		}
		log.Printf("received: %s", string(response))

		time.Sleep(time.Second)
	}

	if err := conn.WriteMessage(websocket.CloseMessage, websocket.FormatCloseMessage(websocket.CloseNormalClosure, "bye")); err != nil {
		log.Printf("close write error: %v", err)
	}
}
