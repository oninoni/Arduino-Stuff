#include <EEPROM.h>

#define relay_1 A0
#define relay_2 A1
#define relay_3 A2

#define btn_1 2
#define btn_2 3
#define btn_3 4
#define btn_4 5

#define led_1 10
#define led_2 11
#define led_3 12
#define led_4 13

// State Machine with 3 States for first 3 Buttons
uint8_t main_State = 0;

// Secondary State for 4th Button
uint8_t secondary_State = 1;
// Secondary State Entprellen
uint8_t secondary_Timer = 0;
uint8_t secondary_button_State = 0;

void boot_Animation(){
  digitalWrite(led_1, HIGH);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
  delay(100);
  
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, HIGH);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
  delay(100);
  
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, HIGH);
  digitalWrite(led_4, LOW);
  delay(100);
  
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, HIGH);
  delay(100);
  
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, HIGH);
  digitalWrite(led_4, LOW);
  delay(100);
  
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, HIGH);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
  delay(100);
  
  digitalWrite(led_1, HIGH);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
  delay(100);
}

void set_Outputs(){
  switch(main_State){
    case 0:
      digitalWrite(led_1, HIGH);
      digitalWrite(led_2, LOW);
      digitalWrite(led_3, LOW);

      digitalWrite(relay_1, LOW);
      digitalWrite(relay_2, LOW);
      break;
    case 1:
      digitalWrite(led_1, LOW);
      digitalWrite(led_2, HIGH);
      digitalWrite(led_3, LOW);

      digitalWrite(relay_1, HIGH);
      digitalWrite(relay_2, HIGH);
      break;
    case 2:
      digitalWrite(led_1, LOW);
      digitalWrite(led_2, LOW);
      digitalWrite(led_3, HIGH);

      digitalWrite(relay_1, HIGH);
      digitalWrite(relay_2, LOW);
      break;
  }
  
  if(secondary_State == 0){
    digitalWrite(led_4, LOW);
    
    digitalWrite(relay_3, LOW);
  }else{
    digitalWrite(led_4, HIGH);
    
    digitalWrite(relay_3, HIGH);
  }
}

void load_Data(){
  uint8_t main_State_Load = EEPROM.read(0);
  uint8_t secondary_State_Load = EEPROM.read(1);

  if(main_State_Load == 0 || main_State_Load == 1 || main_State_Load == 2){
    main_State = main_State_Load;
  }else{
    main_State = 0;
  }

  if(secondary_State_Load == 0 || secondary_State_Load == 1){
    secondary_State = secondary_State_Load;
  }else{
    secondary_State = 0;
  }
}

void save_Data(){
  EEPROM.write(0, main_State);
  EEPROM.write(1, secondary_State);
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
  
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  
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
  }

  if(secondary_Timer == 0){
    if(digitalRead(btn_4) == 1 && secondary_button_State == 1){
      secondary_button_State = 0;
    }

    if(digitalRead(btn_4) == 0 && secondary_button_State == 0){
      secondary_button_State = 1;
      secondary_Timer = 10; // Button can be pressed 10 times a sec ( 10 * 10 ms == 100 ms / 10Hz)
      secondary_State = (secondary_State + 1) % 2;
      save_Data();
    }
  }else{
    secondary_Timer--;
  }

  delay(10);

  set_Outputs();
}
