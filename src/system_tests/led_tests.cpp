#include "led_tests.h"

bool ledTest(RootModule rootModule, unsigned long msDelay){
    DBG_FPRINTLN("Runing LED Colour Test");
    TriColourLED *cLED[2];
    LEDColours currentColour[2];
    cLED[0] = rootModule.cLED1;
    cLED[1] = rootModule.cLED2;
    currentColour[0] = cLED[0]->getColour();
    currentColour[1] = cLED[1]->getColour();


    LEDColours colours[8] = { LEDColours::BLACK, LEDColours::WHITE, 
        LEDColours::RED, LEDColours::GREEN, LEDColours::BLUE, 
        LEDColours::YELLOW, LEDColours::CYAN, LEDColours::MAGENTA };

    DBG_FPRINTFN("System will hang for ~", "%i seconds...", (9*msDelay)/1000);
    DBG_FPRINT("Testing Colour:  "); //Yes this double space is needed
    for (int i = 0; i <= 8; i++){
        DBG_PRINTF("\b%i", i); //[HACK] Nope. This is just haunted. If we don't print the number in 'i' it goes into an infinite loop.
        cLED[0]->setColour(colours[i]);
        cLED[1]->setColour(colours[i]);
        delay(msDelay);
    }
    DBG_PRINTLN();
    DBG_PRINTLN("Test Complete.");

    cLED[0]->setColour(currentColour[0]);
    cLED[1]->setColour(currentColour[1]);

    return true;
}