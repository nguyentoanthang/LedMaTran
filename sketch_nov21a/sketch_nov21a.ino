#include "Lib.h"
#include <EEPROM.h>

extern Led lc;
volatile uint8_t currentState;
volatile int currentTemp;
volatile uint8_t index;
long a, b;
uint8_t c, d;
volatile boolean isRight;
volatile boolean isWait;
volatile uint8_t score;
volatile int count;
volatile int countTemp;
volatile boolean led13;
volatile uint8_t tempWarning;
volatile boolean isGame;
volatile boolean isEnable;
volatile boolean isDoxingau;
uint16_t temp;
unsigned long times;
uint16_t distance;
uint8_t highscore;

void setup() {
  /* Reset Timer/Counter1 */
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  Serial.begin(9600);
  Init();
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(9, INPUT);
  pinMode(8, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);

  led13 = 0;
  highscore = EEPROM.read(11);
  isEnable = false;
  temp = 0;
  countTemp = 0;
  isWait = true;
  count = 0;
  score = 0;
  index = 0;
  isRight = true;
  digitalWrite(13, led13);
  isGame = true;
  currentState = 0x00;
  // setup extern interrupt
  EICRA |= (1 << ISC01);
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
      isEnable = false;
      do {
          temp = (uint16_t)(round(currentTemp*5.0*10000.0/1024.0));
          draw(temp);
      } while(currentState == TEMPERTURE);
      break;
    }
    case SRF05: {
      do {
          if(isDoxingau) {
            for(uint8_t i = 1; i < 10; i++) {
              c = random(7);
              d = random(7);
              disp(c, d);
              delay(i*200);
              isDoxingau = false;
            }
          } else {
            while(isDoxingau == false);
          }
          
      } while(currentState == SRF05);
      break;
    }
    case GAME1: {
      isGame = true;
      isEnable = true;
      score = 0;
      do {
        if(isRight) {
          TCNT2 = 0;
          TCCR2B &= ~(1 << CS22);   // tat timer 2
          count = 0;
          a = random(100);
          b = random(100);
          dispGame((uint8_t)a, (uint8_t)b);
          //bip();
          isWait = true;
          score++;
          TCCR2B |= (1 << CS22);  // prescale 64, bat timer 2
          while(isWait == true);
        } else {
          TCCR2B &= ~(1 << CS22);
           //bip1();
           //EEPROM.update(0, score);
           count = 0;
           isRight = true;
           if(score > highscore) {
              draw(score*100);
              delay(300);
              lc.shutdown(0, true);
              delay(300);
              lc.shutdown(0, false);
              delay(300);
              lc.shutdown(0, true);
              delay(300);
              lc.shutdown(0, false);
              delay(300);
              lc.shutdown(0, true);
              delay(300);
              lc.shutdown(0, false);
              highscore = score;
              EEPROM.write(11, highscore);
              score = 0;
           } else {
            draw(loseIcon, 8);
            score = 0;
           }
           delay(2000);
        }
      } while(currentState == GAME1);
      isRight = true;
      count = 0;
      TCCR2B &= ~(1 << CS22);  // tat timer 2
      
      break;
    }
    case GAME2: {
      isEnable = false;
      do {
        draw(highscore*100);
      } while(currentState == GAME2);
      break;
    }
    case SETTING: {
      isEnable = true;
      isGame = false;
      tempWarning = EEPROM.read(10);
      do {
        draw(tempWarning*100);
      } while(currentState == SETTING);   
      break;
    }
    case WARNING: {
      do {
        uint8_t frame[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        draw(frame, 8);
        //bip();
      } while(currentState == WARNING);
    }
    break;
  }
}

ISR(INT0_vect) {
  delay(10);
  if(digitalRead(BUTTON1) == 0) {
    currentState++;
    if(currentState > GAME2) {
      currentState = TEMPERTURE;
    }
    while(digitalRead(BUTTON1) == 0);
  } else if(digitalRead(BUTTON2) == 0) {
      if(isEnable) {
        if(isGame) {
          isWait = false;
          if(a >= b) {
            isRight = true;
          } else {
            isRight = false;
          }
        } else {
          tempWarning++;
          EEPROM.update(10, tempWarning);
        }
      }
      while(digitalRead(BUTTON2) == 0);
  } else if(digitalRead(BUTTON3) == 0) {
    if(isEnable) {
        if(isGame) {
          isWait = false;
          if(b >= a) {
            isRight = true;  
          } else {
            isRight = false;
          }
        } else {
        tempWarning--;
        EEPROM.update(10, tempWarning);
      }
    }
    while(digitalRead(BUTTON3) == 0);
  } else if(digitalRead(BUTTON4) == 0){
    if(currentState == SRF05) {
      isDoxingau == true;
    } else {
      currentState = SETTING;
    }
    while(digitalRead(BUTTON4) == 0);
  }
  
}

ISR(TIMER1_OVF_vect) {
  if(countTemp++ == 10) {
    currentTemp = analogRead(A0);
    countTemp = 0;
    tempWarning = EEPROM.read(10);
    if(tempWarning*2.048 < currentTemp) {
      currentState = WARNING;
    }
  }
}

ISR(TIMER2_OVF_vect) {
  if(count++ == 2000) {
    isRight = false;
    isWait = false;
  }
}

