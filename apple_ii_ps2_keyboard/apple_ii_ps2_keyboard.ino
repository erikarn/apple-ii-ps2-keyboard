#include <PS2Keyboard.h>

const int DataPin = 3;
const int IRQpin =  2;

PS2Keyboard keyboard;


// Arduino nano - A6, A7 are analog only!

int outputMap[] = { 5,6,7,8,9,10,11 };
int strobePin = 4;
int strobeDurationUs = 125;
int delayBetweenKeys = 25;

void
appleInit()
{
  // Apple keyboard init
 
  pinMode(strobePin, OUTPUT);
  digitalWrite(strobePin, LOW);
  for (int i = 0; i < 7; i++) {
    
    pinMode(outputMap[i], OUTPUT);
    digitalWrite(outputMap[i], HIGH);
  }  
}

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");

  appleInit();
}

void appleSend(char c)
{
  for(int i=0; i<7; i++) {

    if(c & (1 << i)) {
      digitalWrite(outputMap[i], HIGH);
    } else {
      digitalWrite(outputMap[i], LOW);
    }
    
  }
  delay(1); // Ensure everything's nice and settled before toggling strobe
  
  // Toggle strobe
  digitalWrite(strobePin, HIGH);
  delayMicroseconds(strobeDurationUs);
  digitalWrite(strobePin, LOW);
  delay(delayBetweenKeys);
}

void loop() {
  if (keyboard.available()) {
    
    // read the next key
    char c = keyboard.read();
    
    // check for some of the special keys
    if (c == PS2_ENTER) {
      Serial.println();
    } else if (c == PS2_TAB) {
      Serial.print("[Tab]");
    } else if (c == PS2_ESC) {
      Serial.print("[ESC]");
    } else if (c == PS2_PAGEDOWN) {
      Serial.print("[PgDn]");
    } else if (c == PS2_PAGEUP) {
      Serial.print("[PgUp]");
    } else if (c == PS2_LEFTARROW) {
      Serial.print("[Left]");
    } else if (c == PS2_RIGHTARROW) {
      Serial.print("[Right]");
    } else if (c == PS2_UPARROW) {
      Serial.print("[Up]");
    } else if (c == PS2_DOWNARROW) {
      Serial.print("[Down]");
    } else if (c == PS2_DELETE) {
      Serial.print("[Del]");
    } else {
      // otherwise, just print all normal characters
      Serial.print(c);
    }

    // Apple-ii keyboard sending logic
    // We send ASCII values between 0 and 127, everything >= 127 is
    // a potentially extended key press and we don't want to handle
    // it here.
    if (c == PS2_TAB) {
      appleSend(0x09);
    } else if (c == PS2_ESC) {
      appleSend(0x1b);
    } else if (c == PS2_LEFTARROW) {
      appleSend(0x08);
    } else if (c == PS2_RIGHTARROW) {
      appleSend(0x15);
    } else if (c == PS2_UPARROW) {
      appleSend(0x0b);
    } else if (c == PS2_DOWNARROW) {
      appleSend(0x0a);
    } else if (c == PS2_DELETE) {
      appleSend(0x7f);
    } else if (c <= 127) {
      appleSend(c);
    }
  }
}
