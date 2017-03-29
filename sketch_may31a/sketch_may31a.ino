void setup() {
  pinMode(5, OUTPUT);
}

void loop() {
  delayMicroseconds(500);
  if(random(2) == 0){
    digitalWrite(5, HIGH);
  }else{
    digitalWrite(5, LOW);
  }
}
