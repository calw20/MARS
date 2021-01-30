#include "led_tests.h"

bool ledTest(RootModule rootModule, unsigned long msDelay){
    TriColourLED *cLED[2];
    LEDColours currentColour[2];
    cLED[0] = rootModule.cLED1;
    cLED[1] = rootModule.cLED2;
    currentColour[0] = cLED[0]->getColour();
    currentColour[1] = cLED[1]->getColour();


    LEDColours colours[8] = { LEDColours::BLACK, LEDColours::WHITE, 
        LEDColours::RED, LEDColours::GREEN, LEDColours::BLUE, 
        LEDColours::YELLOW, LEDColours::CYAN, LEDColours::MAGENTA };

    for(int i = 0; i<9; i++){
        cLED[0]->setColour(colours[i]);
        cLED[1]->setColour(colours[i]);
        delay(msDelay);
    }

    cLED[0]->setColour(currentColour[0]);
    cLED[1]->setColour(currentColour[1]);

    return true;
}