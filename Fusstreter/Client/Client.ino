#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char ssid[] = "Fusstreter001";
const char password[] = "42FTR001";

unsigned int localPort = 42;
unsigned long lastBlink;

char packetBuffer[2];
char replyBuffer[1];

WiFiUDP udp;

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  
  lastBlink = millis();
  replyBuffer[0] = 0;
  
  WiFi.softAP(ssid, password);

  udp.begin(localPort);
}

void loop() {
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
  }
  if(lastBlink + 1000 < millis()){
    lastBlink = millis();
    if(replyBuffer[0] == 0){
      replyBuffer[0] = 1;
      digitalWrite(2, HIGH);
    }else{
      replyBuffer[0] = 0;
      digitalWrite(2, LOW);
    }
    
    udp.beginPacket(udp.remoteIP(), 42);
    udp.write(replyBuffer);
    udp.endPacket();
  }
}
