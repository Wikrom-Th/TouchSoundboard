#include <WiFi.h>
#include <WiFiUdp.h>
#include "secrets.h"

const int TOUCH_PIN_COUNTS = 3;
const int TOUCH_PIN_ARR[TOUCH_PIN_COUNTS] = {5, 14, 34}; //change pins as needed

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

const char* addr = SERVER_ADDR;
const int port = SERVER_PORT;

//create UDP instance
WiFiUDP udp;

/*
 * current and previous output is sent as int of the addition (bitwise or) of the array's index (in LSB order)
 * examples: 
 * touching the 3 touch sensors altogether: output = 7 in decimal (111 in binary) 
 * touching only the 2nd and the 3rd sensors: output = 6 in decimal (110 in binary)
 */

int output = 0; 
int prev_output = 0;

//query to send to the server
int play_query = 0;

//previous reading of the pin
int prev[TOUCH_PIN_COUNTS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for(int i = 0; i < TOUCH_PIN_COUNTS; i++) {
    pinMode(TOUCH_PIN_ARR[i], INPUT);
    prev[i] = LOW;
  }

  output = 0;

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  delay(2000); //delay before program starts
  Serial.println("Initialization Completed");
}

void loop() {
  for(int i = 0; i < TOUCH_PIN_COUNTS; i++) {
    int curr = digitalRead(TOUCH_PIN_ARR[i]); // current reading of the pin

    if((curr == HIGH) && (prev[i] == LOW)) {
      output += (int) round(pow(2,i));
      prev[i] = HIGH;
    }
    else if((curr == LOW) && (prev[i] == HIGH)){
      output -= (int) round(pow(2,i));
      prev[i] = LOW;
    }
  }

  if(output != prev_output) {
    play_query = (output ^ prev_output) & output;
    Serial.print(output);
    Serial.print(" ");
    Serial.println(prev_output);
    Serial.print(" ");
    Serial.println(play_query);

    if((WiFi.status()== WL_CONNECTED) && play_query){
      //send play_query
      uint8_t buf[1] = {byte(round(log2(play_query)))};
      udp.beginPacket(addr, port);
      udp.write(buf, 1);
      memset(buf, 0, 1);
      udp.endPacket();
    }
    prev_output = output;
  }
}