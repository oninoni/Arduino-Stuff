#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("AN!");
  delay(1000);
  Serial.println("AUS!");
  delay(1000);
}
