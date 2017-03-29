#include <SevSeg.h>

SevSeg sevSeg;

void setup() {
  // put your setup code here, to run once:
  byte numDigits = 3;
  byte digitPins[] = {12, 11, 10};
  byte segmentPins[] = {9, 8, 7, 6, 5, 4, 3, 2};
  sevSeg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevSeg.setBrightness(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  sevSeg.setNumber(112,2);
  sevSeg.refreshDisplay();
}
