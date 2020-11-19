//Quick and Dirty LED Driver
//Licensed under MIT, See the LICENCE file. 
//https://github.com/calw20/Arduino-Libraries/tree/master/ledDriver
//Cal.W 2020

#ifndef __LED_DRIVER_H__
#define __LED_DRIVER_H__

//This is very much needed
#include "Arduino.h"

enum LEDColours {
    BLACK   = B000,
    WHITE   = B111,
    RED     = B100,
    GREEN   = B010,
    BLUE    = B001,
    YELLOW  = B110,
    CYAN    = B011,
    MAGENTA = B101
};

class LED {
    public:
        LED(int sPin, LEDColours sColour = RED);
    
    public:
        LEDColours getColour();
        bool setColour(LEDColours newColour);
    
    private:
        int pin
        LEDColours currentColour = LEDColours::BLACK;
        LEDColours colour = LEDColours::WHITE; //Will basically only work on B/W
}

/*[TODO] Implement BiColour LED's
class BiColourLED: public LED {
        public:
        BiColourLED(int sPinA, int sPinB, bool commonGround);        
        bool init(); 
    
    public:
        bool setColour(LEDColours newColour);

    private:
        int pinA, pinB;
        LEDColours currentColour = LEDColours::BLACK;
}*/

class TriColourLED: public LED {
    public:
        ColourLED(int sPinR, int sPinG, int sPinB);        
        bool init(); 
    
    public:
        bool setColour(LEDColours newColour);

    private:
        int pinR, pinG, pinB;
        LEDColours currentColour = LEDColours::BLACK;
};

#endif