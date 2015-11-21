
#define TEMPERTURE  0x00
#define SRF05     0x01
#define GAME1   0x02
#define GAME2   0x03
#define GRAPH   0x04

#define BUTTON1   0x04
#define BUTTON2   0x05
#define BUTTON3   0x06
#define BUTTON4   0x07

volatile uint8_t currentState;

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  
  attachInterrupt(2, isr, FALLING);
}

void loop() {

}

void isr() {
  delay(20);
  if(digitalRead(BUTTON1) == 0) {
    currentState++;
    if(currentState == GRAPH) {
      currentState = TEMPERTURE;
    }
  } else if(digitalRead(BUTTON2) == 0) {
    
  } else if(digitalRead(BUTTON3) == 0) {
    
  } else {
    
  }
}


