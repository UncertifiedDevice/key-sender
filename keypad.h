#ifndef KEYPAD_H
#define KEYPAD_H

#include <Wire.h>
#include "buttons.h"

                     //A  B  C  D
const char KA0[] = {4, 1, 2, 3, 4};
                     //E  F  G  H
const char KA1[] = {4, 5, 6, 7, 8};
                     //I  J   K   L   M   N
const char KA2[] = {6, 9, 10, 11, 12, 13, 14};
                     //O   P   Q   R
const char KA3[] = {4, 15, 16, 17, 18};
                     //S   T   U   V
const char KA4[] = {4, 19, 20, 21, 22};
                     //W   X   Y   Z
const char KA5[] = {4, 23, 24, 25, 26};
//The first number in each of these arrays indicates the number of character ids inside

//An array of pointers to the arrays above, can be dereferenced the same way a 2D array can
const char *KEYARRAY[] = {KA0, KA1, KA2, KA3, KA4, KA5};

//Class that interprets button inputs and timings to determine which character the
//user is inputting, and then sending that character to a defined address on the i2c bus
class Keypad
{
  public:
    //Constructor initializes the pin to be used for buttons and the address to send characters to
    Keypad(short int pin, short int targetAddress)
    : m_keys(pin)
    , m_targetAddress(targetAddress)
    {
      //Connect to the i2c bus, no address necessary to send
      Wire.begin();
    }

    //Calls update() on the Buttons class and interprets button inputs, updates timers
    void update()
    {
      //Buttons::update()
      m_keys.update();

      //The button currently pressed, -1 for no button pressed
      short int button = m_keys.buttonDown();

      //Execute code if a button was pressed
      if(button >= 0 && 6 > button)
      {
        //If a new / different button was pressed
        if(button != m_lastButton)
        {
          //If the last button was a valid button
          if(m_lastButton >= 0 && 6 > m_lastButton)
          {
            //Push the last character and start interpreting input from another button
            pushCharacter();
          }

          //Update the last button
          m_lastButton = button;
          //Reset the consecutive presses, new button was only pressed once
          m_consecutivePresses = 0;
        }

        //Increment the consecutivePresses value with an upper bounds of the amount of characters available on the current button
        //loops around to 0 if pressed too many times
        m_consecutivePresses = (m_consecutivePresses + 1 >= KEYARRAY[button][0] + 1) ? 1 : m_consecutivePresses + 1;

        //Update the key / activity timer to timout in 500ms if no button is pressed
        m_keyTime = millis() + 500;

        //Used for debugging purposes
        //Serial.println(64 + KEYARRAY[button][m_consecutivePresses]);
      }

      //If the key / activity timer is up, execute this code
      if(millis() >= m_keyTime)
      {
        //The timer has timed out, commit the current character
        pushCharacter();

        //Reset the last pressed button to blank
        m_lastButton = -1;

        //Update the key / activity timer to timeout in 1500ms, this will send out a blank character
        //to the target device on the i2c bus every 1500ms if nothing is being pressed
        m_keyTime = millis() + 1500;
      }
    }

    //Used to change the target address on the fly, unused in this project
    void setTargetAddress(short int address)
    {
      m_targetAddress = address;
    }

    //Blank destructor, nothing to do here
    virtual ~Keypad() {}

  protected:
    //Pushes the current character to the target address on the i2c bus
    void pushCharacter()
    {
      //Character is blank by default
      char character = 0;

      //If the last button pressed is a valid button
      if(m_lastButton >= 0 && 6 > m_lastButton)
      {
        //Change the character to the character in the array matching the current button
        //and the amount of times it was pressed consecutively
        character = KEYARRAY[m_lastButton][m_consecutivePresses];
      }

      //Begin a transmission on the i2c bus to the target address
      Wire.beginTransmission(m_targetAddress);

      //Send the a single character
      Wire.write(character);

      //End the transmission
      Wire.endTransmission();
    }

  private:
    //The buttons class which handles button inputs
    Buttons m_keys;

    //Stores the button that was pressed last update()
    short int m_lastButton = -1;

    //Stores the number of times the current button was pressed consecutively
    short int m_consecutivePresses = 0;

    //Key / Activity timer used to time button presses or lack thereof
    unsigned long int m_keyTime = 0;

    //Target address to send characters to on the i2c bus
    short int m_targetAddress;
};

#endif // KEYPAD_H
