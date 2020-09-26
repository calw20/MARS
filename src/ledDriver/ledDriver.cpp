//Quick and Dirty LED Driver
//Licensed under MIT, See the LICENCE file.
//https://github.com/calw20/Arduino-Libraries/tree/master/ledDriver
//Cal.W 2020

#include "ledDriver.h"

ColourLED::ColourLED(int sPinR, int sPinG, int sPinB){
    pinR = sPinR;
    pinG = sPinG;
    pinB = sPinB;

    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);

    setColour(currentColour);
}

LEDColours ColourLED::getColour(){
    return currentColour;
}

bool ColourLED::setColour(LEDColours newColour){
    currentColour = newColour;
    digitalWrite(pinR, (currentColour & B100));
    digitalWrite(pinG, (currentColour & B010));
    digitalWrite(pinB, (currentColour & B001));
    return true;
}