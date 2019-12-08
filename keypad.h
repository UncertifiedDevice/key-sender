#ifndef KEYPAD_H
#define KEYPAD_H

#include <Wire.h>
#include "buttons.h"

const char *KEYARRAY[] = {"4ABCD",
                          "4EFGH",
                          "6IJKLMN",
                          "4OPQR",
                          "4STUV",
                          "4WXYZ"};

class Keypad
{
  public:
    Keypad(short int pin, short int targetAddress)
    : m_keys(pin)
    , m_targetAddress(targetAddress)
    {
      Wire.begin();
    }

    void update()
    {
      m_keys.update();

      short int button = m_keys.buttonDown();
      if(button >= 0 && 6 > button)
      {
        if(button != m_lastButton)
        {
          if(m_lastButton >= 0)
          {
            pushCharacter();
          }

          m_lastButton = button;
          m_consecutivePresses = 0;
        }

        m_consecutivePresses = (m_consecutivePresses + 1 >= KEYARRAY[button][0]) ? 1 : m_consecutivePresses + 1;
        m_keyTime = millis() + 500;

        Serial.println(KEYARRAY[button][m_consecutivePresses]);
      }

      if(millis() >= m_keyTime)
      {
        pushCharacter();
        m_lastButton = -1;
        m_keyTime = millis() + 1500;
      }
    }

    void setTargetAddress(short int address)
    {
      m_targetAddress = address;
    }

    virtual ~Keypad() {}

  protected:
    void pushCharacter()
    {
      char character = ' ';

      if(m_lastButton >= 0 && 6 > m_lastButton)
      {
        character = KEYARRAY[m_lastButton][m_consecutivePresses];
      }

      Wire.beginTransmission(m_targetAddress);
      Wire.write(character);
      Wire.endTransmission();
    }

  private:
    buttons m_keys;

    short int m_lastButton = -1;
    short int m_consecutivePresses = 0;

    unsigned long int m_keyTime = 0;

    short int m_targetAddress;
};

#endif // KEYPAD_H
