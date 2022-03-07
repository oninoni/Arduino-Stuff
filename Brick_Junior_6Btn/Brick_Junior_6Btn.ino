#include <EEPROM.h>

#define relay_1 2  // XLR 3 (Clean/Overdrive)
#define relay_2 3  // XLR 4 (Crunch/Drive)
#define relay_3 A3 // XLR 5 (Rythm/Lead)

#define btn_1 9
#define btn_2 8
#define btn_3 7
#define btn_4 6
#define btn_5 5
#define btn_6 4

#define led_1 10
#define led_2 11
#define led_3 12
#define led_4 13
#define led_5 A0
#define led_6 A1
uint8_t leds[] = {
  led_1,
  led_2,
  led_3,
  led_4,
  led_5,
  led_6,
};
#define LED_COUNT 6

// State Machine with 3 States for first 3 Buttons
uint8_t main_State = 0;

void singleLED(uint8_t led){
  for(uint8_t i = 0; i < LED_COUNT; i++){
    if(led == i){
      digitalWrite(leds[i], HIGH);
    }else{
      digitalWrite(leds[i], LOW);
    }
  }
}

void boot_Animation(){
  for(uint8_t i = 0; i < LED_COUNT; i++){
    singleLED(i);
    delay(100);
  }

  singleLED(LED_COUNT);
  delay(100);

  for(uint8_t i = LED_COUNT - 1; i >= 0; i++){
    singleLED(i);
    delay(100);
  }
  
  singleLED(LED_COUNT);
  delay(100);
}

void set_Outputs(){
  singleLED(main_State);
  
  switch(main_State % 3){
    case 0:
      digitalWrite(relay_1, LOW);
      digitalWrite(relay_2, LOW);
      break;
    case 1:
      digitalWrite(relay_1, HIGH);
      digitalWrite(relay_2, LOW);
      break;
    case 2:
      digitalWrite(relay_1, HIGH);
      digitalWrite(relay_2, HIGH);
      break;
  }
  
  if(main_State >= 3){
    digitalWrite(relay_3, LOW);
  }else{
    digitalWrite(relay_3, HIGH);
  }
}

void load_Data(){
  uint8_t main_State_Load = EEPROM.read(0);
  
  if(main_State_Load >= 0 && main_State_Load <= 5){
    main_State = main_State_Load;
  }else{
    main_State = 0;
  }
}

void save_Data(){
  EEPROM.write(0, main_State);
}

void setup() {
  pinMode(relay_1, OUTPUT);
  digitalWrite(relay_1, HIGH);
  pinMode(relay_2, OUTPUT);
  digitalWrite(relay_2, HIGH);
  pinMode(relay_3, OUTPUT);
  digitalWrite(relay_3, HIGH);
  
  pinMode(btn_1, INPUT_PULLUP);
  pinMode(btn_2, INPUT_PULLUP);
  pinMode(btn_3, INPUT_PULLUP);
  pinMode(btn_4, INPUT_PULLUP);
  pinMode(btn_5, INPUT_PULLUP);
  pinMode(btn_6, INPUT_PULLUP);
  
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  pinMode(led_5, OUTPUT);
  pinMode(led_6, OUTPUT);
  
  load_Data();
  set_Outputs();
  
  boot_Animation();
  
  Serial.begin(9600);
}

void loop() {
  if(digitalRead(btn_1) == 0){
    main_State = 0;
    save_Data();
  }else if(digitalRead(btn_2) == 0){
    main_State = 1;
    save_Data();
  }else if(digitalRead(btn_3) == 0){
    main_State = 2;
    save_Data();
  }else if(digitalRead(btn_4) == 0){
    main_State = 3;
    save_Data();
  }else if(digitalRead(btn_5) == 0){
    main_State = 4;
    save_Data();
  }else if(digitalRead(btn_6) == 0){
    main_State = 5;
    save_Data();
  }

  delay(10);

  set_Outputs();
}
