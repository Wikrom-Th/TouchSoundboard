package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"time"

	"github.com/faiface/beep/speaker"
	"github.com/faiface/beep/wav"
)

func play_sound(sample string) {
	f, err := os.Open("./samples/" + sample)
	if err != nil {
		log.Fatal(err)
	}

	streamer, format, err := wav.Decode(f)
	if err != nil {
		log.Fatal(err)
	}
	defer streamer.Close()

	speaker.Init(format.SampleRate, format.SampleRate.N(time.Second/10))
	speaker.Play(streamer)
}

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
