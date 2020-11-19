//Quick and Dirty LED Driver
//Licensed under MIT, See the LICENCE file.
//https://github.com/calw20/Arduino-Libraries/tree/master/ledDriver
//Cal.W 2020

#include "ledDriver.h"

//MonoColour
LED::LED(int sPin, LEDColours sColour){
    pin = sPin;
    colour = sColour;
    
    pinMode(pin, OUTPUT);

    setColour(currentColour);
}

bool LED::setColour(LEDColours newColour){
    //Ignore calls to change it to green if its a red led
    //This can be done due to the way the LEDColours is defined
    currentColour = static_cast<LEDColours>(newColour & colour);
    
    pinMode(pin, currentColour);

    return true;
}

LEDColours LED::getColour(){
    return currentColour;
}


//TriColour
TriColourLED::TriColourLED(int sPinR, int sPinG, int sPinB) : LED(0, WHITE){
    pinR = sPinR;
    pinG = sPinG;
    pinB = sPinB;

    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);

    setColour(currentColour);
}

bool TriColourLED::setColour(LEDColours newColour){
    currentColour = newColour;
    digitalWrite(pinR, (currentColour & B100));
    digitalWrite(pinG, (currentColour & B010));
    digitalWrite(pinB, (currentColour & B001));
    return true;
}