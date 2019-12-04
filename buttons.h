#ifndef BUTTONS_H
#define BUTTONS_H

class buttons
{
    public:
        buttons(short int _pin)
        :   pin(_pin)
        {
            pinMode(pin, INPUT_PULLUP);
        }

        //Poll buttons, to be called in a loop
        void update()
        {
            lastButton = currentButton;

            if(analogRead(pin) < 1000)
            {
                currentButton = (analogRead(pin) - 10) / 15;
            }
            else
            {
                currentButton = -1;
            }
        }

        //Check what happened to an individual button
        bool buttonDown(short int button)
        {
            if(lastButton != button && currentButton == button)
            {
                return true;
            }

            return false;
        }

        bool buttonUp(short int button)
        {
            if(lastButton == button && currentButton != button)
            {
                return true;
            }

            return false;
        }

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
        short int buttonDown()
        {
            if(lastButton != currentButton)
            {
                return currentButton;
            }

            return -1;
        }

        short int buttonUp()
        {
            if(lastButton != currentButton)
            {
                return lastButton;
            }

            return -1;
        }

        short int buttonHeld()
        {
            return currentButton;
        }

        virtual ~buttons() {}

    //private:
        short int currentButton = 0;
        short int lastButton = 0;

        const short int pin;
};

#endif // BUTTONS_H
