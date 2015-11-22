#include "Lib.h"

Led lc = Led(12,11,10,1);
volatile uint8_t currentState;

void Init() {
  lc.shutdown(0,false);  // Wake up displays
  lc.setIntensity(0,0);  // Set intensity levels
  lc.clearDisplay(0);  // Clear Displays
}

void draw(const uint8_t *frame, uint8_t numRow) {
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

void draw(uint16_t number) {
  uint8_t donvi = number%10;
  uint8_t chuc 	= (number%100)/10;
  uint8_t tram 	= (number%1000)/100;
  uint8_t nghin = number/1000;
  uint8_t newFrame[8];
  for(uint8_t i = 0; i < 5; i++) {
    newFrame[i] = numberL[nghin][i] | numberR[tram][i];
  }

  for(uint8_t j = 0; j < 3; j++) {
    newFrame[j + 5] = number1L[chuc][j] | number1R[donvi][j];
  }
  draw(newFrame, 8);
}
