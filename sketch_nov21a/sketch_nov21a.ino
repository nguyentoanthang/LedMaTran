#include "Lib.h"
#include <EEPROM.h>

extern Led lc;
volatile uint8_t currentState;
volatile uint8_t tempArr[8];
volatile uint8_t index;
long a, b;
volatile boolean isRight;
volatile boolean isWait;
volatile uint8_t score;
volatile int count;

void setup() {
  /* Reset Timer/Counter1 */
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;

  Init();
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);

  isWait = true;
  count = 0;
  score = 0;
  index = 0;
  isRight = true;
  
  currentState = 0x00;
  
  EICRA |= (1 << ISC01);
  EICRA |= (0 << ISC00);
  EIMSK |= (1 << INT0);
  sei();
  // setup timer 1
  TCNT1 = 0;  // init timer
  TIMSK1 |= (1 << TOIE1); // enable interrupt
  TCCR1B |= (1 << CS11) | (1 << CS10); // prescale = 64

  // reset timer 2
  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 = 0;
  //setup timer 2
  
  TCNT2 = 0;
  TIMSK2 |= (1 << TOIE2);
} 

void loop() {
  switch(currentState) {
    case TEMPERTURE: {
      do {
          draw(loseIcon, 8);
      } while(currentState == TEMPERTURE);
      break;
    }
    case SRF05: {
      do {
        //digitalWrite(13, 0);
        //lc.shutdown(0, true);
      } while(currentState == SRF05);
      break;
    }
    case GAME1: {
      do {
        if(isRight) {
          TCNT2 = 0;
          TCCR2B &= ~(1 << CS22);
          count = 0;
          a = random(100);
          b = random(100);
          dispGame((uint8_t)a, (uint8_t)b);
          //bip();
          isWait = true;
          score++;
          TCCR2B |= (1 << CS22);  // prescale 64
          while(isWait == true);
        } else {
           //bip1();
           //EEPROM.update(0, score);
           count = 0;
           draw(loseIcon, 8);
           isRight = true;
           delay(2000);
        }
      } while(currentState == GAME1);
      isRight = true;
      count = 0;
      TCCR2B &= ~(1 << CS22);  // tat timer
      break;
    }
    case GAME2: {
      do {
        draw(2845);
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
    case POWER_DOWN: {
      
    }
  }
}

void calculateTemp() {  
  uint8_t currentTemp = analogRead(A0)/4;

  tempArr[index] = currentTemp;
  
  if(index == 8) {
    index = 0;
  } else {
    index++;
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
      isWait = false;
      if(a >= b) {
        isRight = true;
      } else {
        isRight = false;
      }
  } else if(digitalRead(BUTTON3) == 0) {
    isWait = false;
    if(b >= a) {
      isRight = true;  
    } else {
      isRight = false;
    }
  } else {
    
  }
  while(digitalRead(BUTTON1) == 0);
}

ISR(TIMER1_OVF_vect) {
  //calculateTemp();
}

ISR(TIMER2_OVF_vect) {
  if(count++ == 2000) {
    isRight = false;
    isWait = false;
  }
}

