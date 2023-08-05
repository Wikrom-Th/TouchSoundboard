package main

import (
	"fmt"
	"log"
	"net"
)

func main() {
	udpServer, err := net.ListenPacket("udp", ":12345")
	if err != nil {
		log.Fatal(err)
	}
	defer udpServer.Close()

	for {
		buf := make([]byte, 1)
		_, _, err := udpServer.ReadFrom(buf)
		if err != nil {
			continue
		}
		fmt.Println(buf[0])
	}
}
