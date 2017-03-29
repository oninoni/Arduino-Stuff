#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char ssid[] = "Fusstreter001";
const char password[] = "42FTR001";

unsigned int localPort = 42;

char packetBuffer[2];
char replyBuffer[1];

WiFiUDP udp;

void setup() {
  Serial.begin(9600);
  
  pinMode(2, OUTPUT);
  
  WiFi.softAP(ssid, password);

  int state = udp.begin(localPort);

  Serial.println(state);
  
}

void loop() {
  /**
  int packetSize = udp.parsePacket();
  if(packetSize){
    int len = udp.read(packetBuffer, 1);
    if(len > 0){
      packetBuffer[len] = 0;
    }
    if(packetBuffer[0] == 0){
      digitalWrite(2, LOW);
    }else{
      digitalWrite(2, HIGH);
    }
    replyBuffer[0] = packetBuffer[0];
    udp.beginPacket(udp.remoteIP(), 42);
    udp.write(replyBuffer);
    udp.endPacket();
  }
  */
}
