package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"time"

	"github.com/faiface/beep"
	"github.com/faiface/beep/speaker"
	"github.com/faiface/beep/wav"
)

func play_sound(sample string) {
	f, err := os.Open("./samples/" + sample)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(f)

	streamer, format, err := wav.Decode(f)
	if err != nil {
		log.Fatal(err)
	}
	defer streamer.Close()

	speaker.Init(format.SampleRate, format.SampleRate.N(time.Second/10))

	done := make(chan bool)
	speaker.Play(beep.Seq(streamer, beep.Callback(func() {
		done <- true
	})))

	<-done
}

func main() {
	samples := [...]string{"kick1.wav", "snare1.wav", "clap1.wav"}
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
		go play_sound(samples[buf[0]])
	}
}
