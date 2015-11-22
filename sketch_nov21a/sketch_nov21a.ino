#include "Lib.h"

extern LedControl lc;
extern volatile uint8_t currentState;

void setup() {

  EICRA |= (1 << ISC01);
  EICRA |= (0 << ISC00);
  EIMSK |= (1 << INT0);
  //sei();
  lc.shutdown(0,false);  // Wake up displays
  lc.setIntensity(0,0);  // Set intensity levels
  lc.clearDisplay(0);  // Clear Displays

  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  
  currentState = 0x00;
}

void loop() {
  switch(currentState) {
    case TEMPERTURE: {
      do {
        for(uint8_t i = 0; i < 10; i ++) {
          draw(5678 );
        }
      } while(currentState == TEMPERTURE);
      break;
    }
    case SRF05: {
      do {
        //digitalWrite(13, 0);
        lc.shutdown(0, true);
      } while(currentState == SRF05);
      break;
    }
    case GAME1: {
      do {
        lc.shutdown(0, false);
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

ISR(INT0_vect) {
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


