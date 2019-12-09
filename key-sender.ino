#include "keypad.h"

//Creates an instance of the keypad class using pin 14 (Analog 0) for buttons
//and sending characters to the device on address 0 on the i2c bus
Keypad keypad(14, 1);

void setup() 
{
  //Used for debugging purposes
  //Serial.begin(9600);
}

void loop() 
{
  //Call the keypad update function every 50ms
  keypad.update();
  delay(50);
}
