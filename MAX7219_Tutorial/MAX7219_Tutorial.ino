#include "LedControl.h"

LedControl lc=LedControl(12,11,10,1);  // Pin and # of Displays

// Put values in arrays
uint8_t frame[] =
{
   B00011000,  // First frame of invader #1
   B00111100,
   B01111110,
   B11011011,
   B11111111,
   B00100100,
   B01011010,
   B10100101
};

void setup()
{
  lc.shutdown(0,false);  // Wake up displays
  lc.shutdown(1,false);
  lc.setIntensity(0,5);  // Set intensity levels
  lc.setIntensity(1,5);
  lc.clearDisplay(0);  // Clear Displays
  lc.clearDisplay(1);
}

//  Take values in Arrays and Display them
void draw(uint8_t *frame)
{
  for (uint8_t i = 0; i < 8; i++)  
  {
    lc.setRow(0,i,frame[i]);
  }
}

void loop()
{
// Put #1 frame on both Display
    draw(frame);
    delay(400);
}
