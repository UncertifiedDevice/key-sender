#ifndef BUTTONS_H
#define BUTTONS_H

//Class that detects small analog changes on a pin to detect which button
//is pressed out of a number of buttons connected in series with resistors
class Buttons
{
  public:
    //Constructor initializes the pin to be used and sets its mode
    Buttons(short int _pin)
    :   pin(_pin)
    {
      //Using pullup resistor otherwise there is a lot of fluctuation
      pinMode(pin, INPUT_PULLUP);
    }

    //Poll buttons, to be called in a loop
    void update()
    {
      //Move the current button to the last button pressed value
      lastButton = currentButton;

      //Read the analog pin, update the current button pressed if a button was pressed
      if(analogRead(pin) < 1000)
      {
        currentButton = (analogRead(pin) - 10) / 15;
      }
      //Otherwise set to -1 to say nothing is currently pressed
      else
      {
        currentButton = -1;
      }
    }

    //Check what happened to an individual button

    //This button was pressed
    bool buttonDown(short int button)
    {
      if(lastButton != button && currentButton == button)
      {
        return true;
      }

      return false;
    }

    //This button was released
    bool buttonUp(short int button)
    {
      if(lastButton == button && currentButton != button)
      {
        return true;
      }

      return false;
    }

    //This button is currently held
    bool buttonHeld(short int button)
    {
      if(currentButton == button)
      {
        return true;
      }

      return false;
    }

    //Check which button something has happened to
    //Note: -1 means nothing happened to any button

    //A button was pressed
    short int buttonDown()
    {
      if(lastButton != currentButton)
      {
        return currentButton;
      }

      return -1;
    }

    //A button was released
    short int buttonUp()
    {
      if(lastButton != currentButton)
      {
        return lastButton;
      }

      return -1;
    }

    //A button is currently held
    short int buttonHeld()
    {
      return currentButton;
    }

    //Blank destructor, nothing to do here
    virtual ~Buttons() {}

  private:
    //The button currently being pressed, changes every update()
    short int currentButton = 0;
    //The button that was being pressed the last update()
    short int lastButton = 0;

    //The pin to read analog input from
    const short int pin;
};

#endif // BUTTONS_H
