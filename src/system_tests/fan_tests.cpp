#include "fan_tests.h"

bool fanTest(FanController testController, long fanSpeed, unsigned long timeout){
    DBG_FPRINT_SVLN("[EDF & Controller Test] Setting a fan speed of: ", fanSpeed);
    testController.setSpeed(fanSpeed);
    DBG_FPRINT_SVLN("[EDF & Controller Test] Running EDF's for a time (ms) of: ", timeout);
    unsigned long startTime = millis();
    while (millis() - startTime < timeout){
        testController.writeSpeed();
    }
    testController.setSpeed(0);
    testController.writeSpeed();
    DBG_FPRINTLN("[EDF & Controller Test] Test Complete. System should be reset to original setings.");

    return true;
}