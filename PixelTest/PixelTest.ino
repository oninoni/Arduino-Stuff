#include <Time.h>
#include <TimeLib.h>

#include "color.h"
#include <Adafruit_NeoPixel.h>

#define PIN 3
#define PIXEL_COUNT 392

#define INIT_SPEED 30

#define DURATION_FADE 50

#define STATE_INIT -1
#define STATE_IDLE  0
#define STATE_FADE  1

#define CMD_OFF "OFF"

#define CMD_ONR "ONR"
#define CMD_ONG "ONG"
#define CMD_ONB "ONB"

#define CMD_ONP "ONP"
#define CMD_ONY "ONY"
#define CMD_ONC "ONC"

#define CMD_ONO "ONO"

#define CMD_ONW "ONW"

#define CMD_RAN "RAN"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIN, NEO_GRB + NEO_KHZ800);

typedef unsigned int uint;
typedef unsigned long ulong;

ulong timeOld;

// Color Constants
const color COLOR_BLACK  = color(0  , 0  , 0  );
const color COLOR_RED    = color(255, 0  , 0  );
const color COLOR_GREEN  = color(0  , 255, 0  );
const color COLOR_BLUE   = color(0  , 0  , 255);
const color COLOR_PURPLE = color(255, 0  , 255);
const color COLOR_YELLOW = color(255, 255, 0  );
const color COLOR_CYAN   = color(0  , 255, 255);
const color COLOR_WHITE  = color(255, 214, 170);

const color COLOR_ORANGE = color(255, 127, 0  );

// LED Buffer for Animations
color sourceColor[PIXEL_COUNT];
color targetColor[PIXEL_COUNT];

// Single Target Buffer for Speed
color targetColorUniform;
bool useUniform = false;

// Identifies active State
short stateID = STATE_INIT;

// Counter to control active State
uint counter = 0;

// Origin of last State Change (Position of button , etc...)
uint origin = 0;

// LED HANDLING/UTIL

uint timeLastSecond = millis();
uint ops = 0;

uint lastSecond = 0;

void fadeLED(uint n, color c){
  if(counter == 0){
    sourceColor[n] = strip.getPixelColor(n);
  }
  
  float multiplier = (1.0f + counter) / DURATION_FADE;
  
  color newColor = (1.0f - multiplier) * sourceColor[n] + multiplier * c;
  strip.setPixelColor(n, colorToInt(newColor));
}

void setAll(){
  for(uint i = 0; i < PIXEL_COUNT; i++){
    sourceColor[i] = strip.getPixelColor(i);
    if(useUniform){
      strip.setPixelColor(i,colorToInt(targetColorUniform));
    }else{
      strip.setPixelColor(i,colorToInt(targetColor[i]));
    }
  }
}

void fadeAll(){
  if(counter >= DURATION_FADE)return;
  for(uint i = 0; i < PIXEL_COUNT; i++){
    if(useUniform){
      fadeLED(i, targetColorUniform);
    }else{
      fadeLED(i, targetColor[i]);
    }
  }
}

void setTargetRandom(){
  useUniform = false;
  for(uint i = 0; i < PIXEL_COUNT; i++){
    targetColor[i] = random(0x1000000);
  }
}

void setTarget(color c){
  useUniform = true;
  targetColorUniform = c;
}

void setSource(){
  for(uint i = 0; i < PIXEL_COUNT; i++){
    sourceColor[i] = strip.getPixelColor(i);
  }
}

// STATE HANDLING

void changeState(short s, uint pos = 0){
  origin = pos;
  stateID = s;
  counter = 0;
}

void handleActiveState(){
  switch(stateID){
    case STATE_INIT:
      if(counter == 0){
        setTarget(COLOR_RED);
        setAll();
      }else if(counter == INIT_SPEED){
        setTarget(COLOR_BLUE);
        setAll();
      }else if(counter == INIT_SPEED * 2){
        setTarget(COLOR_GREEN);
        setAll();
      }else if(counter == INIT_SPEED * 3){
        setTarget(COLOR_WHITE);
        setAll();
        changeState(STATE_IDLE);
        break;
      }
      counter++;
      break;
    case STATE_IDLE:
      if(counter == 0){
        counter = 1;
        setSource();
      }
      if(Serial.available()){
        String serialInput = Serial.readStringUntil('\n');
        Serial.println(serialInput);
        if(serialInput == CMD_RAN){
          setTargetRandom();
          changeState(STATE_FADE);
        }else if(serialInput == CMD_OFF){
          setTarget(COLOR_BLACK);
          changeState(STATE_FADE);
        }else if(serialInput == CMD_ONW){
          setTarget(COLOR_WHITE);
          changeState(STATE_FADE);
        }else if(serialInput == CMD_ONR){
          setTarget(COLOR_RED);
          changeState(STATE_FADE);
        }else if(serialInput == CMD_ONG){
          setTarget(COLOR_GREEN);
          changeState(STATE_FADE);
        }else if(serialInput == CMD_ONB){
          setTarget(COLOR_BLUE);
          changeState(STATE_FADE);
        }else if(serialInput == CMD_ONP){
          setTarget(COLOR_PURPLE);
          changeState(STATE_FADE);
        }else if(serialInput == CMD_ONY){
          setTarget(COLOR_YELLOW);
          changeState(STATE_FADE);
        }else if(serialInput == CMD_ONC){
          setTarget(COLOR_CYAN);
          changeState(STATE_FADE);
        }else if(serialInput == CMD_ONO){
          setTarget(COLOR_ORANGE);
          changeState(STATE_FADE);
        }
      }
      break;
    case STATE_FADE:
      fadeAll();
      if(++counter == DURATION_FADE){
        changeState(STATE_IDLE);
      }
      break;
  }
}

void initLight(){
  strip.begin();
  strip.show();
  changeState(STATE_INIT);
}

void setup() {
  Serial.begin(9600);
  initLight();
  timeOld = micros();
}

void loop() {
  handleActiveState();
  delayMicroseconds(10000 - (micros() - timeOld) - (PIXEL_COUNT * 1.3));
  timeOld = micros();
  ops++;
  if((uint) second() != lastSecond){
    lastSecond = second();
    ops = 0;
    timeLastSecond = millis();
  }
  strip.show();
}
