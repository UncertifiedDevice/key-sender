#include "buttons.h"

buttons btns(19);

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  btns.update();

  if(btns.buttonDown() >= 0)
  {
    Serial.println(btns.buttonDown());
  }

  delay(50);
}
