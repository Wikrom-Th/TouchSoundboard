# TouchSoundboard
 Play sound samples wirelessly with ESP32 via UDP connection

## Requirements
- WiFi connection
- [go](https://go.dev/) 1.20 or above
- [Arduino IDE](https://www.arduino.cc/en/software)
- [ESP32](https://www.espressif.com/en/products/socs/esp32) or equivalent
- [Touch Sensor(s)](https://www.google.com/search?q=Touch+Sensor+ttp223) or equivalent (any type of switch works actually)

## Setup
1. run `go get` to get all the requirements
2. rename [secrets_example.h](Arduino/touchUdp/secrets_example.h) in the Arduino folder to `secrets.h`
3. change `secrets.h` to your wifi credentials, server ip, etc.
4. connect your touch sensor/switch to ESP32, and change the pins in [touchUdp.ino](Arduino/touchUdp/touchUdp.ino) accordingly.
5. upload `touchUdp.ino` to your ESP32 (see [Upload a sketch in Arduino IDE](https://support.arduino.cc/hc/en-us/articles/4733418441116-Upload-a-sketch-in-Arduino-IDE) for more details).
6. either do `go run server.go` or `go build` & `.\touchSoundboard.exe` to run your server. 