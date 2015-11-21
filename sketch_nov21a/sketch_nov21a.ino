#include "LedControl.h"

#define TEMPERTURE  0x00
#define SRF05     0x01
#define GAME1   0x02
#define GAME2   0x03
#define GRAPH   0x04

#define BUTTON1   4
#define BUTTON2   5
#define BUTTON3   6
#define BUTTON4   7

LedControl lc=LedControl(12,11,10,1);

const uint8_t number[10][5] = {
  {B11100000, B10100000, B10100000, B10100000, B11100000}, // 0
  {B00100000, B00100000, B00100000, B00100000, B00100000}, // 1
  {B11100000, B00100000, B11100000, B10000000, B11100000}, // 2
  {B11100000, B00100000, B11100000, B00100000, B11100000}, // 3
  {B10100000, B10100000, B11100000, B00100000, B00100000}, // 4
  {B11100000, B10000000, B11100000, B00100000, B11100000}, // 5
  {B11100000, B10000000, B11100000, B10100000, B11100000}, // 6
  {B11100000, B00100000, B00100000, B00100000, B00100000}, // 7
  {B11100000, B10100000, B11100000, B10100000, B11100000}, // 8
  {B11100000, B10100000, B11100000, B00100000, B11100000}, // 9
};

void draw(uint8_t *frame, uint8_t numRow) {
  for(uint8_t i = 0; i < numRow; i++) {
      lc.setRow(0, i, frame[i]);
  }
}

void draw(const uint8_t *frame, uint8_t numRow, uint8_t x, uint8_t y) {
  uint8_t newFrame[numRow];
  for(uint8_t i = 0; i < numRow; i++) {
    newFrame[i] = frame[i] >> x;
  }
  for(uint8_t j = 0; j < numRow; j++) {
    lc.setRow(0, j + y, newFrame[j]);
  }
}

volatile uint8_t currentState;

void setup() {
  lc.shutdown(0,false);  // Wake up displays
  lc.setIntensity(0,0);  // Set intensity levels
  lc.clearDisplay(0);  // Clear Displays

  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  
  attachInterrupt(0, isr, FALLING);
}

void loop() {
  switch(currentState) {
    case TEMPERTURE: {
      do {
        for(uint8_t i = 0; i < 10; i ++) {
          draw(number[i], 5, 1, 1);
          delay(500);
        }
      } while(currentState == TEMPERTURE);
      break;
    }
    case SRF05: {
      do {
        digitalWrite(13, 0);
      } while(currentState == SRF05);
      break;
    }
    case GAME1: {
      do {
        digitalWrite(13, 1);
        delay(500);
        digitalWrite(13, 0);
        delay(500);
      } while(currentState == GAME1);
      break;
    }
    case GAME2: {
      do {
        digitalWrite(13, 1);
        delay(100);
        digitalWrite(13, 0);
        delay(700);
      } while(currentState == GAME2);
      break;
    }
    case GRAPH: {
      do {
        digitalWrite(13, 1);
        delay(90);
        digitalWrite(13, 0);
        delay(90);
      } while(currentState == GRAPH);
      break;
    }
  }
}

void isr() {
  delay(10);
  if(digitalRead(BUTTON1) == 0) {
    currentState++;
    if(currentState > GRAPH) {
      currentState = TEMPERTURE;
    }
  } else if(digitalRead(BUTTON2) == 0) {
    
  } else if(digitalRead(BUTTON3) == 0) {
    
  } else {
    
  }
  while(digitalRead(BUTTON1) == 0);
}


