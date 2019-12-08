#include "keypad.h"

Keypad keypad(14, 1);

void setup() 
{
  //Serial.begin(9600);
}

void loop() 
{
  keypad.update();
  delay(50);
}
